/*
 * Read-Copy Update mechanism for mutual exclusion 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright IBM Corporation, 2001
 *
 * Author: Dipankar Sarma <dipankar@in.ibm.com>
 * 
 * Based on the original work by Paul McKenney <paulmck@us.ibm.com>
 * and inputs from Rusty Russell, Andrea Arcangeli and Andi Kleen.
 * Papers:
 * http://www.rdrop.com/users/paulmck/paper/rclockpdcsproof.pdf
 * http://lse.sourceforge.net/locking/rclock_OLS.2001.05.01c.sc.pdf (OLS2001)
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 * 		http://lse.sourceforge.net/locking/rcupdate.html
 *
 */

#ifndef __LINUX_RCUPDATE_H
#define __LINUX_RCUPDATE_H

#ifdef __KERNEL__

#include <linux/cache.h>
#include <linux/spinlock.h>
#include <linux/threads.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>
#include <linux/seqlock.h>
#include <linux/lockdep.h>

/**
 * struct rcu_head - callback structure for use with RCU
 * @next: next update requests in a list
 * @func: actual update function to call after the grace period.
 */
struct rcu_head {
	struct rcu_head *next;
	void (*func)(struct rcu_head *head);
};

#ifdef CONFIG_CLASSIC_RCU
#include <linux/rcuclassic.h>
#else /* #ifdef CONFIG_CLASSIC_RCU */
#include <linux/rcupreempt.h>
#endif /* #else #ifdef CONFIG_CLASSIC_RCU */

#define RCU_HEAD_INIT 	{ .next = NULL, .func = NULL }
#define RCU_HEAD(head) struct rcu_head head = RCU_HEAD_INIT
#define INIT_RCU_HEAD(ptr) do { \
       (ptr)->next = NULL; (ptr)->func = NULL; \
} while (0)



#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern struct lockdep_map rcu_lock_map;
# define rcu_read_acquire()	lock_acquire(&rcu_lock_map, 0, 0, 2, 1, _THIS_IP_)
# define rcu_read_release()	lock_release(&rcu_lock_map, 1, _THIS_IP_)
#else
# define rcu_read_acquire()	do { } while (0)
# define rcu_read_release()	do { } while (0)
#endif

/**
 * rcu_read_lock - mark the beginning of an RCU read-side critical section.
 *
 * When synchronize_rcu() is invoked on one CPU while other CPUs
 * are within RCU read-side critical sections, then the
 * synchronize_rcu() is guaranteed to block until after all the other
 * CPUs exit their critical sections.  Similarly, if call_rcu() is invoked
 * on one CPU while other CPUs are within RCU read-side critical
 * sections, invocation of the corresponding RCU callback is deferred
 * until after the all the other CPUs exit their critical sections.
 *
 * Note, however, that RCU callbacks are permitted to run concurrently
 * with RCU read-side critical sections.  One way that this can happen
 * is via the following sequence of events: (1) CPU 0 enters an RCU
 * read-side critical section, (2) CPU 1 invokes call_rcu() to register
 * an RCU callback, (3) CPU 0 exits the RCU read-side critical section,
 * (4) CPU 2 enters a RCU read-side critical section, (5) the RCU
 * callback is invoked.  This is legal, because the RCU read-side critical
 * section that was running concurrently with the call_rcu() (and which
 * therefore might be referencing something that the corresponding RCU
 * callback would free up) has completed before the corresponding
 * RCU callback is invoked.
 *
 * RCU read-side critical sections may be nested.  Any deferred actions
 * will be deferred until the outermost RCU read-side critical section
 * completes.
 *
 * It is illegal to block while in an RCU read-side critical section.
 */
#define rcu_read_lock() __rcu_read_lock()

/**
 * rcu_read_unlock - marks the end of an RCU read-side critical section.
 *
 * See rcu_read_lock() for more information.
 */
#define rcu_read_unlock() __rcu_read_unlock()

/*
 * So where is rcu_write_lock()?  It does not exist, as there is no
 * way for writers to lock out RCU readers.  This is a feature, not
 * a bug -- this property is what provides RCU's performance benefits.
 * Of course, writers must coordinate with each other.  The normal
 * spinlock primitives work well for this, but any other technique may be
 * used as well.  RCU does not care how the writers keep out of each
 * others' way, as long as they do so.
 */

/**
 * rcu_read_lock_bh - mark the beginning of a softirq-only RCU critical section
 *
 * This is equivalent of rcu_read_lock(), but to be used when updates
 * are being done using call_rcu_bh(). Since call_rcu_bh() callbacks
 * consider completion of a softirq handler to be a quiescent state,
 * a process in RCU read-side critical section must be protected by
 * disabling softirqs. Read-side critical sections in interrupt context
 * can use just rcu_read_lock().
 *
 */
#define rcu_read_lock_bh() __rcu_read_lock_bh()

/*
 * rcu_read_unlock_bh - marks the end of a softirq-only RCU critical section
 *
 * See rcu_read_lock_bh() for more information.
 */
#define rcu_read_unlock_bh() __rcu_read_unlock_bh()

/*
 * Prevent the compiler from merging or refetching accesses.  The compiler
 * is also forbidden from reordering successive instances of ACCESS_ONCE(),
 * but only when the compiler is aware of some particular ordering.  One way
 * to make the compiler aware of ordering is to put the two invocations of
 * ACCESS_ONCE() in different C statements.
 *
 * This macro does absolutely -nothing- to prevent the CPU from reordering,
 * merging, or refetching absolutely anything at any time.
 */
#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

/**
 * rcu_dereference - fetch an RCU-protected pointer in an
 * RCU read-side critical section.  This pointer may later
 * be safely dereferenced.
 *
 * Inserts memory barriers on architectures that require them
 * (currently only the Alpha), and, more importantly, documents
 * exactly which pointers are protected by RCU.
 */

#define rcu_dereference(p)     ({ \
				typeof(p) _________p1 = ACCESS_ONCE(p); \
				smp_read_barrier_depends(); \
				(_________p1); \
				})

/**
 * rcu_assign_pointer - assign (publicize) a pointer to a newly
 * initialized structure that will be dereferenced by RCU read-side
 * critical sections.  Returns the value assigned.
 *
 * Inserts memory barriers on architectures that require them
 * (pretty much all of them other than x86), and also prevents
 * the compiler from reordering the code that initializes the
 * structure after the pointer assignment.  More importantly, this
 * call documents which pointers will be dereferenced by RCU read-side
 * code.
 */

#define rcu_assign_pointer(p, v)	({ \
						smp_wmb(); \
						(p) = (v); \
					})

/**
 * synchronize_sched - block until all CPUs have exited any non-preemptive
 * kernel code sequences.
 *
 * This means that all preempt_disable code sequences, including NMI and
 * hardware-interrupt handlers, in progress on entry will have completed
 * before this primitive returns.  However, this does not guarantee that
 * softirq handlers will have completed, since in some kernels, these
 * handlers can run in process context, and can block.
 *
 * This primitive provides the guarantees made by the (now removed)
 * synchronize_kernel() API.  In contrast, synchronize_rcu() only
 * guarantees that rcu_read_lock() sections will have completed.
 * In "classic RCU", these two guarantees happen to be one and
 * the same, but can differ in realtime RCU implementations.
 */
#define synchronize_sched() __synchronize_sched()

/**
 * call_rcu - Queue an RCU callback for invocation after a grace period.
 * @head: structure to be used for queueing the RCU updates.
 * @func: actual update function to be invoked after the grace period
 *
 * The update function will be invoked some time after a full grace
 * period elapses, in other words after all currently executing RCU
 * read-side critical sections have completed.  RCU read-side critical
 * sections are delimited by rcu_read_lock() and rcu_read_unlock(),
 * delimited by rcu_read_lock() and rcu_read_unlock(),
 * and may be nested.
 */
#ifdef CONFIG_CLASSIC_RCU
#define call_rcu call_rcu_classic
#else /* #ifdef CONFIG_CLASSIC_RCU */
#define call_rcu call_rcu_preempt
#endif /* #else #ifdef CONFIG_CLASSIC_RCU */

/**
 * call_rcu_bh - Queue an RCU for invocation after a quicker grace period.
 * @head: structure to be used for queueing the RCU updates.
 * @func: actual update function to be invoked after the grace period
 *
 * The update function will be invoked some time after a full grace
 * period elapses, in other words after all currently executing RCU
 * read-side critical sections have completed. call_rcu_bh() assumes
 * that the read-side critical sections end on completion of a softirq
 * handler. This means that read-side critical sections in process
 * context must not be interrupted by softirqs. This interface is to be
 * used when most of the read-side critical sections are in softirq context.
 * RCU read-side critical sections are delimited by rcu_read_lock() and
 * rcu_read_unlock(), * if in interrupt context or rcu_read_lock_bh()
 * and rcu_read_unlock_bh(), if in process context. These may be nested.
 */
extern void FASTCALL(call_rcu_bh(struct rcu_head *head,
				void (*func)(struct rcu_head *head)));

/* Exported common interfaces */
extern void synchronize_rcu(void);
extern void rcu_barrier(void);
extern long rcu_batches_completed(void);
extern long rcu_batches_completed_bh(void);

/* Internal to kernel */
extern void rcu_check_callbacks(int cpu, int user);
extern long rcu_batches_completed(void);
extern long rcu_batches_completed_bh(void);
extern void rcu_check_callbacks(int cpu, int user);
extern void rcu_init(void);
extern int  rcu_needs_cpu(int cpu);
extern int  rcu_pending(int cpu);
struct softirq_action;
extern void rcu_restart_cpu(int cpu);

DECLARE_PER_CPU(int, rcu_data_passed_quiesc);

/*
 * Increment the quiescent state counter.
 * The counter is a bit degenerated: We do not need to know
 * how many quiescent states passed, just if there was at least
 * one since the start of the grace period. Thus just a flag.
 */
static inline void rcu_qsctr_inc(int cpu)
{
	per_cpu(rcu_data_passed_quiesc, cpu) = 1;
}

struct dentry;

#ifdef CONFIG_PREEMPT_RCU_BOOST
extern void init_rcu_boost_late(void);
extern void rcu_boost_readers(void);
extern void rcu_unboost_readers(void);
extern void __rcu_preempt_boost(void);
#ifdef CONFIG_RCU_TRACE
extern int rcu_trace_boost_create(struct dentry *rcudir);
extern void rcu_trace_boost_destroy(void);
#endif /* CONFIG_RCU_TRACE */
#define rcu_preempt_boost() /* cpp to avoid #include hell. */ \
	do { \
		if (unlikely(current->rcu_read_lock_nesting > 0)) \
			__rcu_preempt_boost(); \
	} while (0)
extern void __rcu_preempt_unboost(void);
#else /* #ifdef CONFIG_PREEMPT_RCU_BOOST */
static inline void init_rcu_boost_late(void)
{
}
static inline void rcu_preempt_boost(void)
{
}
static inline void __rcu_preempt_unboost(void)
{
}
static inline void rcu_boost_readers(void)
{
}
static inline void rcu_unboost_readers(void)
{
}
#ifdef CONFIG_RCU_TRACE
static inline int rcu_trace_boost_create(struct dentry *rcudir)
{
	return 0;
}
static inline void rcu_trace_boost_destroy(void)
{
}
#endif /* CONFIG_RCU_TRACE */
#endif /* #else #ifdef CONFIG_PREEMPT_RCU_BOOST */

#endif /* __KERNEL__ */
#endif /* __LINUX_RCUPDATE_H */

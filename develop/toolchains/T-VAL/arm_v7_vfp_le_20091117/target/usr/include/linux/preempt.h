#ifndef __LINUX_PREEMPT_H
#define __LINUX_PREEMPT_H

/*
 * include/linux/preempt.h - macros for accessing and manipulating
 * preempt_count (used for kernel preemption, interrupt count, etc.)
 */

#include <linux/thread_info.h>
#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/thread_info.h>
#include <linux/init.h>

#if defined(CONFIG_DEBUG_PREEMPT) || defined(CONFIG_CRITICAL_TIMING)
  extern void notrace add_preempt_count(unsigned int val);
  extern void notrace sub_preempt_count(unsigned int val);
  extern void notrace mask_preempt_count(unsigned int mask);
  extern void notrace unmask_preempt_count(unsigned int mask);
#else
# define add_preempt_count(val)	do { preempt_count() += (val); } while (0)
# define sub_preempt_count(val)	do { preempt_count() -= (val); } while (0)
# define mask_preempt_count(mask) \
		do { preempt_count() |= (mask); } while (0)
# define unmask_preempt_count(mask) \
		do { preempt_count() &= ~(mask); } while (0)
#endif

#ifdef CONFIG_CRITICAL_TIMING
  extern void touch_critical_timing(void);
  extern void stop_critical_timing(void);
#else
# define touch_critical_timing()	do { } while (0)
# define stop_critical_timing()	do { } while (0)
#endif

#define inc_preempt_count() add_preempt_count(1)
#define dec_preempt_count() sub_preempt_count(1)

#define preempt_count()		(current_thread_info()->preempt_count)

#ifdef CONFIG_PREEMPT

asmlinkage void preempt_schedule(void);
asmlinkage void preempt_schedule_irq(void);

#define preempt_disable() \
do { \
	inc_preempt_count(); \
	barrier(); \
} while (0)

#define __preempt_enable_no_resched() \
do { \
	barrier(); \
	dec_preempt_count(); \
} while (0)


#ifdef CONFIG_DEBUG_PREEMPT
extern void notrace preempt_enable_no_resched(void);
#else
# define preempt_enable_no_resched() __preempt_enable_no_resched()
#endif

#define preempt_check_resched() \
do { \
	if (unlikely(test_thread_flag(TIF_NEED_RESCHED))) \
		preempt_schedule(); \
} while (0)

#define preempt_check_resched_delayed() \
do { \
	if (unlikely(test_thread_flag(TIF_NEED_RESCHED_DELAYED))) \
		preempt_schedule(); \
} while (0)

#define preempt_enable() \
do { \
	__preempt_enable_no_resched(); \
	barrier(); \
	preempt_check_resched(); \
} while (0)

#else

#define preempt_disable()		do { } while (0)
#define preempt_enable_no_resched()	do { } while (0)
#define __preempt_enable_no_resched()	do { } while (0)
#define preempt_enable()		do { } while (0)
#define preempt_check_resched()		do { } while (0)
#define preempt_check_resched_delayed()	do { } while (0)

#define preempt_schedule_irq()		do { } while (0)

#endif

#ifdef CONFIG_PREEMPT_NOTIFIERS

struct preempt_notifier;

/**
 * preempt_ops - notifiers called when a task is preempted and rescheduled
 * @sched_in: we're about to be rescheduled:
 *    notifier: struct preempt_notifier for the task being scheduled
 *    cpu:  cpu we're scheduled on
 * @sched_out: we've just been preempted
 *    notifier: struct preempt_notifier for the task being preempted
 *    next: the task that's kicking us out
 */
struct preempt_ops {
	void (*sched_in)(struct preempt_notifier *notifier, int cpu);
	void (*sched_out)(struct preempt_notifier *notifier,
			  struct task_struct *next);
};

/**
 * preempt_notifier - key for installing preemption notifiers
 * @link: internal use
 * @ops: defines the notifier functions to be called
 *
 * Usually used in conjunction with container_of().
 */
struct preempt_notifier {
	struct hlist_node link;
	struct preempt_ops *ops;
};

void preempt_notifier_register(struct preempt_notifier *notifier);
void preempt_notifier_unregister(struct preempt_notifier *notifier);

static inline void preempt_notifier_init(struct preempt_notifier *notifier,
				     struct preempt_ops *ops)
{
	INIT_HLIST_NODE(&notifier->link);
	notifier->ops = ops;
}

#endif

#endif /* __LINUX_PREEMPT_H */

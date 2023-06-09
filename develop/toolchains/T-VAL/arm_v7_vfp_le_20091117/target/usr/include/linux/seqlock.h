#ifndef __LINUX_SEQLOCK_H
#define __LINUX_SEQLOCK_H
/*
 * Reader/writer consistent mechanism without starving writers. This type of
 * lock for data where the reader wants a consistent set of information
 * and is willing to retry if the information changes.  Readers never
 * block but they may have to retry if a writer is in
 * progress. Writers do not wait for readers. 
 *
 * This is not as cache friendly as brlock. Also, this will not work
 * for data that contains pointers, because any writer could
 * invalidate a pointer that a reader was following.
 *
 * Expected reader usage:
 * 	do {
 *	    seq = read_seqbegin(&foo);
 * 	...
 *      } while (read_seqretry(&foo, seq));
 *
 *
 * On non-SMP the spin locks disappear but the writer still needs
 * to increment the sequence variables because an interrupt routine could
 * change the state of the data.
 *
 * Based on x86_64 vsyscall gettimeofday 
 * by Keith Owens and Andrea Arcangeli
 */

#include <linux/spinlock.h>
#include <linux/preempt.h>

typedef struct {
	unsigned sequence;
	spinlock_t lock;
} __seqlock_t;

typedef struct {
	unsigned sequence;
	raw_spinlock_t lock;
} __raw_seqlock_t;

#define seqlock_need_resched(seq) lock_need_resched(&(seq)->lock)

#ifdef CONFIG_PREEMPT_RT
typedef __seqlock_t seqlock_t;
#else
typedef __raw_seqlock_t seqlock_t;
#endif

typedef __raw_seqlock_t raw_seqlock_t;

/*
 * These macros triggered gcc-3.x compile-time problems.  We think these are
 * OK now.  Be cautious.
 */
#define __RAW_SEQLOCK_UNLOCKED(lockname) \
		{ 0, RAW_SPIN_LOCK_UNLOCKED(lockname) }

#ifdef CONFIG_PREEMPT_RT
# define __SEQLOCK_UNLOCKED(lockname) { 0, __SPIN_LOCK_UNLOCKED(lockname) }
#else
# define __SEQLOCK_UNLOCKED(lockname) __RAW_SEQLOCK_UNLOCKED(lockname)
#endif

#define SEQLOCK_UNLOCKED \
		 __SEQLOCK_UNLOCKED(old_style_seqlock_init)

#define raw_seqlock_init(x) \
	do { *(x) = (raw_seqlock_t) __RAW_SEQLOCK_UNLOCKED(x); spin_lock_init(&(x)->lock); } while (0)

#define seqlock_init(x) \
		do { *(x) = (seqlock_t) __SEQLOCK_UNLOCKED(x); spin_lock_init(&(x)->lock); } while (0)

#define DEFINE_SEQLOCK(x) \
		seqlock_t x = __SEQLOCK_UNLOCKED(x)

#define DEFINE_RAW_SEQLOCK(name) \
	raw_seqlock_t name __cacheline_aligned_in_smp = \
					__RAW_SEQLOCK_UNLOCKED(name)


/* Lock out other writers and update the count.
 * Acts like a normal spin_lock/unlock.
 * Don't need preempt_disable() because that is in the spin_lock already.
 */
static inline void __write_seqlock(seqlock_t *sl)
{
	spin_lock(&sl->lock);
	++sl->sequence;
	smp_wmb();
}

static __always_inline unsigned long __write_seqlock_irqsave(seqlock_t *sl)
{
	unsigned long flags;

	local_save_flags(flags);
	__write_seqlock(sl);
	return flags;
}

static inline void __write_sequnlock(seqlock_t *sl)
{
	smp_wmb();
	sl->sequence++;
	spin_unlock(&sl->lock);
}

#define __write_sequnlock_irqrestore(sl, flags)	__write_sequnlock(sl)

static inline int __write_tryseqlock(seqlock_t *sl)
{
	int ret = spin_trylock(&sl->lock);

	if (ret) {
		++sl->sequence;
		smp_wmb();
	}
	return ret;
}

/* Start of read calculation -- fetch last complete writer token */
static __always_inline unsigned __read_seqbegin(const seqlock_t *sl)
{
	unsigned ret = sl->sequence;
	smp_rmb();
	return ret;
}

/* Test if reader processed invalid data.
 * If initial values is odd, 
 *	then writer had already started when section was entered
 * If sequence value changed
 *	then writer changed data while in section
 *    
 * Using xor saves one conditional branch.
 */
static inline int __read_seqretry(seqlock_t *sl, unsigned iv)
{
	int ret;

	smp_rmb();
	ret = (iv & 1) | (sl->sequence ^ iv);
	/*
	 * If invalid then serialize with the writer, to make sure we
	 * are not livelocking it:
	 */
	if (unlikely(ret)) {
		unsigned long flags;
		spin_lock_irqsave(&sl->lock, flags);
		spin_unlock_irqrestore(&sl->lock, flags);
	}
	return ret;
}

static __always_inline void __write_seqlock_raw(raw_seqlock_t *sl)
{
	spin_lock(&sl->lock);
	++sl->sequence;
	smp_wmb();
}

static __always_inline unsigned long
__write_seqlock_irqsave_raw(raw_seqlock_t *sl)
{
	unsigned long flags;

	local_irq_save(flags);
	__write_seqlock_raw(sl);
	return flags;
}

static __always_inline void __write_seqlock_irq_raw(raw_seqlock_t *sl)
{
	local_irq_disable();
	__write_seqlock_raw(sl);
}

static __always_inline void __write_seqlock_bh_raw(raw_seqlock_t *sl)
{
	local_bh_disable();
	__write_seqlock_raw(sl);
}

static __always_inline void __write_sequnlock_raw(raw_seqlock_t *sl)
{
	smp_wmb();
	sl->sequence++;
	spin_unlock(&sl->lock);
}

static __always_inline void
__write_sequnlock_irqrestore_raw(raw_seqlock_t *sl, unsigned long flags)
{
	__write_sequnlock_raw(sl);
	local_irq_restore(flags);
	preempt_check_resched();
}

static __always_inline void __write_sequnlock_irq_raw(raw_seqlock_t *sl)
{
	__write_sequnlock_raw(sl);
	local_irq_enable();
	preempt_check_resched();
}

static __always_inline void __write_sequnlock_bh_raw(raw_seqlock_t *sl)
{
	__write_sequnlock_raw(sl);
	local_bh_enable();
}

static __always_inline int __write_tryseqlock_raw(raw_seqlock_t *sl)
{
	int ret = spin_trylock(&sl->lock);

	if (ret) {
		++sl->sequence;
		smp_wmb();
	}
	return ret;
}

static __always_inline unsigned __read_seqbegin_raw(const raw_seqlock_t *sl)
{
	unsigned ret = sl->sequence;
	smp_rmb();
	return ret;
}

static __always_inline int __read_seqretry_raw(const raw_seqlock_t *sl, unsigned iv)
{
	smp_rmb();
	return (iv & 1) | (sl->sequence ^ iv);
}

extern int __bad_seqlock_type(void);

/*
 * PICK_SEQ_OP() is a small redirector to allow less typing of the lock
 * types raw_seqlock_t, seqlock_t, at the front of the PICK_FUNCTION
 * macro.
 */
#define PICK_SEQ_OP(...) 	\
	PICK_FUNCTION(raw_seqlock_t *, seqlock_t *, ##__VA_ARGS__)
#define PICK_SEQ_OP_RET(...) \
	PICK_FUNCTION_RET(raw_seqlock_t *, seqlock_t *, ##__VA_ARGS__)

#define write_seqlock(sl) PICK_SEQ_OP(__write_seqlock_raw, __write_seqlock, sl)

#define write_sequnlock(sl)	\
	PICK_SEQ_OP(__write_sequnlock_raw, __write_sequnlock, sl)

#define write_tryseqlock(sl)	\
	PICK_SEQ_OP_RET(__write_tryseqlock_raw, __write_tryseqlock, sl)

#define read_seqbegin(sl) 	\
	PICK_SEQ_OP_RET(__read_seqbegin_raw, __read_seqbegin, sl)

#define read_seqretry(sl, iv)	\
	PICK_SEQ_OP_RET(__read_seqretry_raw, __read_seqretry, sl, iv)

#define write_seqlock_irqsave(lock, flags)			\
do {								\
	flags = PICK_SEQ_OP_RET(__write_seqlock_irqsave_raw,	\
		__write_seqlock_irqsave, lock);			\
} while (0)

#define write_seqlock_irq(lock)	\
	PICK_SEQ_OP(__write_seqlock_irq_raw, __write_seqlock, lock)

#define write_seqlock_bh(lock)	\
	PICK_SEQ_OP(__write_seqlock_bh_raw, __write_seqlock, lock)

#define write_sequnlock_irqrestore(lock, flags)		\
	PICK_SEQ_OP(__write_sequnlock_irqrestore_raw,	\
		__write_sequnlock_irqrestore, lock, flags)

#define write_sequnlock_bh(lock)	\
	PICK_SEQ_OP(__write_sequnlock_bh_raw, __write_sequnlock, lock)

#define write_sequnlock_irq(lock)	\
	PICK_SEQ_OP(__write_sequnlock_irq_raw, __write_sequnlock, lock)

static __always_inline
unsigned long __seq_irqsave_raw(raw_seqlock_t *sl)
{
	unsigned long flags;

	local_irq_save(flags);
	return flags;
}

static __always_inline unsigned long __seq_irqsave(seqlock_t *sl)
{
	unsigned long flags;

	local_save_flags(flags);
	return flags;
}

#define read_seqbegin_irqsave(lock, flags)				\
({									\
	flags = PICK_SEQ_OP_RET(__seq_irqsave_raw, __seq_irqsave, lock);\
	read_seqbegin(lock);						\
})

static __always_inline int
__read_seqretry_irqrestore(seqlock_t *sl, unsigned iv, unsigned long flags)
{
	return __read_seqretry(sl, iv);
}

static __always_inline int
__read_seqretry_irqrestore_raw(raw_seqlock_t *sl, unsigned iv,
			       unsigned long flags)
{
	int ret = read_seqretry(sl, iv);
	local_irq_restore(flags);
	preempt_check_resched();
	return ret;
}

#define read_seqretry_irqrestore(lock, iv, flags)			\
	PICK_SEQ_OP_RET(__read_seqretry_irqrestore_raw, 		\
		__read_seqretry_irqrestore, lock, iv, flags)

/*
 * Version using sequence counter only.
 * This can be used when code has its own mutex protecting the
 * updating starting before the write_seqcountbeqin() and ending
 * after the write_seqcount_end().
 */

typedef struct seqcount {
	unsigned sequence;
} seqcount_t;

#define SEQCNT_ZERO { 0 }
#define seqcount_init(x)	do { *(x) = (seqcount_t) SEQCNT_ZERO; } while (0)

/* Start of read using pointer to a sequence counter only.  */
static inline unsigned read_seqcount_begin(const seqcount_t *s)
{
	unsigned ret = s->sequence;
	smp_rmb();
	return ret;
}

/* Test if reader processed invalid data.
 * Equivalent to: iv is odd or sequence number has changed.
 *                (iv & 1) || (*s != iv)
 * Using xor saves one conditional branch.
 */
static inline int read_seqcount_retry(const seqcount_t *s, unsigned iv)
{
	smp_rmb();
	return (iv & 1) | (s->sequence ^ iv);
}


/*
 * Sequence counter only version assumes that callers are using their
 * own mutexing.
 */
static inline void write_seqcount_begin(seqcount_t *s)
{
	s->sequence++;
	smp_wmb();
}

static inline void write_seqcount_end(seqcount_t *s)
{
	smp_wmb();
	s->sequence++;
}
#endif /* __LINUX_SEQLOCK_H */

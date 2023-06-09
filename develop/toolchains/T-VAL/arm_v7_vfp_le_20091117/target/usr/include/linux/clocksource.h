/*  linux/include/linux/clocksource.h
 *
 *  This file contains the structure definitions for clocksources.
 *
 *  If you are not a clocksource, or timekeeping code, you should
 *  not be including this file!
 */
#ifndef _LINUX_CLOCKSOURCE_H
#define _LINUX_CLOCKSOURCE_H

#include <linux/types.h>
#include <linux/timex.h>
#include <linux/time.h>
#include <linux/list.h>
#include <linux/cache.h>
#include <linux/timer.h>
#include <asm/div64.h>
#include <asm/io.h>

/* clocksource cycle base type */
typedef u64 cycle_t;
struct clocksource;

extern unsigned long preempt_max_latency;
extern unsigned long preempt_thresh;
extern unsigned long preempt_mark_thresh;

struct ctl_table;
struct file;
extern int proc_preempt_max_latency(struct ctl_table *table, int write,
				    struct file *file, void __user *buffer,
				    size_t *length, loff_t *ppos);
extern int proc_preempt_threshold(struct ctl_table *table, int write,
				  struct file *file, void __user *buffer,
				  size_t *length, loff_t *ppos);
/**
 * struct clocksource - hardware abstraction for a free running counter
 *	Provides mostly state-free accessors to the underlying hardware.
 *
 * @name:		ptr to clocksource name
 * @list:		list head for registration
 * @rating:		rating value for selection (higher is better)
 *			To avoid rating inflation the following
 *			list should give you a guide as to how
 *			to assign your clocksource a rating
 *			1-99: Unfit for real use
 *				Only available for bootup and testing purposes.
 *			100-199: Base level usability.
 *				Functional for real use, but not desired.
 *			200-299: Good.
 *				A correct and usable clocksource.
 *			300-399: Desired.
 *				A reasonably fast and accurate clocksource.
 *			400-499: Perfect
 *				The ideal clocksource. A must-use where
 *				available.
 * @read:		returns a cycle value
 * @mask:		bitmask for two's complement
 *			subtraction of non 64 bit counters
 * @mult:		cycle to nanosecond multiplier
 * @shift:		cycle to nanosecond divisor (power of two)
 * @flags:		flags describing special properties
 * @vread:		vsyscall based read
 * @resume:		resume function for the clocksource, if necessary
 * @cycle_last:		Used internally by timekeeping core, please ignore.
 * @cycle_accumulated:	Used internally by timekeeping core, please ignore.
 * @cycle_montonic:	Used internally by timekeeping core, please ignore.
 * @cycle_interval:	Used internally by timekeeping core, please ignore.
 * @xtime_interval:	Used internally by timekeeping core, please ignore.
 * @xtime_nsec:		Used internally by timekeeping core, please ignore.
 * @error:		Used internally by timekeeping core, please ignore.
 */
struct clocksource {
	/*
	 * First part of structure is read mostly
	 */
	char *name;
	struct list_head list;
	int rating;
	cycle_t (*read)(void);
	cycle_t mask;
	u32 mult;
	u32 shift;
	unsigned long flags;
	cycle_t (*vread)(void);
	void (*resume)(void);
#ifdef CONFIG_IA64
	void *fsys_mmio;        /* used by fsyscall asm code */
#define CLKSRC_FSYS_MMIO_SET(mmio, addr)      ((mmio) = (addr))
#else
#define CLKSRC_FSYS_MMIO_SET(mmio, addr)      do { } while (0)
#endif

	/* timekeeping specific data, ignore */
	u64	xtime_interval;
	/*
	 * Second part is written at each timer interrupt
	 * Keep it in a different cache line to dirty no
	 * more than one cache line.
	 */
	struct {
		cycle_t cycle_last, cycle_accumulated, cycle_monotonic;
		cycle_t cycle_interval;
	} ____cacheline_aligned_in_smp;

	u64 xtime_nsec;
	s64 error;

#ifdef CONFIG_CLOCKSOURCE_WATCHDOG
	/* Watchdog related data, used by the framework */
	struct list_head wd_list;
	cycle_t wd_last;
#endif
};

/*
 * Clock source flags bits::
 */
#define CLOCK_SOURCE_IS_CONTINUOUS		0x01
#define CLOCK_SOURCE_MUST_VERIFY		0x02

#define CLOCK_SOURCE_WATCHDOG			0x10
#define CLOCK_SOURCE_VALID_FOR_HRES		0x20

/* simplify initialization of mask field */
#define CLOCKSOURCE_MASK(bits) (cycle_t)(bits<64 ? ((1ULL<<bits)-1) : -1)

/**
 * clocksource_khz2mult - calculates mult from khz and shift
 * @khz:		Clocksource frequency in KHz
 * @shift_constant:	Clocksource shift factor
 *
 * Helper functions that converts a khz counter frequency to a timsource
 * multiplier, given the clocksource shift value
 */
static inline u32 clocksource_khz2mult(u32 khz, u32 shift_constant)
{
	/*  khz = cyc/(Million ns)
	 *  mult/2^shift  = ns/cyc
	 *  mult = ns/cyc * 2^shift
	 *  mult = 1Million/khz * 2^shift
	 *  mult = 1000000 * 2^shift / khz
	 *  mult = (1000000<<shift) / khz
	 */
	u64 tmp = ((u64)1000000) << shift_constant;

	tmp += khz/2; /* round for do_div */
	do_div(tmp, khz);

	return (u32)tmp;
}

/**
 * clocksource_hz2mult - calculates mult from hz and shift
 * @hz:			Clocksource frequency in Hz
 * @shift_constant:	Clocksource shift factor
 *
 * Helper functions that converts a hz counter
 * frequency to a timsource multiplier, given the
 * clocksource shift value
 */
static inline u32 clocksource_hz2mult(u32 hz, u32 shift_constant)
{
	/*  hz = cyc/(Billion ns)
	 *  mult/2^shift  = ns/cyc
	 *  mult = ns/cyc * 2^shift
	 *  mult = 1Billion/hz * 2^shift
	 *  mult = 1000000000 * 2^shift / hz
	 *  mult = (1000000000<<shift) / hz
	 */
	u64 tmp = ((u64)1000000000) << shift_constant;

	tmp += hz/2; /* round for do_div */
	do_div(tmp, hz);

	return (u32)tmp;
}

/**
 * clocksource_read: - Access the clocksource's current cycle value
 * @cs:		pointer to clocksource being read
 *
 * Uses the clocksource to return the current cycle_t value
 */
static inline cycle_t clocksource_read(struct clocksource *cs)
{
	return cs->read();
}

/**
 * clocksource_get_cycles: - Access the clocksource's accumulated cycle value
 * @cs:		pointer to clocksource being read
 * @now:	current cycle value
 *
 * Uses the clocksource to return the current cycle_t value.
 * NOTE!!!: This is different from clocksource_read, because it
 * returns the accumulated cycle value! Must hold xtime lock!
 */
static inline cycle_t
clocksource_get_cycles(struct clocksource *cs, cycle_t now)
{
	cycle_t offset = (now - cs->cycle_last) & cs->mask;
	offset += cs->cycle_accumulated;
	return offset;
}

/**
 * clocksource_accumulate: - Accumulates clocksource cycles
 * @cs:		pointer to clocksource being read
 * @now:	current cycle value
 *
 * Used to avoids clocksource hardware overflow by periodically
 * accumulating the current cycle delta. Must hold xtime write lock!
 */
static inline void clocksource_accumulate(struct clocksource *cs, cycle_t now)
{
	cycle_t offset = (now - cs->cycle_last) & cs->mask;
	cs->cycle_last = now;
	cs->cycle_accumulated += offset;
	cs->cycle_monotonic += offset;
}

/**
 * cyc2ns - converts clocksource cycles to nanoseconds
 * @cs:		Pointer to clocksource
 * @cycles:	Cycles
 *
 * Uses the clocksource and ntp ajdustment to convert cycle_ts to nanoseconds.
 * Must hold xtime lock!
 *
 * XXX - This could use some mult_lxl_ll() asm optimization
 */
static inline s64 cyc2ns(struct clocksource *cs, cycle_t cycles)
{
	return ((u64)cycles * cs->mult) >> cs->shift;
}

/**
 * ns2cyc - converts nanoseconds to clocksource cycles
 * @cs:		Pointer to clocksource
 * @nsecs:	Nanoseconds
 */
static inline cycle_t ns2cyc(struct clocksource *cs, u64 nsecs)
{
	cycle_t ret = nsecs << cs->shift;

	do_div(ret, cs->mult + 1);

	return ret;
}

/**
 * clocksource_calculate_interval - Calculates a clocksource interval struct
 *
 * @c:		Pointer to clocksource.
 * @length_nsec: Desired interval length in nanoseconds.
 *
 * Calculates a fixed cycle/nsec interval for a given clocksource/adjustment
 * pair and interval request. Must hold xtime_lock!
 *
 * Unless you're the timekeeping code, you should not be using this!
 */
static inline void clocksource_calculate_interval(struct clocksource *c,
					  	  unsigned long length_nsec)
{
	u64 tmp;

	/* XXX - All of this could use a whole lot of optimization */
	tmp = length_nsec;
	tmp <<= c->shift;
	tmp += c->mult/2;
	do_div(tmp, c->mult);

	c->cycle_interval = (cycle_t)tmp;
	if (c->cycle_interval == 0)
		c->cycle_interval = 1;

	c->xtime_interval = (u64)c->cycle_interval * c->mult;
}


/* used to install a new clocksource */
extern int clocksource_register(struct clocksource*);
extern struct clocksource* clocksource_get_next(void);
extern void clocksource_change_rating(struct clocksource *cs, int rating);
extern void clocksource_resume(void);
extern cycle_t get_monotonic_cycles(void);
extern unsigned long cycles_to_usecs(cycle_t cycles);
extern cycle_t usecs_to_cycles(unsigned long usecs);

/* used to initialize clock */
extern struct clocksource clocksource_jiffies;
extern void clocksource_touch_watchdog(void);

#ifdef CONFIG_GENERIC_TIME_VSYSCALL
extern void update_vsyscall(struct timespec *ts, struct clocksource *c);
extern void update_vsyscall_tz(void);
#else
static inline void update_vsyscall(struct timespec *ts, struct clocksource *c)
{
}

static inline void update_vsyscall_tz(void)
{
}
#endif

#endif /* _LINUX_CLOCKSOURCE_H */

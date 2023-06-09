#ifndef _LINUX_MARKER_H
#define _LINUX_MARKER_H

/*
 * Code markup for dynamic and static tracing.
 *
 * See Documentation/marker.txt.
 *
 * (C) Copyright 2006 Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 * This file is released under the GPLv2.
 * See the file COPYING for more details.
 */

#include <linux/immediate.h>
#include <linux/types.h>

struct module;
struct marker;

/**
 * marker_probe_func - Type of a marker probe function
 * @probe_private: probe private data
 * @call_private: call site private data
 * @fmt: format string
 * @args: variable argument list pointer. Use a pointer to overcome C's
 *        inability to pass this around as a pointer in a portable manner in
 *        the callee otherwise.
 *
 * Type of marker probe functions. They receive the mdata and need to parse the
 * format string to recover the variable argument list.
 */
typedef void marker_probe_func(void *probe_private, void *call_private,
		const char *fmt, va_list *args);

struct marker_probe_closure {
	marker_probe_func *func;	/* Callback */
	void *probe_private;		/* Private probe data */
};

struct marker {
	const char *name;	/* Marker name */
	const char *format;	/* Marker format string, describing the
				 * variable argument list.
				 */
	DEFINE_IMV(char, state);/* Immediate value state. */
	char ptype;		/* probe type : 0 : single, 1 : multi */
	void (*call)(const struct marker *mdata,	/* Probe wrapper */
		void *call_private, const char *fmt, ...);
	struct marker_probe_closure single;
	struct marker_probe_closure *multi;
} __attribute__((aligned(8)));

#ifdef CONFIG_MARKERS

/*
 * Generic marker flavor always available.
 * Note : the empty asm volatile with read constraint is used here instead of a
 * "used" attribute to fix a gcc 4.1.x bug.
 * Make sure the alignment of the structure in the __markers section will
 * not add unwanted padding between the beginning of the section and the
 * structure. Force alignment to the same alignment as the section start.
 */
#define __trace_mark(generic, name, call_private, format, args...)	\
	do {								\
		static const char __mstrtab_##name[]			\
		__attribute__((section("__markers_strings")))		\
		= #name "\0" format;					\
		static struct marker __mark_##name			\
		__attribute__((section("__markers"), aligned(8))) =	\
		{ __mstrtab_##name, &__mstrtab_##name[sizeof(#name)],	\
		0, 0, marker_probe_cb,					\
		{ __mark_empty_function, NULL}, NULL };			\
		__mark_check_format(format, ## args);			\
		if (!generic) {						\
			if (unlikely(imv_read(__mark_##name.state)))	\
				(*__mark_##name.call)			\
					(&__mark_##name, call_private,	\
					format, ## args);		\
		} else {						\
			if (unlikely(_imv_read(__mark_##name.state)))	\
				(*__mark_##name.call)			\
					(&__mark_##name, call_private,	\
					format, ## args);		\
		}							\
	} while (0)

extern void marker_update_probe_range(struct marker *begin,
	struct marker *end);
#else /* !CONFIG_MARKERS */
#define __trace_mark(generic, name, call_private, format, args...) \
		__mark_check_format(format, ## args)
static inline void marker_update_probe_range(struct marker *begin,
	struct marker *end)
{ }
#endif /* CONFIG_MARKERS */

/**
 * trace_mark - Marker using code patching
 * @name: marker name, not quoted.
 * @format: format string
 * @args...: variable argument list
 *
 * Places a marker using optimized code patching technique (imv_read())
 * to be enabled.
 */
#define trace_mark(name, format, args...) \
	__trace_mark(0, name, NULL, format, ## args)

/**
 * _trace_mark - Marker using variable read
 * @name: marker name, not quoted.
 * @format: format string
 * @args...: variable argument list
 *
 * Places a marker using a standard memory read (_imv_read()) to be
 * enabled. Should be used for markers in __init and __exit functions and in
 * lockdep code.
 */
#define _trace_mark(name, format, args...) \
	__trace_mark(1, name, NULL, format, ## args)

/**
 * MARK_NOARGS - Format string for a marker with no argument.
 */
#define MARK_NOARGS " "

/* To be used for string format validity checking with gcc */
static inline void __printf(1, 2) __mark_check_format(const char *fmt, ...)
{
}

extern marker_probe_func __mark_empty_function;

extern void marker_probe_cb(const struct marker *mdata,
	void *call_private, const char *fmt, ...);
extern void marker_probe_cb_noarg(const struct marker *mdata,
	void *call_private, const char *fmt, ...);

/*
 * Connect a probe to a marker.
 * private data pointer must be a valid allocated memory address, or NULL.
 */
extern int marker_probe_register(const char *name, const char *format,
				marker_probe_func *probe, void *probe_private);

/*
 * Returns the private data given to marker_probe_register.
 */
extern int marker_probe_unregister(const char *name,
	marker_probe_func *probe, void *probe_private);
/*
 * Unregister a marker by providing the registered private data.
 */
extern int marker_probe_unregister_private_data(marker_probe_func *probe,
	void *probe_private);

extern void *marker_get_private_data(const char *name, marker_probe_func *probe,
	int num);

struct marker_iter {
	struct module *module;
	struct marker *marker;
};

extern void marker_iter_start(struct marker_iter *iter);
extern void marker_iter_next(struct marker_iter *iter);
extern void marker_iter_stop(struct marker_iter *iter);
extern void marker_iter_reset(struct marker_iter *iter);
extern int marker_get_iter_range(struct marker **marker, struct marker *begin,
	struct marker *end);

#endif

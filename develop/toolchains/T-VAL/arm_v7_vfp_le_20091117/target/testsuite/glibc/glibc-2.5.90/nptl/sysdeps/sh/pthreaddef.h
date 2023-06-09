/* Copyright (C) 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sysdep.h>

/* Default stack size.  */
#define ARCH_STACK_DEFAULT_SIZE	(2 * 1024 * 1024)

/* Default thread priority  */
#define THREAD_DEFAULT_PRIORITY 0

/* Required stack pointer alignment at beginning.  */
#define STACK_ALIGN		8

/* Minimal stack size after allocating thread descriptor and guard size.  */
#define MINIMAL_REST_STACK	2048

/* Alignment requirement for TCB.  */
#define TCB_ALIGNMENT		8


/* Location of current stack frame.  */
#define CURRENT_STACK_FRAME	__builtin_frame_address (0)


/* XXX Until we have a better place keep the definitions here.  */

/* While there is no such syscall.  */
#define __exit_thread_inline(val) \
  while (1) {								      \
    if (__builtin_constant_p (val) && (val) == 0)			      \
      asm volatile ("mov #0,r4; mov %0,r3; trapa #0x11\n\t" SYSCALL_INST_PAD  \
		   :: "i" (__NR_exit));  \
    else								      \
      asm volatile ("mov %1,r4; mov %0,r3; trapa #0x11\n\t" SYSCALL_INST_PAD  \
		    :: "i" (__NR_exit), "r" (val));			      \
  }

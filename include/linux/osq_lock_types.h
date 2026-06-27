/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_OSQ_LOCK_TYPES_H
#define _LINUX_OSQ_LOCK_TYPES_H

#include <linux/types.h>

/*
 * An MCS like lock especially tailored for optimistic spinning for sleeping
 * lock implementations (mutex, rwsem, etc).
 */

struct optimistic_spin_queue {
	/*
	 * Stores an encoded value of the CPU # of the tail node in the queue.
	 * If the queue is empty, then it's set to OSQ_UNLOCKED_VAL.
	 */
	atomic_t tail;
};

#endif

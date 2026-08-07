/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MODULE_INFO_H
#define _LINUX_MODULE_INFO_H

#include <linux/build_bug.h>
#include <linux/stringify.h>
#include <linux/compiler.h>

/*
 * The maximum module name length, including the NUL byte.
 * Chosen so that structs with an unsigned long line up, specifically
 * modversion_info.
 */
#define MODULE_NAME_LEN (64 - sizeof(unsigned long))

#ifdef MODULE
#define __MODULE_INFO_PREFIX /* empty */
#else
#define __MODULE_INFO_PREFIX KBUILD_MODNAME "."
#endif

/* Generic info of form tag = "info" */
#define MODULE_INFO(tag, info)					  \
	static_assert(						  \
		sizeof(info) - 1 == __builtin_strlen(info),	  \
		"MODULE_INFO(" #tag ", ...) contains embedded NUL byte"); \
	static const char __UNIQUE_ID(modinfo)[]			  \
		__used __section(".modinfo") __aligned(1)		  \
		= __MODULE_INFO_PREFIX __stringify(tag) "=" info

#endif /* _LINUX_MODULE_INFO_H */

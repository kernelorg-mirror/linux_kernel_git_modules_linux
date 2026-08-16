/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SYSFS_TYPES_H_
#define _SYSFS_TYPES_H_

#include <linux/lockdep_types.h>
#include <linux/types.h>

struct attribute {
	const char		*name;
	umode_t			mode;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	bool			ignore_lockdep:1;
	struct lock_class_key	*key;
	struct lock_class_key	skey;
#endif
};

struct kobject;
struct file;
struct vm_area_struct;
struct address_space;

struct bin_attribute {
	struct attribute	attr;
	size_t			size;
	void			*private;
	struct address_space *(*f_mapping)(void);
	ssize_t (*read)(struct file *, struct kobject *, const struct bin_attribute *,
			char *, loff_t, size_t);
	ssize_t (*write)(struct file *, struct kobject *, const struct bin_attribute *,
			 char *, loff_t, size_t);
	loff_t (*llseek)(struct file *, struct kobject *, const struct bin_attribute *,
			 loff_t, int);
	int (*mmap)(struct file *, struct kobject *, const struct bin_attribute *attr,
		    struct vm_area_struct *vma);
};

#ifdef CONFIG_CFI
#define __SYSFS_FUNCTION_ALTERNATIVE(MEMBERS...) struct { MEMBERS }
#else
#define __SYSFS_FUNCTION_ALTERNATIVE(MEMBERS...) union { MEMBERS }
#endif

/**
 * struct attribute_group - data structure used to declare an attribute group.
 * @name:	Optional: Attribute group name
 *		If specified, the attribute group will be created in a
 *		new subdirectory with this name. Additionally when a
 *		group is named, @is_visible and @is_bin_visible may
 *		return SYSFS_GROUP_INVISIBLE to control visibility of
 *		the directory itself.
 * @is_visible:	Optional: Function to return permissions associated with an
 *		attribute of the group. Will be called repeatedly for
 *		each non-binary attribute in the group. Only read/write
 *		permissions as well as SYSFS_PREALLOC are accepted. Must
 *		return 0 if an attribute is not visible. The returned
 *		value will replace static permissions defined in struct
 *		attribute. Use SYSFS_GROUP_VISIBLE() when assigning this
 *		callback to specify separate _group_visible() and
 *		_attr_visible() handlers.
 * @is_bin_visible:
 *		Optional: Function to return permissions associated with a
 *		binary attribute of the group. Will be called repeatedly
 *		for each binary attribute in the group. Only read/write
 *		permissions as well as SYSFS_PREALLOC (and the
 *		visibility flags for named groups) are accepted. Must
 *		return 0 if a binary attribute is not visible. The
 *		returned value will replace static permissions defined
 *		in struct bin_attribute. If @is_visible is not set, Use
 *		SYSFS_GROUP_VISIBLE() when assigning this callback to
 *		specify separate _group_visible() and _attr_visible()
 *		handlers.
 * @bin_size:
 *		Optional: Function to return the size of a binary attribute
 *		of the group. Will be called repeatedly for each binary
 *		attribute in the group. Overwrites the size field embedded
 *		inside the attribute itself.
 * @attrs:	Pointer to NULL terminated list of attributes.
 * @bin_attrs:	Pointer to NULL terminated list of binary attributes.
 *		Either attrs or bin_attrs or both must be provided.
 */
struct attribute_group {
	const char		*name;
	__SYSFS_FUNCTION_ALTERNATIVE(
		umode_t			(*is_visible)(struct kobject *,
						      struct attribute *, int);
		umode_t			(*is_visible_const)(struct kobject *,
							    const struct attribute *, int);
	);
	umode_t			(*is_bin_visible)(struct kobject *,
						  const struct bin_attribute *, int);
	size_t			(*bin_size)(struct kobject *,
					    const struct bin_attribute *,
					    int);
	union {
		struct attribute	**attrs;
		const struct attribute	*const *attrs_const;
	};
	const struct bin_attribute	*const *bin_attrs;
};

struct sysfs_ops {
	ssize_t	(*show)(struct kobject *, struct attribute *, char *);
	ssize_t	(*store)(struct kobject *, struct attribute *, const char *, size_t);
};

#endif /* _SYSFS_TYPES_H_ */

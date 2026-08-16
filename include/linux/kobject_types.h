// SPDX-License-Identifier: GPL-2.0
#ifndef _KOBJECT_TYPES_H_
#define _KOBJECT_TYPES_H_

#include <linux/compiler_types.h>
#include <linux/kref_types.h>
#include <linux/spinlock_types.h>
#include <linux/sysfs_types.h>
#include <linux/types.h>
#include <linux/uidgid_types.h>
#include <linux/workqueue_types.h>

#define UEVENT_NUM_ENVP			64	/* number of env pointers */
#define UEVENT_BUFFER_SIZE		2048	/* buffer for the variables */

struct kobject {
	const char		*name;
	struct list_head	entry;
	struct kobject		*parent;
	struct kset		*kset;
	const struct kobj_type	*ktype;
	struct kernfs_node	*sd; /* sysfs directory entry */
	struct kref		kref;

	unsigned int state_initialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
	unsigned int uevent_suppress:1;

#ifdef CONFIG_DEBUG_KOBJECT_RELEASE
	struct delayed_work	release;
#endif
};

struct kobj_type {
	void (*release)(struct kobject *kobj);
	const struct sysfs_ops *sysfs_ops;
	const struct attribute_group **default_groups;
	const struct kobj_ns_type_operations *(*child_ns_type)(const struct kobject *kobj);
	const struct ns_common *(*namespace)(const struct kobject *kobj);
	void (*get_ownership)(const struct kobject *kobj, kuid_t *uid, kgid_t *gid);
};

struct kobj_uevent_env {
	char *argv[3];
	char *envp[UEVENT_NUM_ENVP];
	int envp_idx;
	char buf[UEVENT_BUFFER_SIZE];
	int buflen;
};

struct kset_uevent_ops {
	int (* const filter)(const struct kobject *kobj);
	const char *(* const name)(const struct kobject *kobj);
	int (* const uevent)(const struct kobject *kobj, struct kobj_uevent_env *env);
};

struct kobj_attribute {
	struct attribute attr;
	__SYSFS_FUNCTION_ALTERNATIVE(
		ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
		ssize_t (*show_const)(struct kobject *kobj, const struct kobj_attribute *attr,
				      char *buf);
	);
	__SYSFS_FUNCTION_ALTERNATIVE(
		ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr,
				 const char *buf, size_t count);
		ssize_t (*store_const)(struct kobject *kobj, const struct kobj_attribute *attr,
				       const char *buf, size_t count);
	);
};

/**
 * struct kset - a set of kobjects of a specific type, belonging to a specific subsystem.
 *
 * A kset defines a group of kobjects.  They can be individually
 * different "types" but overall these kobjects all want to be grouped
 * together and operated on in the same manner.  ksets are used to
 * define the attribute callbacks and other common events that happen to
 * a kobject.
 *
 * @list: the list of all kobjects for this kset
 * @list_lock: a lock for iterating over the kobjects
 * @kobj: the embedded kobject for this kset (recursion, isn't it fun...)
 * @uevent_ops: the set of uevent operations for this kset.  These are
 * called whenever a kobject has something happen to it so that the kset
 * can add new environment variables, or filter out the uevents if so
 * desired.
 */
struct kset {
	struct list_head list;
	spinlock_t list_lock;
	struct kobject kobj;
	const struct kset_uevent_ops *uevent_ops;
} __randomize_layout;

#endif /* _KOBJECT_TYPES_H_ */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include "sleepProcess.h"

/* Upper bound for the kernel buffer, avoids huge allocations. */
#define SLEEP_KBUF_MAX	(64 * 1024)

asmlinkage long sys_listSleepProcess(char __user *buf, int size) {
	struct task_struct *proces;
	char *kbuf;
	int kbufsz;
	int len = 0;
	int count = 0;
	int truncated = 0;
	long state;
	long ret;

	if (buf == NULL || size <= 0)
		return -EINVAL;

	kbufsz = min(size, SLEEP_KBUF_MAX);
	kbuf = kmalloc(kbufsz, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	len = scnprintf(kbuf, kbufsz, "%-8s %-16s %s\n", "PID", "NAME", "STATE");

	/* Walk the process list looking for sleeping processes */
	rcu_read_lock();
	for_each_process(proces) {
		state = proces->state;

		/* TASK_RUNNING == 0: process is running or ready to run */
		if (!(state & (TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE)))
			continue;

		/* Keep room for the line and the final '\0' */
		if (len + 64 >= kbufsz) {
			truncated = 1;
			break;
		}

		len += scnprintf(kbuf + len, kbufsz - len, "%-8ld %-16s %s\n",
				(long)task_pid_nr(proces),
				proces->comm,
				(state & TASK_INTERRUPTIBLE) ?
					"TASK_INTERRUPTIBLE" : "TASK_UNINTERRUPTIBLE");
		count++;
	}
	rcu_read_unlock();

	/* User buffer is too small */
	if (truncated) {
		kfree(kbuf);
		return -ENOSPC;
	}

	/* Copy including the final '\0' */
	if (copy_to_user(buf, kbuf, len + 1))
		ret = -EFAULT;
	else
		ret = count;

	kfree(kbuf);
	return ret;
}

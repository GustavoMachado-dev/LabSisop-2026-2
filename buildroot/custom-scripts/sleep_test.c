#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SYSCALL_SLEEPPROCESS	386
#define BUF_SIZE		16384

int main(int argc, char** argv){
	char buf[BUF_SIZE];
	long ret;

	printf("Invoking 'listSleepProcess' system call.\n");

	ret = syscall(SYSCALL_SLEEPPROCESS, buf, sizeof(buf));

	if(ret >= 0) {
		/* Success, show the sleeping processes. */
		printf("%s", buf);
		printf("Total sleeping processes: %ld\n", ret);
	}
	else {
		printf("System call 'listSleepProcess' did not execute as expected error %ld (errno %d: %s)\n", ret, errno, strerror(errno));
	}

	return 0;
}

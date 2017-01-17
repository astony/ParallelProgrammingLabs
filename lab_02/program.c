#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

int main()
{
  printf("Norbert Małecki, pid: %d\n", getpid());
  return 0;
}

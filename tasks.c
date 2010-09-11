#include <tasks.h>

task_1()
{
	unsigned long num=0;
	for(;; num++)
	{
		k_printf("Task 1: %d", num);
	};
};

task_2()
{
	unsigned long num=0;
	for(;; num++)
	{
		k_printf("Task 2: %d", num);
	};
};

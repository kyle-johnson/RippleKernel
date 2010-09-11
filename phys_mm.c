#include <data_types.h>
#include <mutex.h>
#include <k_printf.h>
#include <phys_mm.h>

void *setup_phys_mm_stack(basic_stack_struct *the_basic_stack)
{
	the_basic_stack->base = (u_long *) ((&the_basic_stack) + sizeof(basic_stack_struct));
	the_basic_stack->posistion = 0;
	the_basic_stack->mutex = 0;

	return(the_basic_stack);
};

void push_phys_mm(basic_stack_struct *the_stack, u_long data)
{
	// lock_mutex_block(the_stack->mutex);
	the_stack->posistion++;
	the_stack->base[the_stack->posistion] = data;

	// unlock_mutex(the_stack->mutex);
};

// returns the address if succesful, or 0 if not
u_long pop_phys_mm(basic_stack_struct *the_stack)
{
	if(the_stack->posistion >= 1)
	{
		the_stack->posistion--;
		return(the_stack->base[the_stack->posistion]);
	}
	else	// nothing on the stack
	{
		return(0);
	};
};

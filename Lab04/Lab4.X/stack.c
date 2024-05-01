#include "stack.h"

void StackInit(struct Stack *stack)
{
	stack->currentItemIndex = -1;
	stack->initialized = TRUE;
}

int StackPush(struct Stack *stack, double value)
{	
	if (stack->currentItemIndex == STACK_SIZE-1 || stack->initialized == FALSE)
	{
		return STANDARD_ERROR;
	}
	else
	{
		stack->currentItemIndex += 1;
		stack->stackItems[stack->currentItemIndex] = value;
		return SUCCESS;
	}
}

int StackPop(struct Stack *stack, double *value)
{
	if (stack->currentItemIndex == -1 || stack->initialized == FALSE)
	{
		return STANDARD_ERROR;
	}
	else
	{
		*value = stack->stackItems[stack->currentItemIndex];
		stack->stackItems[stack->currentItemIndex] = 0;
		stack->currentItemIndex -= 1;
		return SUCCESS;
	}
}

int StackIsEmpty(const struct Stack *stack)
{
	if (stack->currentItemIndex == -1 && stack->initialized == TRUE)
	{
		return TRUE;
	}	
	return FALSE;
}

int StackIsFull(const struct Stack *stack)
{
	if (stack->currentItemIndex != STACK_SIZE - 1 || stack->initialized == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

int StackGetSize(const struct Stack *stack)
{
	if (stack->initialized == FALSE)
	{
		return SIZE_ERROR;
	}
	else
	{
		return stack->currentItemIndex + 1;
	}
}
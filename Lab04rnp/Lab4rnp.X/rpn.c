#include "rpn.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int RPN_Evaluate(char * rpn_string, double * result)
{
	struct Stack stack = {};
	StackInit(&stack);
	char *token = strtok(rpn_string, " ");
	while (token != NULL)
	{
		double pop1;
		double pop2;
		int isnum = 1;
		//printf("\n%s",token);
		for(int x = 0; x < strlen(token); x++)
		{
			if(token[x] == '-' && x == (strlen(token) - 1))
			{
				isnum = 0;
				break;
			}
			if (!((token[x] >= '0' && token[x] <= '9') || token[x] == '.' || token[x] == '-'))
			{
				isnum = 0;
				break;
			}
		}
		//printf("%s",token);
		if (isnum == 1)
		{
			
			double n = atof(token);
			//printf("\n%f",n);
			if (StackPush(&stack, n) == STANDARD_ERROR)
			{
				return RPN_ERROR_STACK_OVERFLOW;
			}
		}
		else if (*token == '+' || *token == '-' || *token == '*' || *token == '/') 
		{
			StackPop(&stack, &pop2);
			if (StackIsEmpty(&stack) == TRUE)
			{
				return RPN_ERROR_STACK_UNDERFLOW;
			}
			StackPop(&stack, &pop1);

			//Checking for errors
			if ((pop2 == STANDARD_ERROR && pop2 != 0) || pop1 == STANDARD_ERROR)
			{
				return RPN_ERROR_STACK_UNDERFLOW;
			}
			if (pop2 == 0)
			{
				return RPN_ERROR_DIVIDE_BY_ZERO;
			}
			switch (*token)
			{
				case '+': *result = pop1 + pop2; break;
				case '-': *result = pop1 - pop2; break;
				case '*': *result = pop1 * pop2; break;
				case '/': *result = pop1 / pop2; break;
			}
			StackPush(&stack, *result);
		}
		else
		{
			return RPN_ERROR_INVALID_TOKEN;
		}
		token = strtok(NULL, " ");
	}
	double temp;
	if (StackPop(&stack, &temp) == STANDARD_ERROR)
	{
		return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
	}
	if (!StackIsEmpty(&stack))
	{
		return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
	}
	*result = temp;
	return RPN_NO_ERROR;
}
	


int ProcessBackspaces(char *rpn_sentence)
{
	int x = 0, y = 0;

	while (x < strlen(rpn_sentence))
	{
		if (rpn_sentence[x] == '\b')
		{
			if (y > 0)
			{
				y--;
            	}
		}
		else
		{
			rpn_sentence[y] = rpn_sentence[x];
            	y++;
		}
	x++;
	}

	rpn_sentence[y] = '\0';
	return y;
}
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#define COUNT 9

jmp_buf buf;

typedef struct Stack
{
	char id;
	struct Stack* next;
} Stack;

void read_stacks(FILE* file, Stack** stacks)
{
	char c1, c2, c3, c4;
	int i = 0;
	while (fscanf(file, "%c", &c1) == 1 && c1 != '\n' && fscanf(file, "%c%c%c", &c2, &c3, &c4) == 3)
	{
		if (c1 == '[' && c3 == ']')
		{
			Stack* newnode = (Stack*)malloc(sizeof(Stack));
			if (newnode == NULL)
				longjmp(buf, 1);
			newnode->id = c2;
			newnode->next = NULL;

			Stack** pend = stacks + i;
			while (*pend != NULL)
				pend = &(*pend)->next;
			*pend = newnode;
		}
		i = (i + 1) % COUNT;
	}
}

void read_and_execute_orders(FILE* file, Stack** stacks)
{
	int how_many, from, to;
	while (fscanf(file, " move %d from %d to %d", &how_many, &from, &to) == 3)
	{
		Stack* last = stacks[from - 1];
		int n = how_many - 1;
		while (last->next != NULL && n > 0)
		{
			last = last->next;
			n--;
		}

		Stack* newhead = last->next;
		last->next = stacks[to - 1];
		stacks[to - 1] = stacks[from - 1];
		stacks[from - 1] = newhead;
	}
}

void print_top_view(Stack** stacks)
{
	for (int i = 0; i < COUNT; i++)
		printf("%c", stacks[i]->id);
	printf("\n");
}

void free_stacks(Stack** stacks)
{
	for (int i = 0; i < COUNT; i++)
		free(stacks[i]);
}

int main(void)
{
	FILE* file = fopen("day5.txt", "r");
	if (file == NULL)
		return 1;

	Stack* stacks[COUNT] = { NULL };

	if (setjmp(buf) == 1)
	{
		fclose(file);
		free_stacks(stacks);
		return 2;
	}

	read_stacks(file, stacks);
	print_top_view(stacks);
	read_and_execute_orders(file, stacks);
	print_top_view(stacks);

	fclose(file);
	free_stacks(stacks);

	return 0;
}

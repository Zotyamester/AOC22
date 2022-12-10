#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef enum
{
	NOOP, ADDX, ILLEGAL
} Instruction;

bool fetch(FILE* file, char *instruction, int *p_operand)
{
	if (fscanf(file, "%s", instruction) != 1)
		return false;

	return strcmp(instruction, "noop") == 0 ||
		(strcmp(instruction, "addx") == 0 && fscanf(file, "%d", p_operand) == 1);
}

Instruction decode(const char* instruction)
{
	if (strcmp(instruction, "noop") == 0)
		return NOOP;
	else if (strcmp(instruction, "addx") == 0)
		return ADDX;
	else
		return ILLEGAL;
}

void update_crt(int X, int cycle, bool CRT[][40])
{
	int row = (cycle - 1) / 40;
	int col = (cycle - 1) % 40;

	if (col >= X - 1 && col <= X + 1)
		CRT[row][col] = '#';
}

void execute(Instruction instruction, int operand, int* p_X, int *p_cycle, bool CRT[][40])
{
	update_crt(*p_X, *p_cycle, CRT); /* during the N-th cycle */

	(*p_cycle)++;

	if (instruction == ADDX)
	{
		update_crt(*p_X, *p_cycle, CRT); /* start of the N-th cycle */
		*p_X += operand; /* during the N-th cycle */

		(*p_cycle)++;
	}	
}

int main(void)
{
	FILE* file = fopen("day10.txt", "r");
	if (file == NULL)
		return 1;

	int X = 1, cycle = 1;
	bool CRT[6][40] = { false };

	char instruction[4 + 1];
	int operand;

	while (fetch(file, instruction, &operand))
		execute(decode(instruction), operand, &X, &(cycle), CRT);

	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 40; col++)
			putchar(CRT[row][col] ? '#' : '.');
		putchar('\n');
	}

	fclose(file);

	return 0;
}

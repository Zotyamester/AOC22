#include <stdio.h>
#include <string.h>

#define TOTAL_SIZE 70000000
#define TOTAL_USED 47048086
#define TOTAL_FREE (TOTAL_SIZE - TOTAL_USED)
#define TOTAL_REQUIRED 30000000

#define TO_BE_FREED (TOTAL_REQUIRED - TOTAL_FREE)

/* Takes a file and a pointer to an int as params
 * and returns the size of everything that the
 * directory contains (and could be seen on the
 * console when excuting the commands in the file). */
int dir_size(FILE* file, int* pmin)
{
	char line[127 + 1];
	char program[2 + 1];
	char param[63 + 1];
	
	int size = 0;

	while (fgets(line, 128, file) != NULL)
	{
		if (sscanf(line, "$ %s %s", program, param) == 2)
		{
			if (strcmp(program, "cd") == 0)
			{
				if (strcmp(param, "..") == 0)
					break;
				size += dir_size(file, pmin);
			}
		}
		else /* either ls itself or the result of an ls */
		{
			int file_size;
			if (sscanf(line, "%d %s", &file_size, param) == 2)
				size += file_size;
		}
	}
	if (size >= TO_BE_FREED && (*pmin == -1 || size	< *pmin))
		*pmin = size;

	return size;
}

int main(void)
{
	FILE* file = fopen("day7.txt", "r");
	if (file == NULL)
		return 1;

	int min_to_be_freed = -1;
	dir_size(file, &min_to_be_freed);

	printf("Minimum bytes freed by deleting a directory: %d\n",
			min_to_be_freed);

	fclose(file);

	return 0;
}

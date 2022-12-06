#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int priority(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 1;
	else if (c >= 'A' && c <= 'Z')
		return c - 'A' + 27;
	else
		return 0;
}

char the_one_both_contains(char sacks[][63 + 1], int count)
{
	if (count < 1)
		return '\0';

	int contains[1 + 52] = { 0 };

	for (int i = count - 1; i > 0; i--)
		for (int j = 0; sacks[i][j] != '\0'; j++)
			contains[priority(sacks[i][j])] |= 1 << i;
	for (int j = 0; sacks[0][j] != '\0'; j++)
		if ((contains[priority(sacks[0][j])] | 1) == ((1 << count) - 1))
			return sacks[0][j];

	return '\0';
}

int main(void)
{
	FILE* file = fopen("day3.txt", "r");
	if (file == NULL)
		return 1;

	char buf[3][63 + 1];

	int sum_of_priorities = 0;

	while (fgets(buf[0], sizeof(buf[0]), file) != NULL &&
			fgets(buf[1], sizeof(buf[1]), file) != NULL &&
			fgets(buf[2], sizeof(buf[1]), file) != NULL)
	{
		char c = the_one_both_contains(buf, 3);
		sum_of_priorities += priority(c);
	}

	printf("The sum of all priorities: %d\n", sum_of_priorities);

	fclose(file);

	return 0;
}

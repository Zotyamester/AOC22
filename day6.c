#include <stdio.h>
#include <stdbool.h>

#define TO_DETECT 14

/* It's actually implemented as a  horribly
 * slow n^2 algorithm, but I guess it'
 * suffice until it TO_DETECT becomes very
 * large, but even then there would be an
 * easy fix via saving character frequencies. */
bool distinct_values(const char* chars)
{
	for (int i = 1; i < TO_DETECT; i++)
		for (int j = 0; j < i; j++)
			if (chars[j] == chars[i])
				return false;
	return true;
}

void shift_left(char* chars)
{
	for (int i = 1; i < TO_DETECT; i++)
		chars[i - 1] = chars[i];
}

int get_index_of_marker(FILE* file)
{
	char last_chars[TO_DETECT];

	int c, idx = 0;
	for (int i = 0; i < 4; i++)
	{
		if ((c = fgetc(file)) == EOF)
			return -1;
		last_chars[i] = c;
		idx++;
	}

	while ((c = fgetc(file)) != EOF)
	{
		if (distinct_values(last_chars))
			return idx;
		shift_left(last_chars);
		last_chars[TO_DETECT - 1] = c;
		idx++;
	}

	return -1;
}

int main(void)
{
	FILE* file = fopen("day6.txt", "r");
	if (file == NULL)
		return 1;

	int index = get_index_of_marker(file);
	printf("The position of the marker is %d\n", index);

	fclose(file);

	return 0;
}

#include <stdio.h>

static char wins_against[3] = {
	['A' - 'A'] = 'C',
	['B' - 'A'] = 'A',
	['C' - 'A'] = 'B'
};

static char loses_against[3] = {
	['A' - 'A'] = 'B',
	['B' - 'A'] = 'C',
	['C' - 'A'] = 'A'
};

int indexof(char choice)
{
	return choice - 'A';
}

int ranking(char choice)
{
	return indexof(choice) + 1;
}

int main(void)
{
	FILE* file = fopen("day2.txt", "r");
	if (file == NULL)
		return 1;

	int score = 0;

	char opponents_choice, result;
	while (fscanf(file, " %c %c", &opponents_choice, &result) == 2)
	{
		char my_choice;

		switch (result)
		{
		case 'X':
			my_choice = wins_against[indexof(opponents_choice)];
			/* score += 0 */
			break;
		case 'Y':
			my_choice = opponents_choice;
			score += 3;
			break;
		case 'Z':
			my_choice = loses_against[indexof(opponents_choice)];
			score += 6;
			break;
		}

		score += ranking(my_choice);
	}

	printf("Total score for the strategy is %d\n", score);
	
	fclose(file);
	
	return 0;
}

#include <stdio.h>
#include <stdlib.h>

/* Returns the number that can be read out by only
 * looking at the characters that are representing
 * a digit until the next line. If the line's empty,
 * it returns -1. */
int read_number_in_line(FILE* file)
{
	int num = 0;

	int c = fgetc(file);
	if (c == EOF || c == '\n')
		return -1;
	do
	{
		if (c >= '0' && c <= '9')
			num = 10 * num + (c - '0');
		c = fgetc(file);
	}
	while (c != EOF && c != '\n');

	return num;
}

typedef struct Elf
{
	int sum_of_calories;
	struct Elf* next;
} Elf;

void free_elves(Elf* list)
{
	while (list != NULL)
	{
		Elf* next = list->next;
		free(list);
		list = next;
	}
}

Elf* read_elves(FILE* file)
{
	Elf* list = NULL;

	Elf* eur = NULL; /* Elf Under Read */
	while (!feof(file))
	{
		int num = read_number_in_line(file);
		if (num == -1)
		{
			if (eur == NULL)
				return list;

			Elf** to = &list;
			while (*to != NULL && (*to)->sum_of_calories >= eur->sum_of_calories)
				to = &(*to)->next;
			eur->next = *to;
			*to = eur;

			eur = NULL;
		}
		else
		{
			if (eur == NULL)
			{
				eur = (Elf*)malloc(sizeof(Elf));
				if (eur == NULL)
				{
					free_elves(list);
					return NULL;
				}

				eur->sum_of_calories = 0;
			}

			eur->sum_of_calories += num;
		}
	}

	if (eur != NULL) /* under proper conditions it should never be triggered */
	{
		free_elves(list);
		return NULL;
	}

	return list;
}

/* Returns the sum of calories gathered by the
 * top n elves (the first n in the list),
 * if there are less than n elements in the list,
 * it returns -1. */
int sum_calories(Elf* list, int n)
{
	int sum = 0;

	while (list != NULL && n > 0)
	{
		sum += list->sum_of_calories;
		list = list->next;
		n -= 1;
	}

	if (n > 0)
		return -1;

	return sum;
}

int main(void)
{
	FILE* file = fopen("day1.txt", "r");
	if (file == NULL)
	{
		fprintf(stderr, "Unable to open file.\n");
		return 1;
	}

	Elf* list = read_elves(file);
	if (list == NULL)
	{
		fprintf(stderr, "Memory error or invalid description.\n");
		fclose(file);
		return 2;
	}

	int total = sum_calories(list, 3);
	if (total == -1)
	{
		fprintf(stderr, "Invalid description.\n");
		free_elves(list);
		fclose(file);
		return 3;
	}
	printf("Sum of calories carried by the top 3 elves: %d\n", total);

	free_elves(list);

	fclose(file);

	return 0;
}

#include <stdio.h>
#include <stdbool.h>

typedef struct Interval
{
	int start, end;
} Interval;

#define EMPTY_INTERVAL ((Interval) { .start = -1, .end = -1 })

int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

Interval intersection(Interval a, Interval b)
{
	int max_of_starts = max(a.start, b.start);
	int min_of_ends = min(a.end, b.end);

	if (max_of_starts <= min_of_ends)
		return (Interval) { .start = max_of_starts, .end = min_of_ends };
	return (Interval) { .start = -1, .end = -1 };
}

bool equals(Interval a, Interval b)
{
	return a.start == b.start && a.end == b.end;
}

int main(void)
{
	FILE* file = fopen("day5.txt", "r");
	if (file == NULL)
		return 1;

	int count_of_overlaps = 0;

	Interval a, b;
	while (fscanf(file, "%d-%d,%d-%d", &a.start, &a.end, &b.start, &b.end) == 4)
	{
		Interval c = intersection(a, b);
		if (!equals(c, EMPTY_INTERVAL))
			count_of_overlaps++;
	}

	printf("Total pairs with overlapping intervals: %d\n", count_of_overlaps);

	return 0;
}

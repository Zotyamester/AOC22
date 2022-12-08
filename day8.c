#include <stdio.h>
#include <stdbool.h>

#define N 99 

/* Goes through the trees from start to stop (not including stop)
 * with the given step and returns the distance to stop from the
 * tree that is the closest to it and its height is greater than
 * or equal to stop's height. */
int distance_from_stop(const int* start, const int* stop, int step, int value)
{
	const int* iter, *last = start;
	for (iter = start; iter != stop; iter += step)
		if (*iter >= value)
			last = iter;
	return (stop - last) / step;
}

int highest_scenic(const int map[][N])
{
	int highest = 0;

	for (int i = 1; i < N - 1; i++)
	{
		for (int j = 1; j < N - 1; j++)
		{
			int scenic = distance_from_stop(&map[0][j], &map[i][j], N, map[i][j]) *
			    distance_from_stop(&map[N - 1][j], &map[i][j], -N, map[i][j]) *
			    distance_from_stop(&map[i][0], &map[i][j], 1, map[i][j]) *
			    distance_from_stop(&map[i][N - 1], &map[i][j], -1, map[i][j]);
			if (scenic > highest)
				highest = scenic;
		}
	}

	return highest; 
}

int main(void)
{
	static int map[N][N];

	FILE* file = fopen("day8.txt", "r");
	if (file == NULL)
		return 1;

	char line[N + 1 + 1]; /* N characters (columns) + '\n' + '\0' */
	for (int i = 0; i < N; i++)
	{
		/* Under normal conditions with proper dataset
		 * this branch should never be active. */
		if (fgets(line, sizeof(line), file) == NULL)
			return 2;

		/* ASCII digit to int conversion for every column */
		for (int j = 0; j < N; j++)
			map[i][j] = line[j] - '0';
	}
	

	int scenic = highest_scenic(map);

	printf("The highest scenic score of all trees: %d\n",
			scenic);

	fclose(file);

	return 0;
}

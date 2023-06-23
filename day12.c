#include <stdio.h>
#include <stdbool.h>

#define K 5//41
#define N 8//159

typedef struct Index
{
	int i, j;
} Index;

bool read_map(FILE* file, char map[][N], Index* p_start, Index* p_end)
{
	for (int i = 0; i < K; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int c = fgetc(file);
			if (c == EOF)
				return false;

			map[i][j] = c;

			Index index = { .i = i, .j = j };

			if (c == 'S')
				*p_start = index;
			else if (c == 'E')
				*p_end = index; 
		}

		if (fgetc(file) == EOF)
			return false;
	}

	return true;
}

int min(int a, int b)
{
	return (a < b) ? a : b;
}

Index up(Index pos)
{
	pos.i -= 1;
	return pos;
}

Index down(Index pos)
{
	pos.i += 1;
	return pos;
}

Index left(Index pos)
{
	pos.j -= 1;
	return pos;
}

Index right(Index pos)
{
	pos.j += 1;
	return pos;
}

int find_the_shortest(char map[][N], Index pos, Index end, int route, int shortest)
{
	if (route >= shortest || map[pos.i][pos.j] == '\0')
		return shortest;
	printf("[%d][%d] ==> %c\n", pos.i, pos.j, map[pos.i][pos.j]);
	if (pos.i == end.i && pos.j == end.j)
		return route;

	char c = map[pos.i][pos.j];
	map[pos.i][pos.j] = '\0';

	Index dir;
       
	dir = up(pos);
	if (pos.i > 0 && map[dir.i][dir.j] <= c + 1)
		shortest = min(shortest, find_the_shortest(map, dir, end, route + 1, shortest));
	dir = down(pos);
	if (pos.i < K - 1 && map[dir.i][dir.j] <= c + 1)
		shortest = min(shortest, find_the_shortest(map, dir, end, route + 1, shortest));
	dir = left(pos);
	if (pos.j > 0 && map[dir.i][dir.j] <= c + 1)
		shortest = min(shortest, find_the_shortest(map, dir, end, route + 1, shortest));
	dir = right(pos);
	if (pos.j < N - 1 && map[dir.i][dir.j] <= c + 1)
		shortest = min(shortest, find_the_shortest(map, dir, end, route + 1, shortest));

	map[pos.i][pos.j] = c;
	
	return shortest;
}

int main(void)
{
	FILE* file = fopen("day12_.txt", "r");
	if (file == NULL)
		return 1;

	static char map[K][N];

	Index start, end;
	if (!read_map(file, map, &start, &end))
		return 2;
	map[start.i][start.j] = 'z';
	map[end.i][end.j] = 'a';

	int shortest = find_the_shortest(map, start, end, 0, K * N - 1);

	printf("The shortest path takes %d steps.\n", shortest);

	fclose(file);

	return 0;
}

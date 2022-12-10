#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define KNOT_COUNT 10 
#define N 80

typedef struct
{
	int x, y;
} Point, Vector;

int abs(int x)
{
	return (x >= 0) ? x : -x;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int ch_distance(Point p1, Point p2)
{
	return max(abs(p1.x - p2.x), abs(p1.y - p2.y));
}

typedef struct
{
	Point* points;
	Vector* vectors;
	int size, capacity;
} Array;

void array_init(Array* a)
{
	a->size = 0;
	a->capacity = 1;
	a->points = (Point*)malloc(a->capacity * sizeof(Point));
	a->vectors = (Vector*)malloc(a->capacity * sizeof(Vector));
}

void array_print(Array* a)
{
	for (int i = 0; i < a->size; i++)
		printf("pt: %d;%d vec: %d;%d\n", a->points[i].x - N / 2, a->points[i].y - N / 2,
				a->vectors[i].x, a->vectors[i].y);
}

void array_extend(Array* a, int needed)
{
	if (a->capacity < needed)
	{
		do {
			a->capacity *= 2;
		} while (a->capacity < needed);

		Point* ps = (Point*)malloc(a->capacity * sizeof(Point));
		Vector* vs = (Vector*)malloc(a->capacity * sizeof(Vector));
		for (int i = 0; i < a->size; i++)
		{
			ps[i] = a->points[i];
			vs[i] = a->vectors[i];
		}
		free(a->points);
		free(a->vectors);
		a->points = ps;
		a->vectors = vs;
	}

}

void array_push(Array* a, Point point, Vector vector)
{
	array_extend(a, a->size + 1);
	a->points[a->size] = point;
	a->vectors[a->size] = vector;
	a->size++;
}

void array_free(Array* a)
{
	free(a->points);
	free(a->vectors);
}

Vector movement_to_vector(char dir)
{
	Vector v = { .x = 0, .y = 0 };

	switch (dir)
	{
	case 'U': v.y--; break;
	case 'D': v.y++; break;
	case 'L': v.x--; break;
	case 'R': v.x++; break;
	}

	return v;
}

Point add_vector_to_point(Point p, Vector v)
{
	return (Point){ .x = p.x + v.x, .y = p.y + v.y };
}

void update_visits(bool visited[][N], int* pcount, Point p)
{
	if (!visited[p.y][p.x])
	{
		visited[p.y][p.x] = true;
		(*pcount)++;
	}
}

void array_shift_vectors(Array* a)
{
	for (int i = 1; i < a->size; i++)
		a->vectors[i - 1] = a->vectors[i];
}

int main(void)
{
	FILE* file = fopen("day9_.txt", "r");
	if (file == NULL)
		return 1;

	Array array;
	array_init(&array);
	array_push(&array, (Point){ .x = N / 2, .y = N / 2 }, (Vector){ .x = 0, .y = 0 });
	for (int i = 1; i < KNOT_COUNT; i++)
		array_push(&array, array.points[0], array.vectors[0]);
	
	static bool visited[N][N] = { false };
	visited[N / 2][N / 2] = true;
	int count = 1;

	char dir;
	int offset;
	while (fscanf(file, "%c %d\n", &dir, &offset) == 2)
	{
		for (int i = 0; i < offset; i++)
		{
			Vector mov = movement_to_vector(dir);
			for (int j = array.size - 1; j >= 0; j--)
			{
				Point p_new = add_vector_to_point(array.points[j], mov);
				array.points[j] = p_new;
				if (j == 0)
				{
					update_visits(visited, &count, array.points[j]);
					break;
				}
				printf("knot=%d -> mov=%d;%d\n", j, mov.x, mov.y);
				array_print(&array);


				int dist = ch_distance(array.points[j - 1], array.points[j]);
				if (dist < 2)
				{
					array.vectors[j] = (Vector)add_vector_to_point((Point)array.vectors[j], mov);
					break;
				}

				array_shift_vectors(&array);
				array.vectors[j] = mov;
				mov = array.vectors[j - 1];
			}
		}
	}

	printf("%d\n", count);

	if (count < 0)
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			putchar(visited[i][j] ? '#' : '.');
		putchar('\n');
	}

	array_free(&array);

	fclose(file);

	return 0;
}

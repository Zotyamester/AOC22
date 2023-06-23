#include <stdio.h>
#include <stdbool.h>

int get_token(const char** line)
{
	int num = -1;

	char c;
	while ((c = *(*line)++) != '\0')
	{
		if (c == '\n')
		{
			break;
		}
		else if (c == ',') 
		{
			if (num != -1)
			{
				(*line)--;
				break;
			}
		}
		else if (c == ']')
		{
			if (num == -1)
			{
				return -1;
			}
			else
			{
				(*line)--;
				break;
			}
		}
		else if (c == '[')
		{
			return -2;
		}
		else if (c >= '0' && c <= '9')
		{
			if (num == -1)
			{
				num = c - '0';
			}
			else
			{
				num = num * 10 + c - '0';
			}
		}
	}

	return num;
}

bool is_in_order(const char** line1, const char** line2)
{
	for (;;)
	{
		const char* o1 = *line1, * o2 = *line2;
		int tok1, tok2;
		tok1 = get_token(line1);
		tok2 = get_token(line2);
//		printf("state1: |%s|\nstate2: |%s|\ntok1=%d, tok2=%d\n", *line1, *line2, tok1, tok2);
		if (tok1 == -1)
			return true;
		if (tok2 == -1)
			return false;

		if (tok1 != -2 && tok2 != -2)
		{
			if (tok1 != tok2)
				return tok1 < tok2;
		}
		else
		{
			if (tok1 == -2 && tok2 != -2)
				*line2 = o2;
			else if (tok1 != -2 && tok2 == -2)
				*line1 = o1;

			return is_in_order(line1, line2);
		}
	}
}

int read_and_sum_ordereds(FILE* file)
{
	static char line1[270], line2[270];

	int sum = 0, index = 1;
	while (fgets(line1, sizeof(line1), file) != NULL &&
		fgets(line2, sizeof(line2), file) != NULL && fgetc(file) == '\n')
	{
		const char* p1 = line1, * p2 = line2;
		if (is_in_order(&p1, &p2))
		{
//			printf("WITH INDEX %d\n", index);
			sum += index;
		}
		index++;
	}

	return sum;
}

int main(void)
{
	FILE* file = fopen("day13.txt", "r");
	if (file == NULL)
		return 1;

	int sum = read_and_sum_ordereds(file);

	printf("The sum of ordered pairs' indices: %d\n", sum);

	fclose(file);

	return 0;
}

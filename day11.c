#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MONKEY_COUNT 8 // TODO 
#define ROUNDS 10000

typedef struct Node
{
	long item;
	struct Node* prev, * next;
} Node;

typedef struct Queue
{
	Node* first, * last;
} Queue;

void queue_init(Queue* queue)
{
	queue->first = (Node*)malloc(sizeof(Node));
	queue->last = (Node*)malloc(sizeof(Node));

	queue->first->next = queue->last;
	queue->last->prev = queue->first;

	queue->first->prev = queue->last->next = NULL;

	queue->first->item = queue->last->item = 0xDEADBEEF;
}

void queue_free(Queue* queue)
{
	Node* mov = queue->first;
	while (mov != NULL)
	{
		Node* next = mov->next;
		free(mov);
		mov = next;
	}
}

void queue_print(Queue* queue)
{
	for (Node* mov = queue->first->next; mov != queue->last; mov = mov->next)
		printf("%ld ", mov->item);
	printf("\n");
}

void queue_push(Queue* queue, long item)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->item = item;

	node->prev = queue->last->prev;
	node->next = queue->last;

	node->prev->next = node;
	node->next->prev = node;
}

bool queue_is_empty(Queue* queue)
{
	return queue->first->next == queue->last;
}

long queue_pop(Queue* queue)
{
	Node* head = queue->first->next;
	queue->first->next = head->next;
	head->next->prev = queue->first;
	long item = head->item;
	free(head);
	return item;
}

typedef enum Operation
{
	OLD_OFFSET = 10,
	MUL = '*', ADD = '+',
	MUL_OLD = '*' + OLD_OFFSET, ADD_OLD = '+' + OLD_OFFSET
} Operation; 

typedef struct Monkey
{
	int id;
	Queue items;
	Operation operation;
	int operand, div_check, on_true, on_false;
	long inspections;
} Monkey;

bool monkey_read(Monkey* monkey, FILE* file)
{
	Monkey m;
	if (fscanf(file, " Monkey %d:", &m.id) != 1)
		return false;
	queue_init(&m.items);

	long num;
	if (fscanf(file, " Starting items: %ld", &num) != 1)
		goto monkey_read_error;
	do
	{
		queue_push(&m.items, num);
	} while (fscanf(file, ", %ld", &num) == 1);

	char operation_buf;
	char operand_buf[7 + 1];	
	if (fscanf(file, " Operation: new = old %c %s", &operation_buf, operand_buf) != 2)
		goto monkey_read_error;
	m.operation = operation_buf;
	if (strcmp(operand_buf, "old") == 0)
		m.operation += OLD_OFFSET;
	else if (sscanf(operand_buf, "%d", &m.operand) != 1)
		goto monkey_read_error;

	if (fscanf(file,
		" Test: divisible by %d"
		" If true: throw to monkey %d"
		" If false: throw to monkey %d",
		&m.div_check, &m.on_true, &m.on_false) != 3)
		goto monkey_read_error;

	m.inspections = 0;

	*monkey = m;
	return true;
monkey_read_error:
	queue_free(&m.items);
	return false;
}

void monkey_free(Monkey* monkey)
{
	queue_free(&monkey->items);
}

void simulate_round(Monkey* monkeys, long lcm)
{
	for (int i = 0; i < MONKEY_COUNT; i++)
	{
		while (!queue_is_empty(&monkeys[i].items))
		{
			long level = queue_pop(&monkeys[i].items);
			switch (monkeys[i].operation)
			{
			case MUL:
				level = level * monkeys[i].operand;
				break;
			case ADD:
				level = level + monkeys[i].operand;
				break;
			case MUL_OLD:
				level = level * level;
				break;
			case ADD_OLD:
				level = level + level; 
				break;
			}
	
			monkeys[i].inspections++;
		
			level %= lcm;
			if (level % monkeys[i].div_check == 0)
			{
				queue_push(&monkeys[monkeys[i].on_true].items, level);
			}
			else
			{
				queue_push(&monkeys[monkeys[i].on_false].items, level);
			}
		}
	}
}

void mimax(long* pa, long* pb)
{
	if (*pa > *pb)
	{
		long temp = *pa;
		*pa = *pb;
		*pb = temp;
	}
}

long least_common_multiple(long a, long b)
{
	mimax(&a, &b);

	long lcm = b;
	while (lcm % a != 0)
		lcm += b;

	return lcm;
}

long least_common_multiple_of_monkeys(Monkey* monkeys)
{
	long lcm = 1;

	for (int i = 0; i < MONKEY_COUNT; i++)
		lcm = least_common_multiple(lcm, monkeys[i].div_check);

	return lcm;
}

/* Not quite an optimal solution,
 * but it gets the job done. */
void monkey_sort(Monkey* monkeys)
{
	for (int i = 0; i < MONKEY_COUNT - 1; i++)
	{
		int max = i;
		for (int j = i + 1; j < MONKEY_COUNT; j++)
			if (monkeys[j].inspections > monkeys[max].inspections)
				max = j;
		if (max != i)
		{
			Monkey temp = monkeys[i];
			monkeys[i] = monkeys[max];
			monkeys[max] = temp;
		}
	}
}

int main(void)
{
	FILE* file = fopen("day11.txt", "r");
	if (file == NULL)
		return 1;

	Monkey monkeys[MONKEY_COUNT];
	int index = 0;
	while (index < MONKEY_COUNT && monkey_read(&monkeys[index], file))
		index++;

	if (index < MONKEY_COUNT)
	{
		fprintf(stderr, "Invalid file format or IO error. %d\n", index);
		return 2;
	}

	long lcm = least_common_multiple_of_monkeys(monkeys);

	for (int round = 1; round <= ROUNDS; round++)
		simulate_round(monkeys, lcm);

	monkey_sort(monkeys);
	long business = monkeys[0].inspections * monkeys[1].inspections; 
	printf("The level of monkey business: %ld\n", business);

	return 0;
}

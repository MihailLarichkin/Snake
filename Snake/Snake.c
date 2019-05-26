#include <stdio.h>   
#include "windows.h" 
#include <conio.h>   
#include <time.h>   
#include <stdlib.h> 

#define MAP_SIZE_X 40u
#define MAP_SIZE_Y 20u

#define FOOD_COUNT 5

#define MAP_WALL '#'
#define MAP_EMPTY 0
#define MAP_SNAKE 2
#define MAP_FOOD 1

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_EXIT 27
#define KB_NO -1



typedef struct Snake {
	int pos_x, pos_y;
	char move_direction_x, move_direction_y;
	struct Snake *next_node;
} Snake;



char map[MAP_SIZE_X][MAP_SIZE_Y] = { MAP_EMPTY };
char game_runtime = 1;

char snake_nextmove_x = 0, snake_nextmove_y = 0;
Snake *snake;


void build_map_borders()
{
	size_t i;

	for (i = 0; i < MAP_SIZE_X; ++i)
	{
		map[i][0] = MAP_WALL;
		map[i][MAP_SIZE_Y - 1] = MAP_WALL;
	}

	for (i = 0; i < MAP_SIZE_Y; ++i)
	{
		map[0][i] = MAP_WALL;
		map[MAP_SIZE_X - 1][i] = MAP_WALL;
	}
}

void redraw_screen()
{
	size_t i, j;

	system("cls");

	for (i = 0; i < MAP_SIZE_Y; ++i)
	{
		for (j = 0; j < MAP_SIZE_X; ++j)
			printf("%c", map[j][i]);

		printf("\n");
	}
}

int getKey()
{
	int c;

	if (_kbhit())
	{
		c = _getch();

		if (c == 225)
			c = _getch();

		switch (c)
		{
		case KB_UP:
		case KB_DOWN:
		case KB_LEFT:
		case KB_RIGHT:
		case KB_EXIT:
			return c;
			break;

		default:
			return KB_NO;
		}
	}
	else
	{
		return KB_NO;
	}
}


void spawn_food()
{
	int map_x = rand() % MAP_SIZE_X;
	int map_y = rand() % MAP_SIZE_Y;

	if (map[map_x][map_y] == MAP_EMPTY)
		map[map_x][map_y] = MAP_FOOD;

	else
		spawn_food();
}


void snake_init()
{
	snake = (Snake*)malloc(sizeof(Snake));

	snake->pos_x = 3;
	snake->pos_y = 3;

	snake->move_direction_x = 0;
	snake->move_direction_y = 0;

	snake->next_node = NULL;

	map[snake->pos_x][snake->pos_y] = MAP_SNAKE;
}

void snake_addtail()
{
	Snake *temp = snake;

	while (temp->next_node != NULL)
		temp = temp->next_node;


	temp->next_node = (Snake*)malloc(sizeof(Snake));

	temp->next_node->pos_x = temp->pos_x - temp->move_direction_x;
	temp->next_node->pos_y = temp->pos_y - temp->move_direction_y;

	temp->next_node->move_direction_x = temp->move_direction_x;
	temp->next_node->move_direction_y = temp->move_direction_y;

	temp->next_node->next_node = NULL;
}

void snake_move()
{
	Snake *temp = snake, *previous = snake;
	char old_dir_x, old_dir_y, temp_x, temp_y;

	map[temp->pos_x][temp->pos_y] = MAP_EMPTY;

	switch (map[temp->pos_x + snake_nextmove_x][temp->pos_y + snake_nextmove_y])
	{
	case MAP_SNAKE:
	case MAP_WALL:
		game_runtime = 0;
		return;
		break;

	case MAP_FOOD:
		snake_addtail();
		spawn_food();
		break;
	}

	old_dir_x = temp->move_direction_x;
	old_dir_y = temp->move_direction_y;

	temp->move_direction_x = snake_nextmove_x;
	temp->move_direction_y = snake_nextmove_y;

	temp->pos_x += temp->move_direction_x;
	temp->pos_y += temp->move_direction_y;

	map[temp->pos_x][temp->pos_y] = MAP_SNAKE;

	temp = temp->next_node;


	while (temp != NULL)
	{
		map[temp->pos_x][temp->pos_y] = MAP_EMPTY;

		temp_x = temp->move_direction_x;
		temp_y = temp->move_direction_y;

		temp->move_direction_x = old_dir_x;
		temp->move_direction_y = old_dir_y;

		old_dir_x = temp_x;
		old_dir_y = temp_y;

		temp->pos_x += temp->move_direction_x;
		temp->pos_y += temp->move_direction_y;

		map[temp->pos_x][temp->pos_y] = MAP_SNAKE;

		temp = temp->next_node;
		previous = temp;
	}
}


void main()
{
	srand((unsigned int)time(NULL));

	int key;
	double timer = 0.0;


	build_map_borders();
	snake_init();

	for (size_t i = 0; i < FOOD_COUNT; ++i)
		spawn_food();

	while (game_runtime)
	{
		key = getKey();

		if (key == KB_EXIT)
			break;

		if (key != KB_NO)
			switch (key)
			{
			case KB_UP:
				if (snake_nextmove_y == 0)
				{
					snake_nextmove_x = 0;
					snake_nextmove_y = -1;
				}
				break;

			case KB_DOWN:
				if (snake_nextmove_y == 0)
				{
					snake_nextmove_x = 0;
					snake_nextmove_y = 1;
				}
				break;

			case KB_LEFT:
				if (snake_nextmove_x == 0)
				{
					snake_nextmove_x = -1;
					snake_nextmove_y = 0;
				}
				break;

			case KB_RIGHT:
				if (snake_nextmove_x == 0)
				{
					snake_nextmove_x = 1;
					snake_nextmove_y = 0;
				}
				break;
			}

		Sleep(10);
		timer += 0.01;

		if (timer >= 0.1)
		{
			timer -= 0.1;

			snake_move();
			redraw_screen();
		}
	}

	printf("Game over!");
	_getch();
}
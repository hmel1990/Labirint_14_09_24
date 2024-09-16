#include <iostream>
#include <windows.h> // окрашивание консоли и работа с координатами
#include <conio.h>
using namespace std;

// enumeration - перечисление
enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32 };
enum Colors { DARKGREEN = 2, RED = 12, YELLOW = 14, BLUE = 9 };
// модель локации
enum Objects { HALL, WALL, COIN, ENEMY }; // 0 - коридоры (пустоты), 1 - стена разрушаемая, 2 - монетки,  3 - враги

void print_message(int x, int y, int color, string message);
void print_message_coin(int x, int y, int color, int message);
void hide_cursor();
void location_gen(int location[][50], int WIDTH, int HEIGHT);
void location_show(int location[][50], int WIDTH, int HEIGHT);
void hero_show(COORD position, HANDLE h);
void hero_erase(COORD position, HANDLE h);
COORD key_dir(int location[][50], int code, COORD position);
int hero_coins(int location[][50], COORD position, int coins);
int main();

void print_message(int x, int y, int color, string message) // сообщение которое выводится при победе или сборе монеты
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	setlocale(0, "");
	COORD position_text;
	position_text.X = x;
	position_text.Y = y;
	SetConsoleCursorPosition(h, position_text);
	SetConsoleTextAttribute(h, color);
	cout << message << "\n";
}

void print_message_coin(int x, int y, int color, int message) // сообщение которое выводится при победе
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	setlocale(0, "");
	COORD position_text;
	position_text.X = x;
	position_text.Y = y;
	SetConsoleCursorPosition(h, position_text);
	SetConsoleTextAttribute(h, color);
	cout << message << "\n";
}

void hide_cursor()											// скрыть стандартный мигающий курсор
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);
}

void location_gen(int location[][50], int WIDTH, int HEIGHT)											// генерация локации
{
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			// по дефолту пишется случайное число
			location[y][x] = rand() % 4; // 0 1 2 3

			// стены по краям
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = WALL;

			// вход и выход
			if (x == 0 && y == 2 || x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = HALL;

			if (location[y][x] == ENEMY) {
				// определяется вероятность того, останется враг или нет
				// допустим, вероястность остаться на уровне - 10%
				int prob = rand() % 10; // 0-9
				if (prob != 0) // 1 2 3 4 5 6 7 8 9
					location[y][x] = HALL;
			}
		}
	}
	//return location[HEIGHT][WIDTH];
}

void location_show(int location[][50], int WIDTH, int HEIGHT)											// отображение локации
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++)
		{
			switch (location[y][x]) {
			case HALL: // коридор
				cout << " ";
				break;
			case WALL: // стена
				SetConsoleTextAttribute(h, DARKGREEN); // 0-255
				cout << (char)177; //(char)219;
				break;
			case COIN: // монетки
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)15; // 249
				break;
			case ENEMY: // враги
				SetConsoleTextAttribute(h, RED);
				cout << (char)1;
				break;
			default:
				cout << location[y][x];
				break;
			}
		}
		cout << "\n";
	}
}

void hero_show(COORD position, HANDLE h)										// размещение ГГ (главного героя)
{
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, BLUE);
	cout << (char)1;
}

void hero_erase(COORD position, HANDLE h)										// стирание предыдущей позиции ГГ (главного героя)
{
	SetConsoleCursorPosition(h, position);
	cout << " ";
}

COORD key_dir (int location[][50], int code, COORD position)
{	
	switch (code)
	{
	case RIGHT:
		if (location[position.Y][position.X + 1] != WALL)
			position.X++;
		break;
	case LEFT:
		if (position.X > 0 && location[position.Y][position.X - 1] != WALL)
			position.X--;
		break;
	case UP:
		if (location[position.Y - 1][position.X] != WALL)
			position.Y--;
		break;
	case DOWN:
		if (location[position.Y + 1][position.X] != WALL)
			position.Y++;
		break;
	default:
		cout << code << "\n";
		break;
	}
	return position;
}

int hero_coins(int location[][50], COORD position, int coins)
{
	if (location[position.Y][position.X] == COIN)
	{
		coins++;
		location[position.Y][position.X] = HALL;
		print_message(1, 16, 14, "COIN COLLECTED!\n");
		print_message_coin(1, 17, 14, coins);
	}
	return coins;
}



int main()
{
	const int WIDTH = 50;
	const int HEIGHT = 15;

	int location[HEIGHT][WIDTH] = {};

	COORD position;
	position.X = 1;
	position.Y = 2;

	int coins = 0;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);


	system("title Бомбер!");

	// запуск алгоритма генерации случайных чисел
	srand(time(0));
	rand();

	// скрыть стандартный мигающий курсор
	hide_cursor();

	// генерация локации	
	location_gen(location, WIDTH, HEIGHT);

	// показ локации
	location_show(location, WIDTH, HEIGHT);

	// размещение ГГ (главного героя)

	// функция установки курсора в любую точку на экране консоли
	hero_show (position, h);


	//// игровой движок (интерактив с пользователем)

	while (true)
	{
		int code = _getch(); // функция приостанавливает работу программы, ждёт реакции пользователя
		if (code == 224)
			code = _getch(); // если первый код был 224, то повторный вызов функции вернёт только один, нормальный код

		// стирание ГГ в старой позиции
		hero_erase (position, h);


		// пользователь может нажать любую кнопку (в том числе энтер, эскейп, пробел, стрелочки), после чего вернётся код нажатой клавиши
		position = key_dir(location, code, position);

		// показ ГГ в новой позиции
		hero_show (position, h);


		// взаимодействие ГГ с другими объектами в лабиринте
		coins = hero_coins (location, position, coins);

		
		if (position.Y == HEIGHT - 3 && position.X == WIDTH - 1)
		{
			print_message(1, 18, 12, "победа - найден выход !!!!!!!!!!YOU WIN!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			break;
		}
	}
}
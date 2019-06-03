#include "stdafx.h"
#include "gmap.h"
#include <ctime>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <vector>

bool operator == (point &T, point &U)
{
	return ((T.w == U.w) && (T.h == U.h));
}
bool operator != (point &T, point &U)
{
	return ((T.w != U.w) || (T.h != U.h));
}
std::ostream& operator<<(std::ostream& out, point& x)
{
	out << "(" << x.w_ << ", " << x.h_ << ")";
	return out;
}

gmap::gmap(int _width, int _height)
{
	//начальная инициализация переменных
	width = _width; height = _height; last_dir = 2; dest = point(1, 1);
	//выделение памяти
	map = new int* [height];
	for (int count = 0; count < height; count++)
		map[count] = new int[width];

	//генерация рандомного массива
	//капитальные стенки (границы) по высоте
	for (int i = 0; i < height; i++) {
		map[i][0] = -2;
		map[i][width - 1] = -2;
	}
	//по ширине
	for (int j = 0; j < width; j++) {
		map[0][j] = -2;
		map[height - 1][j] = -2;
	}
	//рандомное заполнение f% поля стенками, остального - пустотами
	const int f = 20; 
	//srand(time(0));
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			if ((rand() % 100) < f) {
				map[i][j] = -2;
			}
			else {
				map[i][j] = -1;
			}

		}
	}

	//рандомная выброска
	int _w, _h;
	srand(time(0));
	do {
		_w = 1 + rand() % (width - 2);
		_h = 1 + rand() % (height - 2);
	} while (is_point(_w, _h, -2) == true);
	set(_w, _h);

	//выбор алгоритма выбора пути
	choice();
}

gmap::~gmap()
{
	for (int count = 0; count < height; count++)
		delete [] map[count];

	if (method == 4) {
		//удаление памяти для массива карты волны
		for (int count = 0; count < height; count++)
			delete[] wave_map[count];
	}
}

void gmap::show() {
	system("cls");

	for (int i = 0; i < height; i++) //переключение по строкам
	{
		for (int j = 0; j < width; j++)// переключение по столбцам
		{
			if (pos == point(j, i) && dest == point(j, i)) {//герой стоит на цели
				std::cout << "XX";
			}
			else {
				if (pos == point(j, i)) {//положение героя
					std::cout << "<>";
					//пока игнорится содежимое клетки, на которой стоит герой, допиши потом!
				}
				else {//все, кроме положения героя
					if (dest == point(j, i))
					{	//цель
						std::cout << "><";
					}
					else {
						if (map[i][j] < 0) {// все неразведанное
							std::cout << "··";
						}
						else { //все оставшееся разведанное
							if (map[i][j] == 2)
							{   //стенки
								std::cout << "##";
							}

							if (map[i][j] == 1)
							{   //пустоты
								std::cout << "  ";
							}

						}
					}
				}
			}
			
		}
		std::cout << std::endl;
	}
	Sleep(2000);
}

void gmap::show_ignored() {
	system("cls");

	for (int i = 0; i < height; i++) //переключение по строкам
	{
		for (int j = 0; j < width; j++)// переключение по столбцам
		{
			if (pos == point(j, i) && dest == point(j, i)) {//герой стоит на цели
				std::cout << "XX";
			}
			else {
				if (pos == point(j, i)) {//само положение героя
					std::cout << "<>";
				}
				else {//все, кроме положения героя
					if (dest == point(j, i))
					{	//цель
						std::cout << "><";
					}
					else {
						if (map[i][j] == 2 || map[i][j] == -2)
						{   //стенки
							std::cout << "##";
						}

						if (map[i][j] == 1 || map[i][j] == -1)
						{   //пустоты
							std::cout << "  ";
						}

					}
				}
			}
		}
		std::cout << std::endl;
	}
	system("PAUSE");
}

void gmap::set(int _w, int _h){
	//разведка соседней территории
	if (_w > 0 && _w < width && _h > 0 && _h < height) { //проверка на выход за границы
		for (int i = _h - 1; i <= _h + 1; i++) {
			for (int j = _w - 1; j <= _w + 1; j++) {
				map[i][j] = abs(map[i][j]);
			}
		}
		pos = point(_w, _h);
	}
}

bool gmap::is_point(int _w, int _h, int value) {
	if (map[_h][_w] == value) { 
		return true; 
	}
	else {
		return false;
	}
}

void gmap::print() {
	system("cls"); 
	std::cout.setf(std::ios::showpos);
	for (int i = 0; i < height; i++) //переключение по строкам
	{
		for (int j = 0; j < width; j++)// переключение по столбцам
		{
			if (pos == point(j, i) && dest == point(j, i)) {//герой стоит на цели
				std::cout << "XX";
			}
			else {
				if (pos == point(j, i)) {//само положение героя
					std::cout << "<>";
				}
				else {//все, кроме положения героя
					if (dest == point(j, i)) {//положение цели
						std::cout << "><";
					}
					else {
						std::cout << map[i][j];
					}
				}
			}
		}
		std::cout << std::endl;
	}
	system("PAUSE");
}

void gmap::wave_print() {
	system("cls");
	for (int i = 0; i < height; i++) //переключение по строкам
	{
		for (int j = 0; j < width; j++)// переключение по столбцам
		{
			if (is_point(j, i, -2) == true || is_point(j, i, 2) == true) { //печатаем стенки для наглядности
				std::cout << "##";
			}
			else {
				if (dest == point(j, i)) { //печатаем цель
					std::cout << "><";
				}
				else {
					if (wave_map[i][j] > -1 && wave_map[i][j] < 10) {
						std::cout << " " << wave_map[i][j];
					}
					else {
						if (wave_map[i][j] == -9) {
							std::cout << "  ";
						}
						else {
							std::cout << wave_map[i][j];
						}
					}
				}
			}
		}
		std::cout << std::endl;
	}
	//system("PAUSE");
	Sleep(2000);
}

bool gmap::is_wall(char* dest) {
	if (dest == "front") {
		if ((last_dir == 2) && (is_point(pos.w, pos.h - 1, 2) == true)) {
			return true;
		}
		if ((last_dir == 4) && (is_point(pos.w - 1, pos.h, 2) == true)) {
			return true;
		}
		if ((last_dir == 6) && (is_point(pos.w + 1, pos.h, 2) == true)) {
			return true;
		}
		if ((last_dir == 8) && (is_point(pos.w, pos.h + 1, 2) == true)) {
			return true;
		}
		return false;
	}
	if (dest == "left") {
		if ((last_dir == 2) && (is_point(pos.w - 1, pos.h, 2) == true)) {
			return true;
		}
		if ((last_dir == 4) && (is_point(pos.w, pos.h + 1, 2) == true)) {
			return true;
		}
		if ((last_dir == 6) && (is_point(pos.w, pos.h - 1, 2) == true)) {
			return true;
		}
		if ((last_dir == 8) && (is_point(pos.w + 1, pos.h, 2) == true)) {
			return true;
		}
		return false;
	}
	if (dest == "right") {
		if ((last_dir == 2) && (is_point(pos.w + 1, pos.h, 2) == true)) {
			return true;
		}
		if ((last_dir == 4) && (is_point(pos.w, pos.h - 1, 2) == true)) {
			return true;
		}
		if ((last_dir == 6) && (is_point(pos.w, pos.h + 1, 2) == true)) {
			return true;
		}
		if ((last_dir == 8) && (is_point(pos.w - 1, pos.h, 2) == true)) {
			return true;
		}
		return false;
	}
	if (dest == "behind") {
		if ((last_dir == 2) && (is_point(pos.w, pos.h + 1, 2) == true)) {
			return true;
		}
		if ((last_dir == 4) && (is_point(pos.w + 1, pos.h, 2) == true)) {
			return true;
		}
		if ((last_dir == 6) && (is_point(pos.w - 1, pos.h, 2) == true)) {
			return true;
		}
		if ((last_dir == 8) && (is_point(pos.w, pos.h - 1, 2) == true)) {
			return true;
		}
		return false;
	}
	return false;
}

int gmap::set_dest_at(char* dest) {
	if (dest == "front") {
		return last_dir;
	}
	if (dest == "left") {
		switch (last_dir) {
		case 2: return 4; break;
		case 4: return 8; break;
		case 6: return 2; break;
		case 8: return 6; break;
		}
	}
	if (dest == "right") {
		switch (last_dir) {
		case 2: return 6; break;
		case 4: return 2; break;
		case 6: return 8; break;
		case 8: return 4; break;
		}
	}
	if (dest == "behind") {
		switch (last_dir) {
		case 2: return 8; break;
		case 4: return 6; break;
		case 6: return 4; break;
		case 8: return 2; break;
		}
	}
	return 5;
}

void gmap::move(int direction) {
	switch (direction) {
	case 2: {
		if (is_point(pos.w, pos.h - 1, 2) == false) {
			set(pos.w, pos.h - 1);
		}
		break;
	}
	case 4: {
		if (is_point(pos.w - 1, pos.h, 2) == false) {
			set(pos.w - 1, pos.h);
		}
		break;
	}
	case 6: {
		if (is_point(pos.w + 1, pos.h, 2) == false) {
			set(pos.w + 1, pos.h);
		}
		break;
	}
	case 8: {
		if (is_point(pos.w, pos.h + 1, 2) == false) {
			set(pos.w, pos.h + 1);
		}
		break;
	}

	case 5: {
		set(pos.w, pos.h);
		break;
	}
	}
	last_dir = direction;
}

void gmap::met1() {//простейший рандомное хождение прямо
	int dir = 5;
	
	if (is_wall("front") == false) {
		dir = last_dir;
	}
	else {
		if (rand() % 2 == 1) {
			if (is_wall("left") == false) {
				dir = set_dest_at("left");
			}
			else {
				if (is_wall("right") == false) {
					dir = set_dest_at("right");
				}
				else {
					if (is_wall("behind") == false) {
						dir = set_dest_at("behind");
					}
					else {
						dir = 5;
					}
				}
			}
		}
		else {
			if (is_wall("right") == false) {
				dir = set_dest_at("right");
			}
			else {
				if (is_wall("left") == false) {
					dir = set_dest_at("left");
				}
				else {
					if (is_wall("behind") == false) {
						dir = set_dest_at("behind");
					}
					else {
						dir = 5;
					}
				}
			}
		}
	
	}
	
	move(dir);
}

void gmap::met2() {//правая рука
	int dir = 5;
	if (is_wall("right") == false) {
		dir = set_dest_at("right");
	}
	else {
		if (is_wall("front") == false) {
			dir = set_dest_at("front");
		}
		else {
			if (is_wall("left") == false) {
				dir = set_dest_at("left");
			}
			else {
				if (is_wall("behind") == false) {
					dir = set_dest_at("behind");
				}
				else {
					dir = 5;
				}
			}
		}
	}
	move(dir);
}

void gmap::met3() {//левая рука
	int dir = 5;
	if (is_wall("left") == false) {
		dir = set_dest_at("left");
	}
	else {
		if (is_wall("front") == false) {
			dir = set_dest_at("front");
		}
		else {
			if (is_wall("right") == false) {
				dir = set_dest_at("right");
			}
			else {
				if (is_wall("behind") == false) {
					dir = set_dest_at("behind");
				}
				else {
					dir = 5;
				}
			}
		}
	}
	move(dir);
}

void gmap::met4() {//волновой

	//выделение памяти для пункта 4
	wave_map = new int*[height];
	for (int count = 0; count < height; count++)
		wave_map[count] = new int[width];

	//заполение всего массива волны "-9"
	for (int i = 0; i < height ; i++) {
		for (int j = 0; j < width; j++) {
			wave_map[i][j] = -9;
		}
	}
	//стартовая ячейка
	wave_map[pos.h][pos.w] = 0;
	int d = -1; //метка волны

	//распространение волны
	int anyting_changing; bool is_point_eql_dest = false;
	do {
		anyting_changing = false;
		d++;
		for (int i = 0; i < height; i++) //переключение по строкам
		{
			for (int j = 0; j < width; j++)// переключение по столбцам
			{
				
				if (wave_map[i][j] == d) {
					if (dest == point(j, i)) {
						is_point_eql_dest = true;
					}
					if (wave_map[i][j - 1] == -9 && (is_point(j - 1, i, -1) == true || is_point(j - 1, i, 1) == true)) {
						wave_map[i][j - 1] = d + 1;
					}
					if (wave_map[i - 1][j] == -9 && (is_point(j, i - 1, -1) == true || is_point(j, i - 1, 1) == true)) {
						wave_map[i - 1][j] = d + 1;
					}
					if (wave_map[i][j + 1] == -9 && (is_point(j + 1, i, -1) == true || is_point(j + 1, i, 1) == true)) {
						wave_map[i][j + 1] = d + 1;
					}
					if (wave_map[i + 1][j] == -9 && (is_point(j, i + 1, -1) == true || is_point(j, i + 1, 1) == true)) {
						wave_map[i + 1][j] = d + 1;
					}
					anyting_changing = true;
				}
			}
		}
		
	} while (anyting_changing == true && is_point_eql_dest == false);

	wave_print();
	route.clear();
	//восстановление пути
	if (is_point_eql_dest == true) {
		point j(dest.w, dest.h);
		for (int i = 0; d != 0; i++) {
			route.insert(route.begin(), j);
			if (wave_map[j.h - 1][j.w] == (d - 1)) {
				point a(j.w, j.h - 1);
				j = a;
				d--;
			} 
			else 
			{
				if (wave_map[j.h + 1][j.w] == (d - 1)) {
					point a(j.w, j.h + 1);
					j = a;
					d--;
				}
				else {
					if (wave_map[j.h][j.w - 1] == (d - 1)) {
						point a(j.w - 1, j.h);
						j = a;
						d--;
					}
					else {
						if (wave_map[j.h][j.w + 1] == (d - 1)) {
							point a(j.w + 1, j.h);
							j = a;
							d--;
						}
					}
					
				}
				
			
			}
			
		}
		//std::cout << "Путь найден" << std::endl;
		//print_route();
	}
	else {
		wave_print();
		std::cout << "Путь не найден, печалька" << std::endl;
		system("PAUSE");
	}

	//рисование на карте волны лишь найденного пути
	//обнуляем карту волны
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			bool point_in_a_route = false;
			for (int k = 0; k < route.size(); k++) {
				if (route[k] == point(j, i)) {
					point_in_a_route = true;
				}
			}
			if (point_in_a_route == false) {
				wave_map[i][j] = -9;
			}
		}
	}
	wave_print();
}

void gmap::calc() {
	switch (method) {

	case 1: {
		met1(); 
		break; 
	}
	case 2: {
		met2();
		break;
	}
	case 3: {
		met3();
		break;
	}
	case 4: {
		met4();
		go_to_route();
		break;
	}


	}
}

void gmap::choice() {
	//выбор алгоритма выбора пути
	system("cls");
	method = -1;
	while (method < 0 || method > 5) {
		std::cout << "Выберите алгоритм:" << std::endl << "1 - прямой рандомный" << std::endl << "2 - правой руки" << std::endl;
		std::cout << "3 - левой руки" << std::endl << "4 - алгоритм Ли (волновой)" << std::endl;
		std::cout << "Ваш выбор: ";
		std::cin >> method;
		std::cin.clear(); // сброс флагов, при неправильном вводе 
		std::cin.sync(); // очистка буфера
	}
}

void gmap::print_route() {
	for (int i = 0; i < route.size(); i++) {
		std::cout << i << " " << route[i] << std::endl;
	}
	system("PAUSE");
}

void gmap::go_to_route() {
	if (route.empty() == false) {
		for (int i = 0; i < route.size(); i++) {
			set(route[i].w, route[i].h);
			//show();
		}
	}
}

void gmap::random_dest() {
	int _w, _h;
	srand(time(0));
	do {
		_w = 1 + rand() % (width - 2);
		_h = 1 + rand() % (height - 2);
	} while (is_point(_w, _h, -2) == true);
	dest = point(_w, _h);
}
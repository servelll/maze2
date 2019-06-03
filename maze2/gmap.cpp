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
	//��������� ������������� ����������
	width = _width; height = _height; last_dir = 2; dest = point(1, 1);
	//��������� ������
	map = new int* [height];
	for (int count = 0; count < height; count++)
		map[count] = new int[width];

	//��������� ���������� �������
	//����������� ������ (�������) �� ������
	for (int i = 0; i < height; i++) {
		map[i][0] = -2;
		map[i][width - 1] = -2;
	}
	//�� ������
	for (int j = 0; j < width; j++) {
		map[0][j] = -2;
		map[height - 1][j] = -2;
	}
	//��������� ���������� f% ���� ��������, ���������� - ���������
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

	//��������� ��������
	int _w, _h;
	srand(time(0));
	do {
		_w = 1 + rand() % (width - 2);
		_h = 1 + rand() % (height - 2);
	} while (is_point(_w, _h, -2) == true);
	set(_w, _h);

	//����� ��������� ������ ����
	choice();
}

gmap::~gmap()
{
	for (int count = 0; count < height; count++)
		delete [] map[count];

	if (method == 4) {
		//�������� ������ ��� ������� ����� �����
		for (int count = 0; count < height; count++)
			delete[] wave_map[count];
	}
}

void gmap::show() {
	system("cls");

	for (int i = 0; i < height; i++) //������������ �� �������
	{
		for (int j = 0; j < width; j++)// ������������ �� ��������
		{
			if (pos == point(j, i) && dest == point(j, i)) {//����� ����� �� ����
				std::cout << "XX";
			}
			else {
				if (pos == point(j, i)) {//��������� �����
					std::cout << "<>";
					//���� ��������� ��������� ������, �� ������� ����� �����, ������ �����!
				}
				else {//���, ����� ��������� �����
					if (dest == point(j, i))
					{	//����
						std::cout << "><";
					}
					else {
						if (map[i][j] < 0) {// ��� �������������
							std::cout << "��";
						}
						else { //��� ���������� �����������
							if (map[i][j] == 2)
							{   //������
								std::cout << "##";
							}

							if (map[i][j] == 1)
							{   //�������
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

	for (int i = 0; i < height; i++) //������������ �� �������
	{
		for (int j = 0; j < width; j++)// ������������ �� ��������
		{
			if (pos == point(j, i) && dest == point(j, i)) {//����� ����� �� ����
				std::cout << "XX";
			}
			else {
				if (pos == point(j, i)) {//���� ��������� �����
					std::cout << "<>";
				}
				else {//���, ����� ��������� �����
					if (dest == point(j, i))
					{	//����
						std::cout << "><";
					}
					else {
						if (map[i][j] == 2 || map[i][j] == -2)
						{   //������
							std::cout << "##";
						}

						if (map[i][j] == 1 || map[i][j] == -1)
						{   //�������
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
	//�������� �������� ����������
	if (_w > 0 && _w < width && _h > 0 && _h < height) { //�������� �� ����� �� �������
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
	for (int i = 0; i < height; i++) //������������ �� �������
	{
		for (int j = 0; j < width; j++)// ������������ �� ��������
		{
			if (pos == point(j, i) && dest == point(j, i)) {//����� ����� �� ����
				std::cout << "XX";
			}
			else {
				if (pos == point(j, i)) {//���� ��������� �����
					std::cout << "<>";
				}
				else {//���, ����� ��������� �����
					if (dest == point(j, i)) {//��������� ����
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
	for (int i = 0; i < height; i++) //������������ �� �������
	{
		for (int j = 0; j < width; j++)// ������������ �� ��������
		{
			if (is_point(j, i, -2) == true || is_point(j, i, 2) == true) { //�������� ������ ��� �����������
				std::cout << "##";
			}
			else {
				if (dest == point(j, i)) { //�������� ����
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

void gmap::met1() {//���������� ��������� �������� �����
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

void gmap::met2() {//������ ����
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

void gmap::met3() {//����� ����
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

void gmap::met4() {//��������

	//��������� ������ ��� ������ 4
	wave_map = new int*[height];
	for (int count = 0; count < height; count++)
		wave_map[count] = new int[width];

	//��������� ����� ������� ����� "-9"
	for (int i = 0; i < height ; i++) {
		for (int j = 0; j < width; j++) {
			wave_map[i][j] = -9;
		}
	}
	//��������� ������
	wave_map[pos.h][pos.w] = 0;
	int d = -1; //����� �����

	//��������������� �����
	int anyting_changing; bool is_point_eql_dest = false;
	do {
		anyting_changing = false;
		d++;
		for (int i = 0; i < height; i++) //������������ �� �������
		{
			for (int j = 0; j < width; j++)// ������������ �� ��������
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
	//�������������� ����
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
		//std::cout << "���� ������" << std::endl;
		//print_route();
	}
	else {
		wave_print();
		std::cout << "���� �� ������, ��������" << std::endl;
		system("PAUSE");
	}

	//��������� �� ����� ����� ���� ���������� ����
	//�������� ����� �����
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
	//����� ��������� ������ ����
	system("cls");
	method = -1;
	while (method < 0 || method > 5) {
		std::cout << "�������� ��������:" << std::endl << "1 - ������ ���������" << std::endl << "2 - ������ ����" << std::endl;
		std::cout << "3 - ����� ����" << std::endl << "4 - �������� �� (��������)" << std::endl;
		std::cout << "��� �����: ";
		std::cin >> method;
		std::cin.clear(); // ����� ������, ��� ������������ ����� 
		std::cin.sync(); // ������� ������
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
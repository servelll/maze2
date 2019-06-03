#pragma once
#include <vector>
using namespace std;
class point {
	int w_; //��������� ��� ���������� << (�������)
	int h_;
public:
	int w;
	int h;
	point(int _w, int _h) {
		w = _w; h = _h;
		w_ = _w; h_ = _h;
	};
	point() {
		w = 0; h = 0;
		w = 0; h = 0;
	};
	point& operator= (const point& right) {
		w = right.w; h = right.h;
		w_ = right.w; h_ = right.h;
		return *this;
	}
	friend ostream& operator<<(ostream&, point&);
	friend bool operator ==(point &T, point &U);
	friend bool operator !=(point &T, point &U);
	~point() {};
};

class gmap
{
	int **map; //������ �����, ������������� - �����������, 1 - �����, 2 - ������, 9 - ����
	int **wave_map;
	long int moves; //���������� ����� �� ����
	std::vector<point> route; //������� �������� � ���� � ������
	point pos; //������� �������
	point dest; //������� ������� ����
	int width, height; //������, ������
	int last_dir; //��������� ����������� ��������
	int method; //����� 
public:
	gmap(int _width, int _height);
	~gmap();
	void show();
	void show_ignored();
	void print();
	void set(int _w, int _h);
	bool is_point(int _w, int _h, int value);
	void calc();
	void choice();
	void random_dest();

	//������ ��� ���������� ����������: ���������� � �\� ���
	bool is_wall(char* dest);
	int set_dest_at(char* dest);
	void move(int direction);
	void met1();
	void met2(); //������
	void met3(); //�����
	void met4(); //��������
	void wave_print();
	void print_route();
	void go_to_route();
};

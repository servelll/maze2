#pragma once
#include <vector>
using namespace std;
class point {
	int w_; //приватные для перегрузки << (костыль)
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
	int **map; //массив карты, положительное - разведанное, 1 - пусто, 2 - стенка, 9 - цель
	int **wave_map;
	long int moves; //количество ходов за игру
	std::vector<point> route; //маршрут движения к цели в точках
	point pos; //текущая позиция
	point dest; //позиция текущей цели
	int width, height; //ширина, высота
	int last_dir; //последнее направление движения
	int method; //метод 
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

	//методы для простейших алгоритмов: рандомного и л\п рук
	bool is_wall(char* dest);
	int set_dest_at(char* dest);
	void move(int direction);
	void met1();
	void met2(); //правая
	void met3(); //левая
	void met4(); //волновой
	void wave_print();
	void print_route();
	void go_to_route();
};

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include "gmap.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	int w = 39, h = 22; //дефолтные границы
	gmap m(w, h);
	m.random_dest();
	m.show_ignored();

	do {
		m.calc();
		m.show();
		m.random_dest();
	} while (true);

	system("PAUSE");
	return 0;
}


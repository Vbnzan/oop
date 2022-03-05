#define PI 3.14159265      
#include "Moving.h"


void test_moving() {
	vector<double> a(4); //создали точку
	a(0) = 1;
	a(1) = 2;
	a(2) = 3;
	a(3) = 4;

	vector<double> b(4); //создали вектор
	b.clear();
	b(0) = 1;

	vector<double> с(4); //создали еще одну точку
	с(0) = 2;
	с(1) = 0;
	с(2) = 1;
	с(3) = 1;


	struct Line* line1 = new struct Line; //cоздали линию
	line1->direction = b;
	line1->point = с;

	vector<double> d(7); //создали непонятно что

	double angle = PI / 2;

	Moving t0; //создали тождевственное преобразование
	std::cout << "t0(a):" << std::endl;
	print_vect_4(t0(a)); //тождевственное преобразование не меняет точку

	Moving t1(b); //сдвиг на вектор b
	std::cout << "t1(a):" << std::endl;
	print_vect_4(t1(a));  //сдвиг сдвигает точку

	std::cout << "t1(b):" << std::endl; //сдвиг не сдвигает вектор 
	print_vect_4(t1(b));

	std::cout << "t1(line1):" << std::endl; //сдвиг не меняет направления, но сдвигает точку
	Line* line2 = t1(line1);
	std::cout << "line direction" << std::endl;
	print_vect_4(line2->direction);
	std::cout << "line point" << std::endl;
	print_vect_4(line2->point);


	Moving t2(a); //пытаемся сделать сдвиг на точку - программа ругается
	Moving wr(d); //пытаемся сделать сдвиг на непонятно что - программа ругается


	double angle1 = PI / 2;
	Moving t3(b, angle1); //поворот на pi/2 вокруг b
	std::cout << "t3(a):" << std::endl;
	print_vect_4(t3(a));

	std::cout << "t3(line1):" << std::endl;
	Line* line3 = t3(line1);
	std::cout << "line direction" << std::endl;
	print_vect_4(line3->direction);
	std::cout << "line point" << std::endl;
	print_vect_4(line3->point);
	//все хорошо поворачивается



	double angle2 = PI / 3;
	matrix<double>* m1 = new matrix<double>(3, 3);
	m1->clear();
	(*m1)(0, 0) = cos(angle2);
	(*m1)(0, 1) = -sin(angle2);
	(*m1)(1, 1) = cos(angle2);
	(*m1)(1, 0) = sin(angle2);
	(*m1)(2, 2) = 1.; //создали ортогональную матрицу
	std::cout << "is m1 orthogonal:  " << is_orthogonal(m1, 3, 0.0001) << std::endl;

	matrix<double>* m2 = new matrix<double>(3, 3);
	m2->clear();
	(*m2)(0, 0) = 2;
	(*m2)(0, 1) = -3;
	(*m2)(1, 1) = 5;
	(*m2)(1, 0) = sin(angle2); //создали не ортогональную матрицу
	std::cout << "is m2 orthogonal:  " << is_orthogonal(m2, 3, 0.0001) << std::endl;

	Moving t4(m1, &b); //создали преобразование с ортогональной матрицей
	std::cout << "t4(a):" << std::endl;
	print_vect_4(t4(a));
	std::cout << "t4(line1):" << std::endl;
	Line* line4 = t4(line1);
	std::cout << "line direction" << std::endl;
	print_vect_4(line4->direction);
	std::cout << "line point" << std::endl;
	print_vect_4(line4->point);

	Moving t5(m2, &b); //попытались создать преобразование с ортогональной матрицей - программа ругается
	Moving t6(m1, &a); //попытались создать преобразование со сдвигом на точку - программа ругается
	Moving t7(m1, &d); //попытались создать преобразование со сдвигом на непонятно что - программа ругается

	std::cout << " inverse transformation of t4(a):" << std::endl; //обратное преобразование превращает t4(a) обратно в a
	print_vect_4(t4.inverse_transformation(t4(a)));
	
	//t4.Print_self();
}


int main() {
	test_moving();

	return 0;
}
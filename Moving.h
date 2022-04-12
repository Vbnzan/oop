#pragma once
#include <iostream>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <cmath>
#include <map>
#include<list>
using std::list;
using std::string;
using std::map;
using boost::numeric::ublas::matrix;
using boost::numeric::ublas::vector;

bool is_orthogonal(matrix<double> &matr, int size, double accurasy); //проверяет ортогональность матрицы
void print_vect_4(vector<double> obj);
double angle_between_vectors(vector<double>& vec1, vector<double>& vec2);


class object {
public:
	virtual void print_self() const { printf("Something is wrong\n"); };
};

class simple_object : public object, public vector<double> { //точка или вектор
public:
	virtual void print_self() const;
	simple_object() {};
	simple_object(vector<double> a);
};

class Line : public object { //прямая в пространстве задается точкой на ней и направлением
public:
	simple_object point;
	simple_object direction;
	void print_self() const;
};





class Transformation { //какое-то преобразование
protected:
	string id;
public:
	string get_id() const{
		return id;
	}
	virtual object* operator() (object& arg) 
	{ 
		printf("something is wrong\n");
		return &arg;
	}; //не должна исполняться, внутренная функция - заглушка

};


class Moving : public Transformation{ //преобразование движения
protected:
	matrix<double> turning_matrix;
	vector<double> moving_vect;
public:
	Moving();  //тождевственное преобразование
	Moving(vector<double> &vect, string id); //сдвиг на вектор
	Moving(vector<double> &direction, double rotation_angle, string id); //поворот на rotation_angle вокруг оси direction
	Moving(matrix<double> &turning_matrix, vector<double> &moving_vect, string id); //создать преобразование по заданной матрице и векторе смкещения	
	simple_object* operator() (simple_object &obj); //применение преобразования
	Line* operator() (Line  &obj); //применение преобразования
	void Print_self() const; //вывод переменных (некрасивый, для личного пользования)
	simple_object inverse_transformation(simple_object &obj); //применение обратного преобразования

};


class Affine_transformation : public Moving  //потомок класса движения, отличается от него только конструкторами
{
public:
	//Affine_transformation();
	Affine_transformation(int axis, double coeff, string id); //преобразование растяжения вдоль axis на coeff
};


class Projection : public Transformation  //преобразование проекции
{
private:
	matrix<double> proj_matrix;
public:
	Projection(matrix<double> &projection_matrix, string id);
	simple_object* operator() (simple_object& obj); //применение преобразования
	Line* operator() (Line& obj); //применение преобразования
};

class Operation_list { //список преобразований, возможно будет заменен на более сложный контейнер когда-нибудь потом
private:
	map<string, Transformation> operations;
public:
	void add(Transformation &elem);
	void erase(string id);
	Transformation* find(string id); //поиск элемента; возвращает NULL, если элемента нету в списке
};

void print_list_of_objects(map<string, object*> list);


class  Programm_element { //элемент программы преобразований - содержит одну операцию
private:
	string trans;
	string argument;
	string result;
	Operation_list* operations; //откуда берем преобразования
	map<string, object*>* objects; //откуда берем объекты
public:
	Programm_element(string trans, string argument, string result, Operation_list* operations, map<string, object*>* objects);
	int execute(); //применяет trans к argument, записывает результат в result
};

class Programm_of_transformations {
private:
	list<Programm_element> programm; //список операций
 	Operation_list* operations; //откуда берем преобразования
	map<string, object*>* objects; //откуда берем объекты
public:
	Programm_of_transformations(Operation_list* operations, map<string, object*>* objects);
	void add_elem(string trans, string argument, string result); 
	int execute(); //выполнить последовательно все операции
};
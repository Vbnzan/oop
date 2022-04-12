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

bool is_orthogonal(matrix<double> &matr, int size, double accurasy); //��������� ��������������� �������
void print_vect_4(vector<double> obj);
double angle_between_vectors(vector<double>& vec1, vector<double>& vec2);


class object {
public:
	virtual void print_self() const { printf("Something is wrong\n"); };
};

class simple_object : public object, public vector<double> { //����� ��� ������
public:
	virtual void print_self() const;
	simple_object() {};
	simple_object(vector<double> a);
};

class Line : public object { //������ � ������������ �������� ������ �� ��� � ������������
public:
	simple_object point;
	simple_object direction;
	void print_self() const;
};





class Transformation { //�����-�� ��������������
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
	}; //�� ������ �����������, ���������� ������� - ��������

};


class Moving : public Transformation{ //�������������� ��������
protected:
	matrix<double> turning_matrix;
	vector<double> moving_vect;
public:
	Moving();  //�������������� ��������������
	Moving(vector<double> &vect, string id); //����� �� ������
	Moving(vector<double> &direction, double rotation_angle, string id); //������� �� rotation_angle ������ ��� direction
	Moving(matrix<double> &turning_matrix, vector<double> &moving_vect, string id); //������� �������������� �� �������� ������� � ������� ���������	
	simple_object* operator() (simple_object &obj); //���������� ��������������
	Line* operator() (Line  &obj); //���������� ��������������
	void Print_self() const; //����� ���������� (����������, ��� ������� �����������)
	simple_object inverse_transformation(simple_object &obj); //���������� ��������� ��������������

};


class Affine_transformation : public Moving  //������� ������ ��������, ���������� �� ���� ������ ��������������
{
public:
	//Affine_transformation();
	Affine_transformation(int axis, double coeff, string id); //�������������� ���������� ����� axis �� coeff
};


class Projection : public Transformation  //�������������� ��������
{
private:
	matrix<double> proj_matrix;
public:
	Projection(matrix<double> &projection_matrix, string id);
	simple_object* operator() (simple_object& obj); //���������� ��������������
	Line* operator() (Line& obj); //���������� ��������������
};

class Operation_list { //������ ��������������, �������� ����� ������� �� ����� ������� ��������� �����-������ �����
private:
	map<string, Transformation> operations;
public:
	void add(Transformation &elem);
	void erase(string id);
	Transformation* find(string id); //����� ��������; ���������� NULL, ���� �������� ���� � ������
};

void print_list_of_objects(map<string, object*> list);


class  Programm_element { //������� ��������� �������������� - �������� ���� ��������
private:
	string trans;
	string argument;
	string result;
	Operation_list* operations; //������ ����� ��������������
	map<string, object*>* objects; //������ ����� �������
public:
	Programm_element(string trans, string argument, string result, Operation_list* operations, map<string, object*>* objects);
	int execute(); //��������� trans � argument, ���������� ��������� � result
};

class Programm_of_transformations {
private:
	list<Programm_element> programm; //������ ��������
 	Operation_list* operations; //������ ����� ��������������
	map<string, object*>* objects; //������ ����� �������
public:
	Programm_of_transformations(Operation_list* operations, map<string, object*>* objects);
	void add_elem(string trans, string argument, string result); 
	int execute(); //��������� ��������������� ��� ��������
};
#pragma once
#include <iostream>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <cmath>   
using std::string;
using boost::numeric::ublas::matrix;
using boost::numeric::ublas::vector;

bool is_orthogonal(matrix<double> &matr, int size, double accurasy);
void print_vect_4(vector<double> obj);

class Moving {
private:
	string id;
	matrix<double> turning_matrix;
	vector<double> moving_vect;
public:
	Moving(string id); 
	Moving(vector<double> &vect, string id);  //�������������� ��������������
	Moving(vector<double> &direction, double rotation_angle, string id); //������� �� rotation_angle ������ ��� direction
	Moving(matrix<double> &turning_matrix, vector<double> &moving_vect, string id); //������� �������������� �� �������� ������� � ������� ���������	
	vector<double> operator() (vector<double> &obj); //���������� ��������������
	struct Line* operator() (struct Line  &obj); //���������� ��������������
	void Print_self() const; //����� ���������� (����������, ��� ������� �����������)
	vector<double> inverse_transformation(vector<double> &obj); //���������� ��������� ��������������

};

struct Line { //������ � ������������ �������� ������ �� ��� � ������������
	vector<double> point;
	vector<double> direction;
};

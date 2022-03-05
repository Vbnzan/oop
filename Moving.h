#pragma once
#include <iostream>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <cmath>   
using std::string;
using boost::numeric::ublas::matrix;
using boost::numeric::ublas::vector;

bool is_orthogonal(matrix<double>* matr, int size, double accurasy);
void print_vect_4(vector<double> obj);

class Moving {
private:
	string id;
	matrix<double>* turning_matrix;
	vector<double>* moving_vect;
public:
	Moving();
	Moving(vector<double> vect);  //тождевственное преобразование
	Moving(vector<double> direction, double rotation_angle); //поворот на rotation_angle вокруг оси direction
	Moving(matrix<double>* turning_matrix, vector<double>* moving_vect); //создать преобразование по заданной матрице и векторе смкещения	
	vector<double> operator() (vector<double> obj); //применение преобразования
	struct Line* operator() (struct Line* obj); //применение преобразования
	void Print_self(); //вывод переменных (некрасивый, для личного пользования)
	vector<double> inverse_transformation(vector<double> obj); //применение обратного преобразования

};

typedef struct Line { //прямая в пространстве задается точкой на ней и направлением
	vector<double> point;
	vector<double> direction;
};

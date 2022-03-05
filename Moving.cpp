#include "Moving.h" 

extern string moving_id = "0";

Moving::Moving() {
	turning_matrix = new matrix<double>(3, 3);
	turning_matrix->clear();
	(*turning_matrix)(0, 0) = 1.;
	(*turning_matrix)(1, 1) = 1.;
	(*turning_matrix)(2, 2) = 1.;
	//создали единичную матрицу

	moving_vect = new vector<double>(4);
	moving_vect->clear();
	//создали нулевой вектор

	this->id = moving_id;
	int num = std::stoi(moving_id);
	num += 1;
	moving_id = std::to_string(num);
	//уникальный строковой id/
}

Moving::Moving(vector<double> vect) {
	turning_matrix = new matrix<double>(3, 3);
	turning_matrix->clear();
	(*turning_matrix)(0, 0) = 1.;
	(*turning_matrix)(1, 1) = 1.;
	(*turning_matrix)(2, 2) = 1.;
	//создали единичную матрицу

	if (vect.size() != 4) {
		//do_something;

		std::cout << "Wrong argument: vector must contain 4 elements" << std::endl;
		return;
	}
	if (vect(3)) {
		std::cout << "Wrong argument: moving vector must be a vector (w = 0)" << std::endl;
		return;
	}
	moving_vect = new vector<double>(vect);
	//moving_vect - копия заданного вектора

	this->id = moving_id;
	int num = std::stoi(moving_id);
	num += 1;
	moving_id = std::to_string(num);
};

Moving::Moving(vector<double> direction, double rotation_angle) {
	if (direction.size() != 4) {
		//do_something;
		std::cout << "Wrong argument: vector must contain 4 elements" << std::endl;
		return;
	}
	double len = pow((pow(direction(0), 2) + pow(direction(1), 2) + pow(direction(2), 2)), 0.5);
	direction /= len;
	//нормировка 


	turning_matrix = new matrix<double>(3, 3);
	(*turning_matrix)(0, 0) = cos(rotation_angle) + (1 - cos(rotation_angle)) * direction(0) * direction(0);
	(*turning_matrix)(1, 1) = cos(rotation_angle) + (1 - cos(rotation_angle)) * direction(1) * direction(1);
	(*turning_matrix)(2, 2) = cos(rotation_angle) + (1 - cos(rotation_angle)) * direction(2) * direction(2);
	(*turning_matrix)(0, 1) = (1 - cos(rotation_angle)) * direction(0) * direction(1) - sin(rotation_angle) * direction(2);
	(*turning_matrix)(1, 0) = (1 - cos(rotation_angle)) * direction(0) * direction(1) + sin(rotation_angle) * direction(2);
	(*turning_matrix)(2, 0) = (1 - cos(rotation_angle)) * direction(0) * direction(2) - sin(rotation_angle) * direction(1);
	(*turning_matrix)(0, 2) = (1 - cos(rotation_angle)) * direction(0) * direction(2) + sin(rotation_angle) * direction(1);
	(*turning_matrix)(2, 1) = (1 - cos(rotation_angle)) * direction(2) * direction(1) + sin(rotation_angle) * direction(0);
	(*turning_matrix)(1, 2) = (1 - cos(rotation_angle)) * direction(1) * direction(2) - sin(rotation_angle) * direction(0);
	//по известной формуле

	//std::cout << "is orthogonal  "<< is_orthogonal(turning_matrix, 3, 0.0001) << std::endl;
	moving_vect = new vector<double>(4);
	moving_vect->clear();

	this->id = moving_id;
	int num = std::stoi(moving_id);
	num += 1;
	moving_id = std::to_string(num);
}

Moving::Moving(matrix<double>* turning_matrix, vector<double>* moving_vect) {
	if (!is_orthogonal(turning_matrix, 3, 0.001)) {
		std::cout << "Wrong argument: matrix must be orthogonal" << std::endl;
		//std::cout << "is m orthogonal:  " << is_orthogonal(turning_matrix, 3, 0.0001) << std::endl;
		return;
	}
	if (moving_vect->size() != 4) {
		std::cout << "Wrong argument: vector must contain 4 elements" << std::endl;
		return;
	}
	if ((*moving_vect)(3)) {
		std::cout << "Wrong argument: moving vector must be a vector (w = 0)" << std::endl;
		return;
	}
	this->turning_matrix = turning_matrix;
	this->moving_vect = moving_vect;
	//если все хорошо, то просто присваиваем

	this->id = moving_id;
	int num = std::stoi(moving_id);
	num += 1;
	moving_id = std::to_string(num);

}

vector<double> Moving::operator() (vector<double> obj) {


	vector<double>* object = new vector<double>(obj); //копия

	double w = (*object)(3);
	object->resize(3); //отрезали w от вектора

	vector<double> res(3);

	res = prod(*turning_matrix, *object); 	//умножение на матрицу

	res.resize(4); //пришиваем w обратно
	res(3) = 0;
	if (w) {   //вектор не нужно смещать
		res /= w;  //нормировка
		res(3) = 1;
		res += *moving_vect; //смещение
	}
	return res;
}

struct Line* Moving::operator() (struct Line* obj) { //для преобразования линии просто преобразуем и точку и вектор в ней
	vector<double> res_point = (*this)(obj->point);
	vector<double> res_direct = (*this)(obj->direction);
	Line* res = new Line;
	res->point = res_point;
	res->direction = res_direct;
	return res;
}

vector<double> Moving::inverse_transformation(vector<double> obj) {
	Moving mov1 = Moving((-1) * (*moving_vect)); //смещение на -1 * moving_vect

	matrix<double>  tr = trans(*turning_matrix);
	vector<double> zero(4);
	zero.clear();
	Moving mov2 = Moving(&tr, &zero);  //умножение на обратную матрицу

	vector<double> res = mov2(mov1(obj)); //сначала смещаемся, потом умножаем
	return res;
}

void Moving::Print_self() //неважно
{

	std::cout << "id:" << id << std::endl;
	std::cout << "Tutning matrix:" << std::endl;
	std::cout << turning_matrix->size1() << "  " << turning_matrix->size2() << std::endl;

	for (int i = 0; i < turning_matrix->size1(); i++) {
		for (int j = 0; j < turning_matrix->size2(); j++) {
			std::cout << (*turning_matrix)(i, j) << std::endl;
		}
	}
	std::cout << "Vector:" << std::endl;
	std::cout << "size:" << moving_vect->size() << std::endl;
	for (int i = 0; i < moving_vect->size(); i++) {
		std::cout << (*moving_vect)(i) << std::endl;
	}
}

bool is_orthogonal(matrix<double>* matr, int size, double accurasy) {
	//матрица ортогональна если произведение ее на ее транстпонированную - единичная матрица

	matrix<double> matr_transposed = boost::numeric::ublas::trans(*matr);
	matrix<double> product = prod(*matr, matr_transposed);
	bool flag = true;

	//проверяем равенство с единичной (с заданной точностью)
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++) {
			if (i == j) {
				if ((product(i, j) - 1 > accurasy) || (product(i, j) - 1 < -accurasy)) {
					flag = false;
					//std::cout << "i = " << i << "j = " << j << "prod = " << product(i, j) << std::endl;
				}
			}
			else {
				if (((product(i, j) > accurasy) || (product(i, j) < -accurasy))) {
					flag = false;
					//std::cout << "i = " << i << " j = " << j << " prod = " << product(i, j) << " flag = " << flag << std::endl;
				}
			}
		}
	}
	return flag;
}

void print_vect_4(vector<double> obj) {
	std::cout << "(" << obj(0) << ", " << obj(1) << ", " << obj(2) << ", " << obj(3) << ")" << std::endl;
}


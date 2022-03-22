#include "Moving.h" 



Moving::Moving(string id) {
	turning_matrix = matrix<double>(3, 3);
	turning_matrix.clear();
	(turning_matrix)(0, 0) = 1.;
	(turning_matrix)(1, 1) = 1.;
	(turning_matrix)(2, 2) = 1.;
	//������� ��������� �������

	moving_vect = vector<double>(4);
	moving_vect.clear();
	//������� ������� ������

	this->id = id;
}

Moving::Moving(vector<double> &vect, string id) {
	turning_matrix = matrix<double>(3, 3);
	turning_matrix.clear();
	(turning_matrix)(0, 0) = 1.;
	(turning_matrix)(1, 1) = 1.;
	(turning_matrix)(2, 2) = 1.;
	//������� ��������� �������

	if (vect.size() != 4) {
		//do_something;

		std::cout << "Wrong argument: vector must contain 4 elements" << std::endl;
		return;
	}
	if (vect(3)) {
		std::cout << "Wrong argument: moving vector must be a vector (w = 0)" << std::endl;
		return;
	}
	moving_vect = vector<double>(vect);
	//moving_vect - ����� ��������� �������

	this->id = id;
};

Moving::Moving(vector<double> &direct, double rotation_angle, string id) {
	if (direct.size() != 4) {
		//do_something;
		std::cout << "Wrong argument: vector must contain 4 elements" << std::endl;
		return;
	}
	vector<double> direction = vector<double>(direct); //�����
	double len = pow((pow(direction(0), 2) + pow(direction(1), 2) + pow(direction(2), 2)), 0.5);
	direction /= len;
	//���������� 


	turning_matrix = matrix<double>(3, 3);
	(turning_matrix)(0, 0) = cos(rotation_angle) + (1 - cos(rotation_angle)) * direction(0) * direction(0);
	(turning_matrix)(1, 1) = cos(rotation_angle) + (1 - cos(rotation_angle)) * direction(1) * direction(1);
	(turning_matrix)(2, 2) = cos(rotation_angle) + (1 - cos(rotation_angle)) * direction(2) * direction(2);
	(turning_matrix)(0, 1) = (1 - cos(rotation_angle)) * direction(0) * direction(1) - sin(rotation_angle) * direction(2);
	(turning_matrix)(1, 0) = (1 - cos(rotation_angle)) * direction(0) * direction(1) + sin(rotation_angle) * direction(2);
	(turning_matrix)(2, 0) = (1 - cos(rotation_angle)) * direction(0) * direction(2) - sin(rotation_angle) * direction(1);
	(turning_matrix)(0, 2) = (1 - cos(rotation_angle)) * direction(0) * direction(2) + sin(rotation_angle) * direction(1);
	(turning_matrix)(2, 1) = (1 - cos(rotation_angle)) * direction(2) * direction(1) + sin(rotation_angle) * direction(0);
	(turning_matrix)(1, 2) = (1 - cos(rotation_angle)) * direction(1) * direction(2) - sin(rotation_angle) * direction(0);
	//�� ��������� �������

	//std::cout << "is orthogonal  "<< is_orthogonal(turning_matrix, 3, 0.0001) << std::endl;
	moving_vect = vector<double>(4);
	moving_vect.clear();

	this->id = id;
}

Moving::Moving(matrix<double> &turning_matrix, vector<double> &moving_vect, string id) {
	if (!is_orthogonal(turning_matrix, 3, 0.001)) {
		std::cout << "Wrong argument: matrix must be orthogonal" << std::endl;
		//std::cout << "is m orthogonal:  " << is_orthogonal(turning_matrix, 3, 0.0001) << std::endl;
		return;
	}
	if (moving_vect.size() != 4) {
		std::cout << "Wrong argument: vector must contain 4 elements" << std::endl;
		return;
	}
	if ((moving_vect)(3)) {
		std::cout << "Wrong argument: moving vector must be a vector (w = 0)" << std::endl;
		return;
	}
	this->turning_matrix = matrix<double>(turning_matrix);
	this->moving_vect = vector<double>(moving_vect);
	//���� ��� ������, �� ������ �����������

	this->id = id;
}

vector<double> Moving::operator() (vector<double> &obj) {


	vector<double>* object = new vector<double>(obj); //�����

	double w = (*object)(3);
	object->resize(3); //�������� w �� �������

	vector<double> res(3);

	res = prod(turning_matrix, *object); 	//��������� �� �������

	res.resize(4); //��������� w �������
	res(3) = 0;
	if (w) {   //������ �� ����� �������
		res /= w;  //����������
		res(3) = 1;
		res += moving_vect; //��������
	}
	return res;
}

struct Line* Moving::operator() (struct Line &obj) { //��� �������������� ����� ������ ����������� � ����� � ������ � ���
	vector<double> res_point = (*this)(obj.point);
	vector<double> res_direct = (*this)(obj.direction);
	Line *res = new Line;
	res->point = res_point;
	res->direction = res_direct;
	return res;
}

vector<double> Moving::inverse_transformation(vector<double> &obj) {
	vector<double> mov_vect = (-1) * (moving_vect);
	Moving mov1 = Moving(mov_vect, "s"); //�������� �� -1 * moving_vect

	matrix<double>  tr = trans(turning_matrix);
	vector<double> zero(4);
	zero.clear();
	Moving mov2 = Moving(tr, zero, "dont matter");  //��������� �� �������� �������

	vector<double> m1 = mov1(obj);
	vector<double> res = mov2(m1); //������� ���������, ����� ��������
	return res;
}

void Moving::Print_self() const //�������
{

	std::cout << "id:" << id << std::endl;
	std::cout << "Tutning matrix:" << std::endl;
	std::cout << turning_matrix.size1() << "  " << turning_matrix.size2() << std::endl;

	for (int i = 0; i < turning_matrix.size1(); i++) {
		for (int j = 0; j < turning_matrix.size2(); j++) {
			std::cout << (turning_matrix)(i, j) << std::endl;
		}
	}
	std::cout << "Vector:" << std::endl;
	print_vect_4(moving_vect);
}

bool is_orthogonal(matrix<double> &matr, int size, double accurasy) {
	//������� ������������ ���� ������������ �� �� �� ������������������ - ��������� �������

	matrix<double> matr_transposed = boost::numeric::ublas::trans(matr);
	matrix<double> product = prod(matr, matr_transposed);
	bool flag = true;

	//��������� ��������� � ��������� (� �������� ���������)
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


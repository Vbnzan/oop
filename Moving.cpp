#include "Moving.h" 



Moving::Moving() {
	turning_matrix = matrix<double>(3, 3);
	turning_matrix.clear();
	(turning_matrix)(0, 0) = 1.;
	(turning_matrix)(1, 1) = 1.;
	(turning_matrix)(2, 2) = 1.;
	//создали единичную матрицу

	moving_vect = vector<double>(4);
	moving_vect.clear();
	//создали нулевой вектор

	this->id = "";
}

Moving::Moving(vector<double> &vect, string id) {
	turning_matrix = matrix<double>(3, 3);
	turning_matrix.clear();
	(turning_matrix)(0, 0) = 1.;
	(turning_matrix)(1, 1) = 1.;
	(turning_matrix)(2, 2) = 1.;
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
	moving_vect = vector<double>(vect);
	//moving_vect - копия заданного вектора

	this->id = id;
};

Moving::Moving(vector<double> &direct, double rotation_angle, string id) {
	if (direct.size() != 4) {
		//do_something;
		std::cout << "Wrong argument: vector must contain 4 elements" << std::endl;
		return;
	}
	vector<double> direction = vector<double>(direct); //копия
	double len = pow((pow(direction(0), 2) + pow(direction(1), 2) + pow(direction(2), 2)), 0.5);
	direction /= len;
	//нормировка 


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
	//по известной формуле

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
	//если все хорошо, то просто присваиваем

	this->id = id;
}

simple_object* Moving::operator() (simple_object &obj) {


	vector<double>* object = new vector<double>(obj); //копия

	double w = (*object)(3);
	object->resize(3); //отрезали w от вектора

	vector<double> res(3);

	res = prod(turning_matrix, *object); 	//умножение на матрицу

	res.resize(4); //пришиваем w обратно
	res(3) = 0;
	if (w) {   //вектор не нужно смещать
		res /= w;  //нормировка
		res(3) = 1;
		res += moving_vect; //смещение
	}

	//simple_object ret(res);
	simple_object* r = new simple_object(res); //сделали simple_object из результата
	return r;
}

Line* Moving::operator() (Line &obj) { //для преобразования линии просто преобразуем и точку и вектор в ней
	simple_object res_point = *(*this)(obj.point);
	simple_object res_direct = *(*this)(obj.direction);
	Line *res = new Line;
	res->point = res_point;
	res->direction = res_direct;
	return res;
}

simple_object Moving::inverse_transformation(simple_object &obj) {
	vector<double> mov_vect = (-1) * (moving_vect);
	Moving mov1 = Moving(mov_vect, "s"); //смещение на -1 * moving_vect

	matrix<double>  tr = trans(turning_matrix);
	vector<double> zero(4);
	zero.clear();
	Moving mov2 = Moving(tr, zero, "dont matter");  //умножение на обратную матрицу

	simple_object m1 = *mov1(obj);
	simple_object res = *mov2(m1); //сначала смещаемся, потом умножаем
	return res;
}

void Moving::Print_self() const //неважно
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
	//матрица ортогональна если произведение ее на ее транстпонированную - единичная матрица

	matrix<double> matr_transposed = boost::numeric::ublas::trans(matr);
	matrix<double> product = prod(matr, matr_transposed);
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

double angle_between_vectors(vector<double>& vec1, vector<double>& vec2) 
{
	double len1 = pow((pow(vec1(0), 2) + pow(vec1(1), 2) + pow(vec1(2), 2)), 0.5);
	double len2 = pow((pow(vec2(0), 2) + pow(vec2(1), 2) + pow(vec2(2), 2)), 0.5);
	double scalar_mult = vec1(0) * vec2(0) + vec1(1) * vec2(1) + vec1(2) * vec2(2);
	double cos_angle = scalar_mult / len1 / len2; //известная формула косинуса угла
	double angle = acos(cos_angle); //арккосинус
	return angle;
}



Affine_transformation::Affine_transformation(int axis, double coeff, string id) {
	turning_matrix = matrix<double>(3, 3);
	turning_matrix.clear();
	(turning_matrix)(0, 0) = coeff;
	(turning_matrix)(1, 1) = coeff;
	(turning_matrix)(2, 2) = coeff;

	if (axis < 0 || axis >= 3)
	{
		std::cout << "Wrong argument: axis must be from 0 to 2" << std::endl;
		return;
	}
	turning_matrix(axis, axis) = 1.; //растягиваем по всем координатам кроме выделенной оси

	moving_vect = vector<double>(4);
	moving_vect.clear();
	//создали нулевой вектор

	this->id = id;
}

Projection::Projection(matrix<double>& projection_matrix, string id)
{
	if (projection_matrix.size1() != 4 || projection_matrix.size2() != 4)
	{
		std::cout << "Wrong argument: matrix must be 4*4" << std::endl;
		return;
	};
	this->proj_matrix = projection_matrix;
	this->id = id;
};


simple_object* Projection::operator() (simple_object &obj) {
	vector<double> res = prod(proj_matrix, obj); //применение проекции - умножение объекта на матрицу преобразования
	simple_object* r = new simple_object(res);
	return r;
};


Line* Projection::operator() (Line& obj) { //для преобразования линии просто преобразуем и точку и вектор в ней
	simple_object res_point = *(*this)(obj.point);
	simple_object res_direct = *(*this)(obj.direction);
	Line* res = new Line;
	res->point = res_point;
	res->direction = res_direct;
	return res;
}



void Operation_list::add(Transformation& elem) {
	operations[elem.get_id()] = elem;
}

void Operation_list::erase(string id) {
	operations.erase(id);
};

Transformation* Operation_list::find(string id) {
	auto el = operations.find(id);
	Transformation* res;
	if (el == operations.end()) 
	{
		res = NULL;
	}
	else 
	{
		res = &(el->second);
	}
	return res;
}




void Line::print_self() const{
	std::cout << "p: (" << point(0) << ", " << point(1) << ", " << point(2) << ", " << point(3) << ") d: (" << direction(0) << ", " << direction(1) << ", " << direction(2) << ", " << direction(3) << ")" << std::endl;
};

void simple_object::print_self() const {
	print_vect_4(*this);
};

void print_list_of_objects(map<string, object*> list) {
	map<string, object*>::const_iterator it;
	it = list.begin();
	for (; it != list.end(); ++it) {
		std::cout << it->first << std::endl;
		//printf("%s   ", it->first);
		(it->second)->print_self();
	};
}

int Programm_element::execute() {
	Transformation* trans_ptr = operations->find(trans);
	if (trans_ptr == NULL) { //проверяем наличие преобразования
		printf("-1");
		return -1; };
	auto arg = (objects->find(argument));
	if (arg == objects->end()) { //проверяем наличие аргумента
		printf("-2");
		return -2; };
	object* argument_ptr = arg->second;


	object* res = (*trans_ptr)(*argument_ptr); //по этой части у меня и вопрос

	//res->print_self();
	(*objects)[result] = res;

	return 0;
};

simple_object::simple_object(vector<double> a) { //создаем simple_object с таким же содержимым, что и заданный вектор
	auto s = a.size();
	this->resize(s);
	for (int i = 0; i < s; i++)
	{
		(*this)(i) = a(i);
	}
}

Programm_element::Programm_element(string trans, string argument, string result, Operation_list* operations, map<string, object*>* objects)
{
	this->trans = trans;
	this->argument = argument;
	this->result = result;
	this->operations = operations;
	this->objects = objects;
}

Programm_of_transformations::Programm_of_transformations(Operation_list* operations, map<string, object*>* objects)
{
	this->operations = operations;
	this->objects = objects;
}

void Programm_of_transformations::add_elem(string trans, string argument, string result)
{
	Programm_element el = Programm_element(trans, argument, result, operations, objects);
	programm.push_back(el);
}

int Programm_of_transformations::execute() 
{
	for (auto &el: programm)
	{
		if (el.execute())
		{
			return -1;
		}
	}
	return 0;
}
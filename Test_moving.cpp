#define PI 3.14159265      
#include "Moving.h"


void test_moving() {
	vector<double> a(4); //������� �����
	a(0) = 1;
	a(1) = 2;
	a(2) = 3;
	a(3) = 4;

	vector<double> b(4); //������� ������
	b.clear();
	b(0) = 1;

	vector<double> �(4); //������� ��� ���� �����
	�(0) = 2;
	�(1) = 0;
	�(2) = 1;
	�(3) = 1;


	struct Line* line1 = new struct Line; //c������ �����
	line1->direction = b;
	line1->point = �;

	vector<double> d(7); //������� ��������� ���

	double angle = PI / 2;

	Moving t0; //������� �������������� ��������������
	std::cout << "t0(a):" << std::endl;
	print_vect_4(t0(a)); //�������������� �������������� �� ������ �����

	Moving t1(b); //����� �� ������ b
	std::cout << "t1(a):" << std::endl;
	print_vect_4(t1(a));  //����� �������� �����

	std::cout << "t1(b):" << std::endl; //����� �� �������� ������ 
	print_vect_4(t1(b));

	std::cout << "t1(line1):" << std::endl; //����� �� ������ �����������, �� �������� �����
	Line* line2 = t1(line1);
	std::cout << "line direction" << std::endl;
	print_vect_4(line2->direction);
	std::cout << "line point" << std::endl;
	print_vect_4(line2->point);


	Moving t2(a); //�������� ������� ����� �� ����� - ��������� ��������
	Moving wr(d); //�������� ������� ����� �� ��������� ��� - ��������� ��������


	double angle1 = PI / 2;
	Moving t3(b, angle1); //������� �� pi/2 ������ b
	std::cout << "t3(a):" << std::endl;
	print_vect_4(t3(a));

	std::cout << "t3(line1):" << std::endl;
	Line* line3 = t3(line1);
	std::cout << "line direction" << std::endl;
	print_vect_4(line3->direction);
	std::cout << "line point" << std::endl;
	print_vect_4(line3->point);
	//��� ������ ��������������



	double angle2 = PI / 3;
	matrix<double>* m1 = new matrix<double>(3, 3);
	m1->clear();
	(*m1)(0, 0) = cos(angle2);
	(*m1)(0, 1) = -sin(angle2);
	(*m1)(1, 1) = cos(angle2);
	(*m1)(1, 0) = sin(angle2);
	(*m1)(2, 2) = 1.; //������� ������������� �������
	std::cout << "is m1 orthogonal:  " << is_orthogonal(m1, 3, 0.0001) << std::endl;

	matrix<double>* m2 = new matrix<double>(3, 3);
	m2->clear();
	(*m2)(0, 0) = 2;
	(*m2)(0, 1) = -3;
	(*m2)(1, 1) = 5;
	(*m2)(1, 0) = sin(angle2); //������� �� ������������� �������
	std::cout << "is m2 orthogonal:  " << is_orthogonal(m2, 3, 0.0001) << std::endl;

	Moving t4(m1, &b); //������� �������������� � ������������� ��������
	std::cout << "t4(a):" << std::endl;
	print_vect_4(t4(a));
	std::cout << "t4(line1):" << std::endl;
	Line* line4 = t4(line1);
	std::cout << "line direction" << std::endl;
	print_vect_4(line4->direction);
	std::cout << "line point" << std::endl;
	print_vect_4(line4->point);

	Moving t5(m2, &b); //���������� ������� �������������� � ������������� �������� - ��������� ��������
	Moving t6(m1, &a); //���������� ������� �������������� �� ������� �� ����� - ��������� ��������
	Moving t7(m1, &d); //���������� ������� �������������� �� ������� �� ��������� ��� - ��������� ��������

	std::cout << " inverse transformation of t4(a):" << std::endl; //�������� �������������� ���������� t4(a) ������� � a
	print_vect_4(t4.inverse_transformation(t4(a)));
	
	//t4.Print_self();
}


int main() {
	test_moving();

	return 0;
}
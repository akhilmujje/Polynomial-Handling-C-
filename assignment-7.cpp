/*
Names: Justin Rubio & Akhil Mujje
UTEID: jer3634 & dam4335
Sections: 16220 & 16215
EE312 - Assignment 7
*/

#include "poly.h"
#include <iostream>
#include <string.h>
#include <vector>

#define MAX_LENGTH 1000
#define CHAR_TO_INT_OFFSET 48
using namespace std;

// Main function of your program - called by main
void runProgram() {
	// repeatedly prompt the user and process the selected
	// operation until the user selects the quit option.

menu_top:

	cout << "Enter the file that you would like to work with: ";
	char* fileName = new char[MAX_LENGTH];
	cin >> fileName;
	FILE * file = fopen(fileName, "r");
	int numPoly = fgetc(file) - CHAR_TO_INT_OFFSET; //gets the number of polynomials in file
	fgetc(file); //clears the newline
	int count = 0;
	char* arr = new char[MAX_LENGTH]; //destroy at end
	char** poly_arr = new char*[numPoly];
	for (int i = 0; i < numPoly; i++)
		poly_arr[i] = new char[MAX_LENGTH];

	for (int i = 0; i < numPoly; i++) {
		fgets(arr, MAX_LENGTH, file);
		strcpy(poly_arr[i], arr);
	}
	cout << "The polynomials available for operation are :" << endl;
	for (int i = 0; i < numPoly; i++) {
		cout << i + 1 << ". " << poly_arr[i];
	}

more_operations:

	cout << "What operation would you like to perform ?" << endl;
	cout << "1. ADD polynomials" << endl;
	cout << "2. MULTIPLY polynomials" << endl;
	cout << "3. EVALUATE polynomial" << endl << endl;
	cout << "Enter choice #: ";
	int choice = 0;

	cin >> choice;

	switch (choice) {

	case 1: {
		int one, two = 0;
		cout << endl << "Enter the two polynomials that you would like to work with : ";
		scanf("%d,%d", &one, &two);
		cout << "The symbolic sum of the 2 polynomials is : " << endl;
		Poly p1(poly_arr[one - 1]);
		Poly p2(poly_arr[two - 1]);
		Poly* p3 = p1.add(p2);
		p3->print();
		delete p3;

		break;
	}
	case 2: {
		int one, two = 0;
		cout << endl << "Enter the two polynomials that you would like to work with : ";
		scanf("%d,%d", &one, &two);
		cout << "The symbolic product of the 2 polynomials is : " << endl;
		Poly p1(poly_arr[one - 1]);
		Poly p2(poly_arr[two - 1]);
		Poly* p3 = p1.multiply(p2);
		p3->print();
		delete p3;
		break;
	}
	case 3: {
		cout << endl << "Enter the polynomial that you would like to work with : ";
		int one;
		double sol;
		cin >> one;
		cout << endl << "Enter the evaluation point (the value of x): ";
		cin >> sol;
		cout << endl << "Value of that polynomial at " << sol << " is: ";
		Poly p(poly_arr[one - 1]);
		printf("%.1lf", p.eval(sol));
		cout << endl;
		break;
	}
	}


	cout << endl << "Do you want to perform additional operations on the existing file(Y / N) ? ";
	char c = getchar(); //clear newline
	scanf("%c", &c);
	if (c == 'Y') {
		goto more_operations;
	}
	else {
		cout << "Do you want to work with another file (Y/N)? ";
		char c = getchar(); //clear newline

		for (int i = 0; i < numPoly; i++)
			delete[] poly_arr[i];
		delete[] poly_arr;
		delete[] arr;
		delete[] fileName;

		scanf("%c", &c);
		if (c == 'Y') {
			goto menu_top;
		}
		else {
			cout << endl << "Thank you for using this program!";

		}
	}

}

Poly::Poly(char* str) {

	char* str3 = new char[MAX_LENGTH]();    //temp char[] for coefficient
	s = initialize_stk();
	int node_count = 0;
	int x_count = 0;
	bool constant = false;
	bool isNeg = false;

	for (int i = 1; str[i] != '\n'; i++) {  //count the number of operators in polynomial, which if added by one yields
		if (str[i] == '+' || str[i] == '-') {//the number of terms (nodes) in the polynomial
			node_count++;
		}
	}
	node_count++;
	for (int i = 0; str[i] != '\n'; i++) {
		if (str[i] == 'x') {
			x_count++;
		}
	}
	if (x_count + 1 == node_count)
		constant = true;

	double cof = 0;
	int index = 0;
	int j = 0;
	for (int i = 0; i < x_count; i++) {
		for (j; str[j] != 'x'; j++) {}
		index = j;
		int m = 0;
		int k = index - 1;
		int cof_count = 0;
		for (k; str[k] != ' ' && k >= 0; k--) {
			cof_count++;
		}

		m = cof_count - 1;

		for (k = index - 1; str[k] != ' ' && k >= 0; k--) {
			str3[m] = str[k];
			m--;
		}

		if (k != -1 && str[k - 1] == '-')
			isNeg = true;

		j = index + 1;

		cof = atof(str3); //get coefficient of node

		if (isNeg)
			cof = -1 * cof;

		int deg = 0;
		if (str[j] == '^') {
			deg = str[j + 1] - CHAR_TO_INT_OFFSET;
		}
		else {
			deg = 1;
		}
		//get degree of node

		if (cof != 0.0)
			push(cof, deg); //push node on stack

	}
	if (constant)
	{
		isNeg = false;
		int m = 0;

		if (x_count == 0) {
			for (int i = 0; str[i] != '\n'; i++) {
				str3[m] = str[i];
				m++;
			}
		}
		else {
			for (j; str[j] != '+' && str[j] != '-'; j++) {}

			if (str[j] == '-')
				isNeg = true;

			for (int t = j + 2; str[t] != '\n'; t++) {
				str3[m] = str[t];
				m++;
			}
		}
		cof = atof(str3);

		if (isNeg)
			cof = -1 * cof;

		int deg = 0;
		if (cof != 0.0)
			push(cof, deg);
	}
	delete[] str3;

}

Poly::Poly(Poly& otherPoly) {
	Node* temp = otherPoly.s.top;

	s = initialize_stk();

	int deg_arr[MAX_LENGTH] = { 0 };
	double cof_arr[MAX_LENGTH] = { 0.0 };
	int j = 0;
	for (int i = otherPoly.size() - 1; i >= 0; i--) {
		double co;
		int deg;
		otherPoly.getValues(i, co, deg);
		deg_arr[j] = deg;
		cof_arr[j] = co;
		j++;
	}

	for (int i = 0; i < j; i++) {
		push(cof_arr[i], deg_arr[i]);
	}
}

Poly::~Poly() {

	//delete &s;

}

Poly* Poly::add(Poly& otherPoly) {

	Poly* sum = new Poly("leh");
	sum->s = initialize_stk();

	int opCount = 1;
	int count = 1;
	double copyCo;
	double opCopyCo;
	int copyDeg;
	int opCopyDeg;
	if (otherPoly.s.size == 0) {
		return this;
	}
	if (s.size == 0) {
		return &otherPoly;
	}

	while (s.size >= count && otherPoly.s.size >= opCount) {
		otherPoly.getValues(otherPoly.s.size - opCount, opCopyCo, opCopyDeg);
		getValues(s.size - count, copyCo, copyDeg);
		if (s.size <count && otherPoly.s.size >= opCount) {
			sum->push(opCopyCo, opCopyDeg);
			opCount++;
		}

		else if (s.size >= count && otherPoly.s.size < opCount) {
			sum->push(copyCo, copyDeg);
			count++;
		}

		else if (copyDeg == opCopyDeg) {
			if (copyCo + opCopyCo != 0) {
				sum->push(copyCo + opCopyCo, copyDeg);
				opCount++;
				count++;
			}
			else {
				opCount++;
				count++;
			}
		}
		else if ((copyDeg > opCopyDeg)) {
			sum->push(copyCo, copyDeg);
			count++;
		}
		else if (copyDeg < opCopyDeg) {
			sum->push(opCopyCo, opCopyDeg);
			opCount++;
		}

	}
	return sum;
}

Poly* Poly::multiply(Poly& otherPoly) {
	Poly* prod = new Poly("leh");
	prod->s = initialize_stk();
	int opCount = 1;
	int count = 1;
	double copyCo;
	double opCopyCo;
	int copyDeg;
	int opCopyDeg;
	int pIndex = 0;
	bool flag = false;
	int prodDeg[MAX_LENGTH] = { 0 };
	double prodCo[MAX_LENGTH] = { 0 };
	if (otherPoly.s.size == 0) {
		return &otherPoly;
	}
	if (s.size == 0) {
		return this;
	}
	while (s.size >= count) {
		getValues(s.size - count, copyCo, copyDeg);
		while (otherPoly.s.size >= opCount) {
			otherPoly.getValues(otherPoly.s.size - opCount, opCopyCo, opCopyDeg);
			for (int i = 0; i < pIndex; i++) {
				if (prodDeg[i] == (copyDeg + opCopyDeg)) {
					prodCo[i] += (copyCo * opCopyCo);
					opCount++;
					flag = true;
					break;
				}
			}
			if (flag) {
				flag = false;
			}
			else {
				prodCo[pIndex] = opCopyCo * copyCo;
				prodDeg[pIndex] = copyDeg + opCopyDeg;
				pIndex++;
				opCount++;
			}
		}
		opCount = 1;
		count++;
	}
	for (int i = 0; i < pIndex; i++) {
		prod->push(prodCo[i], prodDeg[i]);
	}
	return prod;
}

double Poly::eval(double x) {

	double sum = 0;
	double power = 1;
	int count = 1;
	double copyCo;
	int copyDeg;
	while (s.size >= count) {
		getValues(s.size - count, copyCo, copyDeg);
		for (int i = 0; i < copyDeg; i++) {
			power = power * x;
		}
		sum += (power * copyCo);
		count++;
		power = 1;
	}
	return sum;
}

void Poly::print() {
	int siz = size();
	int degree;
	double coefficient;
	if (siz == 0)
		cout << "0" << endl;
	else {
		int pos = siz - 1;

		for (int i = pos; i >= 0; i--) {

			getValues(i, coefficient, degree);

			if (degree > 1) {
				printf("%.1lf", coefficient);
				cout << "x^";
				cout << degree;
				cout << " + ";
			}

			if (degree == 1) {
				printf("%.1lf", coefficient);
				cout << "x";
				cout << " + ";
			}
			if (degree == 0) {
				printf("%.1lf", coefficient);;
			}
		}
		cout << endl;
	}
}

bool Poly::equals(Poly& otherPoly) {
	int opCount = 1;
	int count = 1;
	double copyCo;
	double opCopyCo;
	int copyDeg;
	int opCopyDeg;
	while (s.size >= count && otherPoly.s.size >= opCount) {
		otherPoly.getValues(otherPoly.s.size - opCount, opCopyCo, opCopyDeg);
		getValues(s.size - count, copyCo, copyDeg);
		if (s.size <count && otherPoly.s.size >= opCount) {
			return 0;
		}
		if (s.size >= count && otherPoly.s.size < opCount) {
			return 0;
		}
		if ((copyCo != opCopyCo) || (copyDeg != opCopyDeg)) {
			return 0;
		}
		count++;
		opCount++;
	}
	return 1;
}

Stack Poly::initialize_stk() {
	Stack * stk = new Stack;
	stk->size = 0;
	stk->top = 0;
	return *stk;
}

int Poly::size() {
	return s.size;
}

void Poly::push(double co, int deg) {
	if (s.size == 0)
	{

		Node* node_ptr = new Node;
		node_ptr->coeff = co;
		node_ptr->degree = deg;
		node_ptr->next = 0;
		s.top = node_ptr;
		s.size++;

	}
	else
	{
		Node* node_ptr = new Node;
		node_ptr->coeff = co;
		node_ptr->degree = deg;
		node_ptr->next = s.top;
		s.top = node_ptr;
		s.size++;
	}
}

void Poly::getValues(int position, double &co, int &deg) {
	Node* s2 = s.top;
	for (int i = position; i > 0; i--) {
		s2 = s2->next;
	}

	co = s2->coeff;
	deg = s2->degree;
}

void Poly::destroy() {
	Node* node_ptr = s.top;
	Node* temp = node_ptr;
	while (temp) {

		temp = node_ptr->next;
		delete node_ptr;

	}

}

Poly * Poly::operator+(Poly & otherPoly) {
	return add(otherPoly);
}

double Poly::operator()(double x) {
	return eval(x);
}

bool Poly::operator==(Poly & otherPoly) {
	return equals(otherPoly);
}

Poly * Poly::operator*(Poly & otherPoly) {
	return multiply(otherPoly);
}

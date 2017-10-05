/*
Names: Justin Rubio & Akhil Mujje
UTEID: jer3634 & dam4335
Sections: 16220 & 16215
EE312 - Assignment 7
*/

#include <iostream>
#include <vector>


typedef struct Node {
	double coeff;
	int degree;
	Node* next;
} Node;

typedef struct Stack {
	int size;
	Node* top;
}Stack;

// function where the interactive menu should be implemented
void runProgram();

class Poly {

public:
	Poly(char* str);
	~Poly();
	Poly(Poly & otherPoly);//copy constructor
	Poly* add(Poly& otherPoly);
	Poly* multiply(Poly& otherPoly);
	double eval(double x);
	void print();
	bool equals(Poly& otherPoly);

	// add public members if needed

	//operator overrides
	Poly* operator+(Poly &otherPoly);
	Poly* operator*(Poly &otherPoly);
	double operator()(double x);
	bool operator==(Poly &otherPoly);

	//stack functions

	Stack initialize_stk();
	int size();
	void push(double co, int deg);
	void getValues(int position, double &co, int &deg);
	void destroy();


	/*void destroy() {
	Node* node_ptr = s.top;
	Node* temp = node_ptr;
	while(!temp) {
	temp = node_ptr->next;
	delete node_ptr;

	}
	}*/

private:
	// add private members as needed
	Stack s;
};
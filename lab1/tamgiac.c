#include "tamgiac.h"
#include<math.h>
#include<stdio.h>
double calculateTriangleArea(double canh1, double canh2, double canh3){
	double s = (canh1 + canh2 + canh3)/2;

	double area = sqrt(s*(s-canh1)*(s-canh2)*(s-canh3));

	return area;
}

double calculateTrianglePerimeter(double canh1, double canh2, double canh3){
	double p = canh1 + canh2 + canh3;

	return p;
}


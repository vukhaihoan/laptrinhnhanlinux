#include<stdio.h>
#include "hinhVuong.h"
#include "tamgiac.h"
#include "tron.h"

int main(int argc, char **argv)
{
	int x=-1;
	
	printf("Menu: 1.Tinh chu vi, dien tich tam giac 2.Tinh chu vi, dien tich hinh vuong 3.Tinh chu vi, dien tich hinh tron 4.Ket thuc: ");
	while(1){
		printf("\nNhap lua chon:");
		scanf("%d", &x);

	if (x==1) {
		double canh1, canh2, canh3;
		printf("Nhap lan luot cac canh:");
		scanf("%lf %lf %lf", &canh1, &canh2, &canh3);
		printf("Dien tich cua tam giac la: %lf\n", calculateTriangleArea(canh1, canh2, canh3));
		printf("Chu vi cua tam giac la: %lf\n", calculateTrianglePerimeter(canh1, canh2, canh3));
	} else if (x==2){
		double length;
		printf("Nhap do dai canh hinh vuong:");
		scanf("%lf", &length);
		printf("Dien tich hinh vuong la: %lf\n", calculateSquareArea(length));
		printf("Chu vi hinh vuong la: %lf\n", calculateSquarePerimeter(length));
	} else if (x==3){
		double radius;
		printf("Nhap do dai ban kinh hinh tron: ");
		scanf("%lf", &radius);
		printf("Dien tich hinh tron la: %lf\n", calculateCircleArea(radius));
		printf("Chu vi hinh tron la: %lf\n", calculateCirclePerimeter(radius));
				
	} else if (x==4){
		printf("Ket thuc\n");
		break;	
	}
	}
	
	return 0;
}

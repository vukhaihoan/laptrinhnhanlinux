#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */

#define ROWS 3
#define COLS 3

int matrix1[ROWS][COLS] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int matrix2[ROWS][COLS] = {{10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
bool is_prime(int n) {
    int i;
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    
    for(i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

int find_primes(void) {
    int i, j;
    int m=0;
    for(i = 0; i < ROWS; i++)
        for(j = 0; j < COLS; j++)
            if(is_prime(matrix1[i][j]))
                m +=1;
    return m;
}


int find_min(void) {
    int min = matrix1[0][0];
    int i, j;
    for(i = 0; i < ROWS; i++)
        for(j = 0; j < COLS; j++)
            if(matrix1[i][j] < min)
                min = matrix1[i][j];
    return min;
}

int find_max(void){
	int max = matrix1[0][0];
    	int i, j;
    	for (i = 0; i < ROWS; i++)
        	for (j = 0; j < COLS; j++)
            		if (matrix1[i][j] > max)
                		max = matrix1[i][j];
    	return max;
}
void add_matrices(int matrix1[ROWS][COLS], int matrix2[ROWS][COLS], int result[ROWS][COLS]) {
    int i, j;
    for(i = 0; i < ROWS; i++)
        for(j = 0; j < COLS; j++)
            result[i][j] = matrix1[i][j] + matrix2[i][j];
}

void multiply_matrices(int matrix1[ROWS][COLS], int matrix2[ROWS][COLS], int result[ROWS][COLS]) {
    int i, j, k;
    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++) {
            result[i][j] = 0;
            for (k = 0; k < COLS; k++)
                result[i][j] += matrix1[i][k] * matrix2[k][j];
        }
}

void print_matrix(int matrix[ROWS][COLS]) {
    int i, j;
    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLS; j++)
            printk(KERN_INFO "%d ", matrix[i][j]);
        printk(KERN_INFO "\n");
    }
}

int init_module(void) {
    int result[ROWS][COLS];
    printk(KERN_INFO "Minimum value in the matrix: %d\n", find_min());
    printk(KERN_INFO "Maximum value in the matrix: %d\n", find_max());
    printk(KERN_INFO "Number of prime number in the matrix: %d\n", find_primes());
    add_matrices(matrix1, matrix2, result);
    printk(KERN_INFO "The sum of the two matrices is:\n");
    print_matrix(result);
    multiply_matrices(matrix1, matrix2, result);
    printk(KERN_INFO "The product of the two matrices is:\n");
    print_matrix(result);
    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "Module removed.\n");
}
MODULE_LICENSE("GPL");

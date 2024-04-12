#include <linux/init.h> 
#include <linux/kernel.h> /* for ARRAY_SIZE() */ 
#include <linux/module.h> 
#include <linux/moduleparam.h> 
#include <linux/printk.h> 
#include <linux/stat.h> 

MODULE_LICENSE("GPL"); 
 
static int n = 10; 
static int m = 20;
// declare module param, param permissions: readable, writeable by user, readable by others 
module_param(n, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 
MODULE_PARM_DESC(n, "An integer n"); 
module_param(m, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 
MODULE_PARM_DESC(m, "An integer m"); 
static int matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int count_elements(void) {
    int count = 0;
    int i,j;
    for ( i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (matrix[i][j] < n) {
                count++;
            }
        }
    }
    return count;
}

int count__divide_elements(void) {
    int count = 0;
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (matrix[i][j] % n == 0) {
                count++;
            }
        }
    }
    return count;
}
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
int count_prime_elements_n(void) {
    int count = 0;
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (matrix[i][j] > n && is_prime(matrix[i][j]) ){
                count++;
            }
        }
    }
    return count;
}

int factorial_n(int n){
	if (n == 0)
		return 1;
	else
		return n * factorial_n(n - 1);
}
int count_primes_mn(int m, int n) {
    int count = 0;
    int i;
    int h=0;
    if (n <=m) {
    	h=n;
    	n=m;
    	m=h;
    }
    for (i = m; i <= n; i++) {
        if (is_prime(i)) {
            count++;
        }
    }
    return count;
}
static int __init main_init(void) 
{ 
   
    pr_info("Hello, world 5\n=============\n"); 
 
    pr_info("So phan tu nho hon n: %d\n", count_elements());
    pr_info("So phan tu chia het cho n: %d\n", count__divide_elements());
    pr_info("So phan tu la so nguyen to lon hon n: %d\n",  count_prime_elements_n());
    pr_info("Giai thua cua n: %d\n", factorial_n(n));
    pr_info("So phan tu la so nguyen to nam trong [m, n] la: %d\n", count_primes_mn(m,n));
    return 0; 
} 

static void __exit main_exit(void) 
{ 
    pr_info("Goodbye, world 5\n"); 
} 
 
module_init(main_init); 
module_exit(main_exit);

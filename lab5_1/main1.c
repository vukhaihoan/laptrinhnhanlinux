#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#define SIZE 10

typedef struct {
    char ma_sv[50];
    char ho_ten[50];
    char lop[50];
} SinhVien;

sem_t *sem;
SinhVien *sv;
volatile sig_atomic_t flag = 0;
void nhap_sinh_vien(SinhVien *sv) {
    printf("Nhap ma sinh vien: ");
    scanf("%s", sv->ma_sv);
    printf("Nhap ho ten sinh vien: ");
    scanf("%s", sv->ho_ten);
    printf("Nhap lop: ");
    scanf("%s", sv->lop);
}

void in_sinh_vien(const SinhVien *sv) {
    printf("Ma SV: %s, Ho ten: %s, Lop: %s\n", sv->ma_sv, sv->ho_ten, sv->lop);
}

void copy_sinh_vien(int sig){
    printf("\nCaught signal %d\n", sig);
    // Here you can add the code to copy data into shared memory
    for (int i = 0; i < SIZE; i++) {
        printf("Ma SV: %s, Ho ten: %s, Lop: %s\n", sv[i].ma_sv, sv[i].ho_ten, sv[i].lop);
    }
}

int main() {
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, SIZE * sizeof(SinhVien), 0666|IPC_CREAT);
    sv = (SinhVien*) shmat(shmid, (void*)0, 0);
	sem = sem_open("/mysem", O_CREAT, 0644,1);
    signal(SIGINT, copy_sinh_vien);

    int so_luong_sv = 0;
    char lua_chon;
    do {
        printf("1. Nhap sinh vien\n");
        printf("2.Nhan Ctrl +C de copy danh sach sinh vien vao vung nho chia se\n");
        printf("3. Thoat\n");
        printf("Lua chon cua ban: ");
        scanf(" %c", &lua_chon);
        switch (lua_chon) {
            case '1':
            	sem_wait(sem);
                nhap_sinh_vien(&sv[so_luong_sv++]);
                sem_post(sem);
                break;
            case '2':
            sem_wait(sem);
            	printf("Nhan Ctrl + c de copy danh sach sinh vien vao vung nho chia se ");
            	while(!flag){
            	}
            	flag=0;
            	sem_post(sem);
            	break;
            case '3':
                printf("Thoat chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long nhap lai.\n");
        }
    } while (lua_chon != '3');
sem_close(sem);
    sem_unlink("/mysem");
    shmdt(sv);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}


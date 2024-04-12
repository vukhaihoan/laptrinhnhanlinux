#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 10
volatile sig_atomic_t flag = 0;
typedef struct {
    char ma_sv[50];
    char ho_ten[50];
    char lop[50];
} SinhVien;

SinhVien *sv;

void in_sinh_vien(const SinhVien *sv) {
    printf("Ma SV: %s, Ho ten: %s, Lop: %s\n", sv->ma_sv, sv->ho_ten, sv->lop);
}

void doc_du_lieu(int sig){
    printf("\nCaught signal %d\n", sig);
    // Here you can add the code to read data from shared memory
    printf("Doc du lieu tu vung nho chia se\n");
}

int main() {
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, SIZE * sizeof(SinhVien), 0666|IPC_CREAT);
    sv = (SinhVien*) shmat(shmid, (void*)0, 0);

    signal(SIGINT, doc_du_lieu);

    char lua_chon;
    do {
        printf("1. Nhan Ctrl+V de doc du lieu tu vung nho chia se tu chuong trinh 1\n");
        printf("2. Hien thi danh sach sinh vien\n");
        printf("3. Thoat\n");
        printf("Lua chon cua ban: ");
        scanf(" %c", &lua_chon);
        switch (lua_chon) {
            case '1':
           
                raise(SIGINT);
                break;
            case '2':
                for (int i = 0; i < SIZE; i++) {
                    in_sinh_vien(&sv[i]);
                }
                break;
            case '3':
                printf("Thoat chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long nhap lai.\n");
        }
    } while (lua_chon != '3');

    shmdt(sv);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}


#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[]){
    int i, j;
    int is_prime;

    printf("Prime numbers between 1 and 100:\n");

    // 从2到100遍历每个数
    for (i = 2; i <= 35; i++) {
        is_prime = 1; // 假设当前数是素数

        // 判断当前数是否为素数
        for (j = 2; j <= i / 2; j++) {
            if (i % j == 0) {
                is_prime = 0; // 如果能整除，则不是素数
                break;
            }
        }

        // 如果是素数，则打印输出
        if (is_prime) {
            printf("prime %d\n", i);
        }
    }
    exit(0);
}
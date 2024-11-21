#include <stdio.h>

void add(int* valor) {
    printf("%d\n", (*valor)++);
}

int main(void) {

    int a = 0;

    for (int i = 0; i < 10; i++) {
        add(&a);
    }

    return 0;
}
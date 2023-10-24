#include <stdio.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")

int main(void)
{
    char c;
    c=33;
    while (1) {
        printf("%c",c);
        fflush(stdout);
        sleep(1);
        clear();
        c++;
        if (c>126) c=33;
    }
    return 0;
}

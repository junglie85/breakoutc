#include <stdio.h>

#define UNUSED(X) ((void)(X))

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    printf("Hello, Breakout!\n");

    return 0;
}

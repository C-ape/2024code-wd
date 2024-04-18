#include "func.h"

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    printf("lhs + rhs = %d\n", atoi(argv[1]) + atoi(argv[2]));
    return 0;
}


#include "../include/Runtime.h"

int main(void) {
    int x;
    DSE_Input(x);
    int y;
    DSE_Input(y);
    int z;
    DSE_Input(z);
    if (z == 0) {
        z = (x + y) / (z + 1);
    }
}
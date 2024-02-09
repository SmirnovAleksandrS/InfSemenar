#include <stdlib.h>
#include "bite_massive.h"

int main(){
    bite_massive m;
    initMass(&m);
    writeBit(&m, 3, 1);
    writeBit(&m, 9, 0);
    writeBit(&m, 25, 1);
    writeByte(&m, 7, 255);
    printMass(&m);
    for (bite_iterator* i = begin(&m); !isEqual(i, end(&m)); inc(i)){
        printf("pos: %i, bit: %i\n",i->position, i->bit);
    }

    bite_massive a;
    initMass(&a);
    writeBit(&a, 4, 1);
    writeBit(&a, 9, 0);
    writeBit(&a, 1, 1);
    printMass(&a);

    m = *massOr(&m, &a);
    printMass(&m);

    writeByte(&a, 2, 0);
    m = *massAnd(&m, &a);
    printMass(&m);

    m = *massLeft(&m, 5);
    // printf("1");
    printMass(&m);

    deleteMass(&m);
    return 0;
}
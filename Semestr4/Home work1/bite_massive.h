#ifndef _BITE_MASSIVE_
#define _BITE_MASSIVE_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
    char* mass;
    unsigned int size;
    unsigned int last;
}bite_massive;

typedef struct{
    bite_massive* mass;
    unsigned char bit;
    unsigned int position;
}bite_iterator;


//base functions
void initMass(bite_massive* mass);
unsigned char readBit(bite_massive* mass, unsigned int place);
void writeBit(bite_massive* mass, unsigned int place, char bit);
void printMassAll(bite_massive* mass);
void deleteMass(bite_massive* mass);

//Usebale functions
void printMass(bite_massive* mass);
void writeByte(bite_massive* mass, unsigned int place ,unsigned char number);

//iterators
bite_iterator* begin(bite_massive* mass);
bite_iterator* end(bite_massive* mass);
bite_iterator* inc(bite_iterator* iter);
bite_iterator* dec(bite_iterator* iter);
char isEqual(bite_iterator* iter1, bite_iterator* iter2);

//sugar
bite_massive* massOr(bite_massive* mass1, bite_massive* mass2);
bite_massive* massAnd(bite_massive* mass1, bite_massive* mass2);
bite_massive* massLeft(bite_massive* mass1, unsigned int shift);
bite_massive* massRight(bite_massive* mass1, unsigned int shift);


#endif
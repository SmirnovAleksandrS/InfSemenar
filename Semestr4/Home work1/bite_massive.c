#include "bite_massive.h"

//basic functions
void initMass(bite_massive* mass){
    mass->mass = NULL;
    mass->size = 0;
    mass->last = 0;
}

unsigned char readBit(bite_massive* mass, unsigned int place){
    if (mass->mass[place/8]&(1<<(7-place%8))){
        return 1;
    } else {
        return 0;
    }
}

void writeBit(bite_massive* mass, unsigned int place, char bit){
    if (place+1 > mass->size*8){
        mass->mass = (char *)realloc(mass->mass, place/8+1);
        mass->mass[place/8] = 0b00000000;
        mass->size ++;
    }
    if (bit){
        mass->mass[place/8] |= (0b000000001<<(7-place%8));
    } else {
        mass->mass[place/8] &= ~(0b000000001<<(7-place%8));
    }
    if (place > mass->last){
        mass->last = place;
    }
}

void printMassAll(bite_massive* mass){
    printf("0b");
    for (int i = 0; i < mass->size*8; i ++){
        printf("%i",(int)readBit(mass, i));
    }
    printf("\n");
}

void deleteMass(bite_massive* mass){
    free(mass->mass);
}

//useble functions

void printMass(bite_massive* mass){
    printf("0b");
    for (int i = 0; i < mass->last+1; i ++){
        printf("%i",(int)readBit(mass, i));
    }
    printf("\n");
}

void writeByte(bite_massive* mass, unsigned int place ,unsigned char number){
    for (int i = 0; i < 8; i++){
        writeBit(mass, place+i, number & (0b000000001<<(7-i)));
    }
}

//iterators
bite_iterator* begin(bite_massive* mass){
    bite_iterator* iter = (bite_iterator*) malloc(sizeof(bite_iterator*));
    iter->mass = mass;
    iter->position = 0;
    iter->bit = readBit(mass, 0);
    return iter;
}

bite_iterator* end(bite_massive* mass){
    bite_iterator* iter = (bite_iterator*) malloc(sizeof(bite_iterator*));
    iter->mass = mass;
    iter->position = mass->last;
    iter->bit = readBit(mass, mass->last);
    return iter;
}
bite_iterator* inc(bite_iterator* iter){
    if (iter->position < iter->mass->last){
        iter->position ++;
        iter->bit = readBit(iter->mass, iter->position);
        return iter;
    }else{
        return NULL;
    }
}
bite_iterator* dec(bite_iterator* iter){
    if (iter->position == 0){
        return NULL;
    }else{
        iter->position --;
        iter->bit = readBit(iter->mass, iter->position);
        return iter;
    }
}
char isEqual(bite_iterator* iter1, bite_iterator* iter2){
    if (iter1->mass == iter2->mass){
        if (iter1->position == iter2->position){
            return 1;
        }
    }
    return 0;
}

//sugar
bite_massive* massOr(bite_massive* mass1, bite_massive* mass2){
    if (mass1->last > mass2->last){
        for (int i = 0; i < mass2->last; i ++){
            writeBit(mass1, i, readBit(mass1, i)|readBit(mass2, i));
        }
    } else {
        for (int i = 0; i < mass1->last; i ++){
            writeBit(mass1, i, readBit(mass1, i)|readBit(mass2, i));
        }
    }
    return mass1;
}
bite_massive* massAnd(bite_massive* mass1, bite_massive* mass2){
    if (mass1->last > mass2->last){
        for (int i = 0; i < mass2->last; i ++){
            writeBit(mass1, i, readBit(mass1, i)&readBit(mass2, i));
        }
    } else {
        for (int i = 0; i < mass1->last; i ++){
            writeBit(mass1, i, readBit(mass1, i)&readBit(mass2, i));
        }
    }
    return mass1;
}
bite_massive* massLeft(bite_massive* mass1, unsigned int shift){
    // if (shift < mass1)
    for (int i = shift; i < mass1->last+1; i ++){
        writeBit(mass1, i - shift, readBit(mass1, i));
    }
    for (int i = mass1->last - shift+1;i < mass1->last+1; i++){
        if (i < 0)
            i = 0;
        writeBit(mass1, i, 0);
    }
    return mass1;
}
bite_massive* massRight(bite_massive* mass1, unsigned int shift){
    for (int i = shift; i < mass1->last+1; i ++){
        writeBit(mass1, i - shift, readBit(mass1, i));
    }
    for (int i = mass1->last - shift+1;i < mass1->last+1; i++){
        if (i < 0)
            i = 0;
        writeBit(mass1, i, 0);
    }
    return mass1;
}
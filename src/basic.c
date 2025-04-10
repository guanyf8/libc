#include "basic.h"

int numcmp(union key_type a,union key_type b){
    return a.num-b.num;
}

int mystrcmp(union key_type a,union key_type b){
    return strcmp(a.str,b.str);
}
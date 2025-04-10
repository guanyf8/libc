//
// Created by guan on 2025/3/15.
//

#ifndef PARALLELLJ_HASHTABLE_H
#define PARALLELLJ_HASHTABLE_H
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <string.h>
#include "basic.h"

typedef struct tUnit{
    union key_type key;
    void* value;
} tableUnit;

typedef struct hashTable{
    tableUnit* table;
    int size;   
    int used;
} hashTable;

hashTable* hashInit(int);

int hashInsert(hashTable * table,tableUnit unit) ;

int hashGetBin(hashTable* table,union key_type key);

void* hashGet(hashTable* table,union key_type key);

int hashErase(hashTable* table,union key_type key);

void hashFree(hashTable* table);

void hashTraverse(hashTable* table);
#endif //PARALLELLJ_HASHTABLE_H

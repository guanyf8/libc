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
    // int hash1,hash2;
    union key_type key;
    void* value;
} tableUnit;

typedef struct hashTable{
    tableUnit* table;
    int size;   
    int used;
    int (*hash1)(union key_type key);
    int (*hash2)(union key_type key);
    int (*cmp)(union key_type a, union key_type b);
} hashTable;

#define hashInit(table,exponent) hashInitFunction(table,exponent,NULL,NULL,NULL)

hashTable* hashInitFunction(hashTable* table,int exponent,
                    int (*hash1)(union key_type),
                    int (*hash2)(union key_type),
                    int (*cmp)(union key_type,union key_type));

#define hashInsert(table,key,value) hashInsertUnit(table,(tableUnit){key,value})

int hashInsertUnit(hashTable * table,tableUnit unit) ;

int hashGetBin(hashTable* table,union key_type key);

void* hashGet(hashTable* table,union key_type key);

int hashErase(hashTable* table,union key_type key);

void hashFree(hashTable* table);

void hashTraverse(hashTable* table);

#endif //PARALLELLJ_HASHTABLE_H

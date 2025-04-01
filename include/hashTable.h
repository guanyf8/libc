//
// Created by guan on 2025/3/15.
//

#ifndef PARALLELLJ_HASHTABLE_H
#define PARALLELLJ_HASHTABLE_H
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define tableSize 128

typedef struct tUnit{
    int key;
    void* value;
} tableUnit;

typedef tableUnit hashMap;

hashMap* hashInit(hashMap *table);

int hashInsert(hashMap * table,tableUnit unit) ;

int hashSearch(hashMap* table,int key);

void* hashGet(hashMap* table,int key);

int hashErase(hashMap* table,int key);
#endif //PARALLELLJ_HASHTABLE_H

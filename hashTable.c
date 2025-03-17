//
// Created by guan on 2025/3/15.
//

#include <string.h>
#include "hashTable.h"

static inline int H1(int key) {
    return key & 0b1111111; // 取低7位（0~127）
}

static inline int H2(int key) {
    return ((key & 0b111111000000) >> 6) | 1;     //取中间6位并强制为1
}

hashMap* hashInit(){
    hashMap * table=(hashMap*) malloc(sizeof(tableUnit)*tableSize);
    int i;
    memset((void*)table,0xFF,tableSize*sizeof(tableUnit));
    return table;
}

int hashInsert(hashMap * table,tableUnit unit) {
    int first, jump, bin, cnt = 0,key=unit.key;
    first = H1(key);
    bin = first;

    // 检查初始桶是否为空
    if (table[bin].key == -1) {
        table[bin]=unit;
        return bin;
    }

    // 冲突处理：最多探测 tableSize 次
    jump = H2(key);
    while (cnt < tableSize) {
        cnt++;
        bin = (first + cnt * jump) % tableSize; // 计算新位置

        if (table[bin].key == -1) {
            table[bin]=unit;
            return bin;
        }
    }

    return -1; // 表已满
}

//return bin or -1 if not found
int hashSearch(hashMap* table,int key){
    int first, jump, bin, cnt = 0;
    first = H1(key);
    bin = first;

    if (table[bin].key == key) {
        return bin;
    }

    // 冲突处理
    jump = H2(key);
    while (cnt < tableSize) {
        cnt++;
        bin = (first + cnt * jump) % tableSize; // 计算新位置

        if (table[bin].key == key) {
            return bin;
        }
    }

    return -1;
}

void* hashGet(hashMap* table,int key){
    int bin= hashSearch(table,key);
    if(bin!=-1)
        return table[bin].value;
    else
        return NULL;
}

int hashErase(hashMap* table,int key){
    int bin= hashSearch(table,key);
    if(bin!=-1){
        table[bin].key=-1;
        return 1;
    }
    else
        return 0;
}

//
// Created by guan on 2025/3/15.
//


#include "hashTable.h"
#include "stack.h"


int collasp=0;
static inline int H1(int shift, long long key) {
    return key & ~(0xffffffff<<shift);
}


static inline int H2(long long key) {
    return ((key & 0xf0) >> 6) | 1;     //取中间6位为并强制最低位1作为步长
}

hashTable* hashInit(int exponent){
    int i,size;
    if(exponent==0)
        size=2<<6;
    else if(exponent>10)
        size=2<<9;
    else
        size=1<<exponent;
    hashTable* hashtable=new(hashTable);
    tableUnit* table=new_array(tableUnit,size);
    memset((void*)table,0xff,size*sizeof(tableUnit));
    hashtable->table=table;
    hashtable->size=exponent;
    hashtable->used=0;
    return hashtable;
}


static inline int reHash(tableUnit* src,int src_shift,tableUnit* dst,short current_shift){
    int i=0,src_size=1<<src_shift;
    for(;i<src_size;i++){
        if(*(long long*)&src[i].key == -1)
            continue;
        union key_type key=src[i].key;
        int bin=H1(current_shift,*(long long*)&key);
        

        if (*(long long*)&dst[bin].key == -1) {
            dst[bin]=src[i];
            continue;
        }

        int jump = H2(*(long long*)&key);
        int size=(1<<current_shift);
        int cnt=0;
        for(;cnt < size;cnt++) {
            bin = (bin + jump) % size; 

            if (*(long long*)&dst[bin].key == -1) {
                dst[bin]=src[i];
                break;
            }
        }
    }
}

//flag +1扩容，-1缩容
static void hashResize(hashTable* t,short flag){

    tableUnit* src=t->table,*dst;
    t->size += flag;
    if(flag==1){
        dst=new_array(tableUnit,(1<<t->size));
        memset((void*)dst,0xff,sizeof(tableUnit)*(1<<(t->size)));
        reHash(src,t->size-1,dst,t->size);
        t->table=dst;
        free(src);
    }else if(flag==-1){
        dst=src;
        src+=(1<<t->size);
        reHash(src,t->size,dst,t->size);
        t->table=realloc(t->table,(1<<t->size)*sizeof(tableUnit));
    }
}

int hashInsert(hashTable * t,tableUnit unit) {
    tableUnit* table=t->table;
    int first, jump, bin, cnt = 0;
    long long key=*(long long*)&unit.key;
    first = H1(t->size,key);
    bin = first;

    // 检查初始桶是否为空
    if (*(long long*)&table[bin].key == -1) {
        table[bin]=unit;
        t->used++;
        goto success;
    }
    if(*(long long*)&table[bin].key == key){
        return -1;
    }
    // 冲突处理：最多探测 tableSize 次
    jump = H2(key);
    int size=(1<<t->size);
    for(;cnt < size;cnt++) {
        collasp++;
        bin = (bin + jump) % size;  // 计算新位置

        if (*(long long*)&table[bin].key == -1) {
            table[bin]=unit;
            t->used++;
            break;
        }
        if(*(long long*)&table[bin].key == key){
            return -1;
        }
    }

success:
    //扩容检查
    if((float)t->used/(1<<t->size)>0.7){
        hashResize(t,1);
    }
    return bin; 
}

//return bin or -1 if not found
int hashGetBin(hashTable* t,union key_type _key){
    tableUnit* table=t->table;
    int jump, bin, cnt = 0;
    long long key=*(long long*)&_key;
    bin = H1(t->size,key);

    if (*(long long*)&table[bin].key == key) {
        return bin;
    }

    // 冲突处理
    jump = H2(key);
    int size=(1<<t->size);
    for (cnt=0;cnt < size;cnt++) {
        collasp++;
        bin = (bin + jump) % size; // 计算新位置

        if (*(long long*)&table[bin].key == key) {
            return bin;
        }
    }

    return -1;
}

void* hashGet(hashTable* t,union key_type key){
    tableUnit* table=t->table;
    int bin= hashGetBin(t,key);
    if(bin!=-1)
        return table[bin].value;
    else
        return NULL;
}


int hashErase(hashTable* t,union key_type key){
    tableUnit* table=t->table;
    int bin= hashGetBin(t,key);
    if(bin!=-1){
        *(long long*)&table[bin].key=-1;
        t->used--;
        //缩容
        if((float)t->used/(1<<t->size)<0.1){
            hashResize(t,-1);
        }
        return 1;
    }
    else
        return 0;
}


void hashTraverse(hashTable* t){
    for(int i=0;i<(1<<t->size);i++){
        if(*(long long*)&t->table[i].key!=-1)
            printf("%d ",*(int*)&t->table[i].key);
        else
            printf("-1 ");
    }
}

void hashFree(hashTable* t){
    free(t->table);
    free(t);
}   
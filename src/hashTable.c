//
// Created by guan on 2025/3/15.
//


#include "hashTable.h"
#include "stack.h"
static inline int H1(union key_type key) {
    return key.num ;
}


static inline int H2(union key_type key) {
    return ((key.num & 0xf0) >> 6) | 1;     //取中间6位为并强制最低位1作为步长
}

static inline int cmpdefault(union key_type a,union key_type b){
    return !memcmp(&a,&b,sizeof(union key_type));
}

hashTable* hashInitFunction(hashTable* hashtable,int exponent,int (*hash1)(union key_type),
                     int (*hash2)(union key_type),int (*cmp)(union key_type,union key_type)){
    int i,size;
    if(exponent==0)
        size=2<<6;
    else if(exponent>10)
        size=2<<9;
    else
        size=1<<exponent;
    tableUnit* table=new_array(tableUnit,size);
    memset((void*)table,0xff,size*sizeof(tableUnit));
    hashtable->table=table;
    hashtable->size=exponent;
    hashtable->used=0;
    hashtable->hash1=hash1==NULL?H1:hash1;
    hashtable->hash2=hash2==NULL?H2:hash2;
    hashtable->cmp=cmp==NULL?cmpdefault:cmp;
    return hashtable;
}


static inline int reHash(tableUnit* src,int src_shift,tableUnit* dst,short current_shift,
                        int (*hash1)(union key_type),
                        int (*hash2)(union key_type),
                        int (*cmp)(union key_type,union key_type)){
    int i=0,src_size=1<<src_shift;
    for(;i<src_size;i++){
        if(cmp(src[i].key,(union key_type){.str=(char*)-1}))
            continue;
        union key_type key=src[i].key;
        int bin=hash1(key) & ~(0xffffffff<<current_shift);
        

        if (cmp(dst[i].key,(union key_type){.str=(char*)-1})) {
            dst[bin]=src[i];
            continue;
        }

        int jump = hash2(key);
        int size= (1<<current_shift);
        int cnt=0;
        for(;cnt < size;cnt++) {
            bin = (bin + jump) % size; 

            if (cmp(dst[i].key,(union key_type){.str=(char*)-1})) {
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
        reHash(src,t->size-1,dst,t->size,t->hash1,t->hash2,t->cmp);
        t->table=dst;
        free(src);
    }else if(flag==-1){
        dst=src;
        src+=(1<<t->size);
        reHash(src,t->size,dst,t->size,t->hash1,t->hash2,t->cmp);
        t->table=realloc(t->table,(1<<t->size)*sizeof(tableUnit));
    }
}

int hashInsertUnit(hashTable * t,tableUnit unit) {
    tableUnit* table=t->table;
    int  jump, bin, cnt = 0;
    union key_type key=unit.key;
    bin = t->hash1(key) & ~(0xffffffff<<t->size);

    // 检查初始桶是否为空
    if (t->cmp(table[bin].key,(union key_type){.str=(char*)-1})) {
        table[bin]=unit;
        t->used++;
        goto success;
    }else if(t->cmp(table[bin].key,key)){
        return -1;
    }
    // 冲突处理：最多探测 tableSize 次
    jump = t->hash2(key);
    int size=(1<<t->size);
    for(;cnt < size;cnt++) {
        bin = (bin + jump) % size;  // 计算新位置

        if (t->cmp(table[bin].key,(union key_type){.str=(char*)-1})) {
            table[bin]=unit;
            t->used++;
            break;
        }else if(t->cmp(table[bin].key,key)){
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
    union key_type key=_key;
    bin = t->hash1(key) & ~(0xffffffff<<t->size);

    if (t->cmp(table[bin].key,key)) {
        return bin;
    }

    // 冲突处理
    jump = t->hash2(key);
    int size=(1<<t->size);
    for (cnt=0;cnt < size;cnt++) {
        bin = (bin + jump) % size; // 计算新位置

        if (t->cmp(table[bin].key,key)) {
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
        table[bin].key=(union key_type){.str=(char*)-1};
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
        if(t->cmp(t->table[i].key,(union key_type){.str=(char*)-1}))
            printf("-1 ");    
        else
            printf("%d ",*(int*)&t->table[i].key);
    }
}

void hashFree(hashTable* t){
    free(t->table);
}   
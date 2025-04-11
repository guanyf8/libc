#include<stdio.h>
#include<pthread.h>
#include<time.h>



int main(void){
    pthread_rwlock_t lock=PTHREAD_RWLOCK_INITIALIZER;
    int i,j=0;
    volatile int a=0;
    int start,end;

    start=clock();
    for(i=0;i<100000000;i++){
        j++;
    }
    end=clock();
    printf("a++ , time:%d\n",end-start);


    start=clock();
    for(i=0;i<100000000;i++){
        a++;
    }
    end=clock();
    printf("volatile a++, time:%d\n",end-start);

    a=0;
    start=clock();
    for(i=0;i<100000000;i++){
        pthread_rwlock_wrlock(&lock);
        a++;
        pthread_rwlock_unlock(&lock);
    }
    end=clock();
    printf("volatile a++, wrlock&unlock, time:%d\n",end-start);

    a=0;
    start=clock();
    for(i=0;i<100000000;i++){
        pthread_rwlock_rdlock(&lock);
        a++;
        pthread_rwlock_unlock(&lock);
    }
    end=clock();
    printf("volatile a++, rdlock&unlock, time:%d\n",end-start);


}
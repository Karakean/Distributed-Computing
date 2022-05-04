#define MAX_NUMBERS 100000
#define MAX_PRIMES_NUM 10000
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_data{
    int num;
    int end;
};

int numbers[MAX_NUMBERS] = { [0 ... MAX_NUMBERS-1] = 1};
int primes[MAX_PRIMES_NUM];
int primes_counter = 0;
pthread_mutex_t mut;

void *thread_function(void *data){
    struct thread_data* td = (struct thread_data*)data;
    if (numbers[td->num] == 0){
        return NULL;
    }
    pthread_mutex_lock(&mut);
    primes[primes_counter] = td->num;
    primes_counter++;
    pthread_mutex_unlock(&mut);
    for(int i = td->num*td->num; i < td->end; i+=td->num){
        numbers[i] = 0;
    }
    return NULL;
}

int main(){   
    int beg, end;
    printf("Interval begin: ");
    scanf("%d", &beg);
    printf("Interval end: ");
    scanf("%d", &end);

    struct thread_data** data = (struct thread_data**)malloc((end/2)*sizeof(struct thread_data));
    pthread_t *threads = (pthread_t*)malloc(end*sizeof(pthread_t));
    pthread_mutex_init(&mut, NULL);
    int i = 2;
    for(i; i*i <= end; i++){
        data[i-2] = (struct thread_data*)malloc(sizeof(struct thread_data));
        data[i-2]->num = i;
        data[i-2]->end = end;
        pthread_create(&threads[i-2], NULL, thread_function, data[i-2]);
    }
    for(int j = 2; j <= i; j++){
        pthread_join(threads[i-2], NULL);
    }

    for(i; i < end; i++){
        if(numbers[i]==1){
            primes[primes_counter] = i;
            primes_counter++;
        }
    }

    i = 0;
    while(primes[i] < beg)
        ++i;
    for(i; i < primes_counter; i++)
        printf("%d ", primes[i]);
    printf("\n");

    for (int j = 0; j < end/2; j++)
        free(data[j]);
    free(data);
    free(threads);
    pthread_mutex_destroy(&mut);
    return 0;
}
#include <stdio.h>
#include <pthread.h>

// global variable
float mean;
int min;
int max;

// store pointer of first element and last element
struct arr_data{
    int *first_p;
    int *last_p;
};

void* thread_mean(void* thread_arg){
    struct arr_data *arg;
    arg = (struct arr_data*) thread_arg;
    int *fp = arg -> first_p;
    int *lp = arg -> last_p;
    int sum = 0;
    int arr_size = ((int)lp - (int)fp+4)/sizeof(int);
    
    for (int i=0; i < arr_size; i++){
        sum = sum + *(fp + i);
    }
    float avg = sum/(arr_size);
    printf("Thread-Mean: %.2f \n", avg);
    mean = avg;
}

void* thread_min(void* thread_arg){
    struct arr_data *arg;
    arg = (struct arr_data*) thread_arg;
    int *fp = arg -> first_p;
    int *lp = arg -> last_p;
    int *min_p = fp;
    int arr_size = ((int)lp - (int)fp+4)/sizeof(int);
    
    for (int i = 0; i < arr_size; i++){
        if(*(fp+i) < *min_p){
            min_p = (fp+i);
        }
    }
    printf("Thread-Min: %d \n", *min_p);
    min = *min_p;
}

void* thread_max(void* thread_arg){
    struct arr_data *arg;
    arg = (struct arr_data*) thread_arg;
    int *fp = arg -> first_p;
    int *lp = arg -> last_p;
    int *max_p = fp;
    int arr_size = ((int)lp - (int)fp + 4)/sizeof(int);
    
    for (int i=0; i < arr_size; i++){
        if(*(fp+i) > *max_p){
            max_p = (fp+i);
        }
    }
    printf("Thread-Max: %d \n", *max_p);
    max = *max_p;
}


void main(){

    // input array
    int arr_size;
    printf("Enter array size: ");
    scanf("%d", &arr_size);
    int arr[arr_size];
    printf("Enter array element (integer)\n");
    for(int i = 0; i < arr_size; i++){
        scanf("%d", &arr[i]);
    }

    int *fp = &arr[0];
    int *lp = &arr[arr_size-1];
    
    struct arr_data arr_data;
    arr_data.first_p = fp;
    arr_data.last_p = lp;

    // create 3 thread for mean, max, max
    pthread_t threads[3];

    // print input array
    printf("array: ");
    for(int i = 0; i < arr_size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");

    // create thread for calculate mean, min, max
    pthread_create(&threads[0], NULL, thread_mean, &arr_data);
    pthread_create(&threads[1], NULL, thread_min, &arr_data);
    pthread_create(&threads[2], NULL, thread_max, &arr_data);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    printf("The avg value is: %.2f\n", mean);
    printf("The min value is: %d\n", min);
    printf("The maxs value is: %d\n", max);
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


// Store argument data for passing to thread
struct Thread_subarray_data{
    int thread_id;
    int *first_p;
    int *last_p;
};

struct Thread_merge_data{
    int thread_id;
    int *left;
    int *mid;
    int *right;
};

struct timeval start_time, end_time;

// Maximun print element
#define Max_size 25

/* ------------------------ Merge Sort ------------------------*/
void merge(int a[], int left, int mid, int right){
    int i, j, k;
    int ls = mid - left + 1;
    int rs = right - mid;

    // Create temp array in heap
    int *L = (int *)malloc(sizeof(int) * ls);
    int *R = (int *)malloc(sizeof(int) * rs); 

    // Copy data to temp array
    for (i = 0; i < ls; i++){
        L[i] = a[left+i];
    }
    for (j = 0; j < rs; j++){
        R[j] = a[mid+j+1];
    }

    // Merge temp array to array a[l..r]
    i = 0; 
    j = 0; 
    k = left;
    while (i < ls && j < rs){
        if (L[i] <= R[j]){
            a[k] = L[i];
            i++;
        }
        else{
            a[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < ls){
        a[k] = L[i];
        k++; i++;
    }
    while (j < rs){
        a[k] = R[j];
        k++; j++;
    }
    free(L); free(R);
}

void mergesort(int a[], int left, int right){
    // If array contains more than 1 element recursive call...
    if (left < right){
        int mid = (left+right)/2;
        mergesort(a, left, mid);
        mergesort(a, mid+1, right);
        merge(a, left, mid, right);
    }
}

/* ------------------------ Thread mergesort ------------------------*/
void* thread_mergesort(void* thread_arg){
    struct Thread_subarray_data *arg;
    arg = (struct Thread_subarray_data*) thread_arg;

    int thread_id;
    int *fp;
    int *lp;
    thread_id = arg -> thread_id;
    fp = arg -> first_p;
    lp = arg -> last_p;

    int temp_size = ((int)lp-(int)fp+4)/4;
    int *temp_array = (int *)malloc(sizeof(int) * temp_size);
    /*
        Copy array element to temp_array
        Mergesort(temp_array)
        Copy temp_array element to array
    */
    for (int i = 0; i < temp_size; i++){
        temp_array[i] = *(fp + i);
    }
    mergesort(temp_array, 0, temp_size-1);
    for (int i = 0; i < temp_size; i++){
        *(fp + i) = temp_array[i];
    }
    free(temp_array);
}


/* ------------------------ Thread merge ------------------------*/
void* thread_merge(void* thread_arg){
    struct Thread_merge_data *arg;
    arg = (struct Thread_merge_data*) thread_arg;
    int thread_id = arg -> thread_id;
    int *left_p = arg -> left;
    int *mid_p = arg -> mid;
    int *right_p = arg -> right;

    int temp_size = ((int)right_p-(int)left_p+4)/4;
    int *temp_array = (int *)malloc(sizeof(int) * temp_size); 
    int mid = ((int)mid_p-(int)left_p)/4;
    /*
        Copy element in array to temp_array
        Merge(temp_array)
        Copy element in temp_array to array
    */
    for (int i = 0; i < temp_size; i++){
        temp_array[i] = *(left_p + i);
    }
    merge(temp_array, 0, mid, temp_size-1);
    for (int i = 0; i < temp_size; i++){
        *(left_p + i) = temp_array[i];
    }
    free(temp_array);
}

/* ------------------------ Recursive merge sub-array ------------------------*/
void recursive_merge(int a[], int arr_size, int p, struct Thread_subarray_data subarr_data[]){
    // If partintion of array > 1
    if (p > 1){ 
        int merged_size = (arr_size/p)*2;         // size of array after merged
        int num_thread = p/2;                     // number of thread for merge sub array
        pthread_t threads[num_thread];

        // marge_datastore data for merge(a, l, m, r)
        struct Thread_merge_data marge_data[num_thread];
        // merged data for recursive call
        struct Thread_subarray_data merged_arr_data[num_thread];

        /*
            Create p/2-thread for merge p-sub_array
        */
        for (int i = 0; i < num_thread; i++){
            marge_data[i].thread_id = i;
            marge_data[i].left = subarr_data[2*i].first_p;
            marge_data[i].mid = subarr_data[2*i].last_p;
            marge_data[i].right = subarr_data[(2*i)+1].last_p;

            merged_arr_data[i].first_p = subarr_data[2*i].first_p;
            merged_arr_data[i].last_p = subarr_data[(2*i)+1].last_p;
            pthread_create(&threads[i], NULL, thread_merge, &marge_data[i]);
        }
        for (int i = 0; i < num_thread; i++){
            pthread_join(threads[i], NULL);
        }
        recursive_merge(a, arr_size, p/2, merged_arr_data);
    }
}


void printArray(int a[], int size){
    int i = 0;
    for (i; i < size; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

/* ------------------------ Main ------------------------*/
int main(){

    printf("--------------- Multi-thread Merge sort ---------------\n");

    printf("Enter Thread (2^n): ");
    int num_thread;
    scanf("%d", &num_thread);

    // Validate Number of thread
    if (!(num_thread && !(num_thread & (num_thread - 1)))){
        printf("Number of thread should be 2^n\n");
        main();
    }

    printf("Enter array size: ");
    int array_size;
    scanf("%d", &array_size);

    printf("Enter data type 1=dec, 2=inc, 3=ran: ");
    int data_type;
    scanf("%d", &data_type);
    if (data_type > 3 || data_type < 1){
        printf("data type should be 1, 2, 3\n");
        main();
    }

    printf("--------------------------------------\n");

    pthread_t threads[num_thread];
    struct Thread_subarray_data subarr_data[num_thread];


    char File_Name[3][30] = {"decrease_int.txt", "increase_int.txt", "random_int.txt"};
    FILE *data_file;
    data_file = fopen(("%s", File_Name[data_type-1]), "r");
    int *array = (int *)malloc(sizeof(int) * array_size);

    for (int i = 0; i < array_size; i++){
        fscanf(data_file, "%d", &array[i]);
    } 

    // Print array before sort
    if(array_size > Max_size){
        printArray(array, Max_size);
    }else{
        printArray(array, array_size);
    }


    int *p;     // first array element pointer
    p = array;
    int element_per_thread = array_size/num_thread;
    printf("array of %d element, %d element per thread\n", array_size, element_per_thread);
    /*
       --Start timer
        Create Thread, mergesort(sub-array)
    */
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < num_thread; i++){
        if (i!=num_thread-1){
            subarr_data[i].thread_id = i;
            subarr_data[i].first_p = p + (element_per_thread*i);
            subarr_data[i].last_p = p + (element_per_thread*i) + (element_per_thread-1);
            pthread_create(&threads[i], NULL, thread_mergesort, &subarr_data[i]);
        }
        // last thread
        else{
            subarr_data[i].thread_id = i;
            subarr_data[i].first_p = p + (element_per_thread*i);
            subarr_data[i].last_p = &array[array_size-1];
            pthread_create(&threads[i], NULL, thread_mergesort, &subarr_data[i]);
        }
    }
    for (int i = 0; i < num_thread; i++){
        pthread_join(threads[i], NULL);
    }
    /*
        After each thread completed mergesort(sub-array)
        merge each sub-array together
        --Stop timer
    */
    recursive_merge(array, array_size, num_thread, subarr_data);
    gettimeofday(&end_time, NULL);

    // Print array after sort
    if(array_size > Max_size){
        printArray(array, Max_size);
    }else{
        printArray(array, array_size);
    }

    float t_ms = ((float)(end_time.tv_sec - start_time.tv_sec) * 1000) + ((float) (end_time.tv_usec - start_time.tv_usec)/1000);
    printf(">> Time: %*.3f ms\n", 15, t_ms);

    free(array);
    main();
}
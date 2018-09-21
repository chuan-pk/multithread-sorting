#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

// Maximun print element
#define Max_size 15

struct timeval start_time, end_time;

void printArray(int a[], int array_size){
    int i;
    for (i = 0; i < array_size; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}


// merge two sub-array
void merge(int a[], int left, int mid, int right){
    int i, j, k;

    // Size of left and right temp array
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


int main(){
    printf("--------------- Single-thread Merge sort ---------------\n");

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

    char File_Name[3][30] = {"decrease_int.txt", "increase_int.txt", "random_int.txt"};
    FILE *data_file;
    data_file = fopen(("%s", File_Name[data_type-1]), "r");
    int *array = (int *)malloc(sizeof(int) * array_size);
    for (int i = 0; i < array_size; i++){
        fscanf(data_file, "%d", &array[i]);
    }

    // Print array before sorting
    if(array_size > Max_size){
        printArray(array, Max_size);
    }
    else{
        printArray(array, array_size);
    }

    /*
        Start timer
            Merge sort
        Stop timer
    */
    gettimeofday(&start_time, NULL);
    mergesort(array, 0, array_size-1);
    gettimeofday(&end_time, NULL);

    // Print array after Mergesort
    if(array_size > Max_size){
        printArray(array, Max_size);
    }
    else{
        printArray(array, array_size);
    }

    float t_ms = ((float)(end_time.tv_sec - start_time.tv_sec) * 1000) + ((float) (end_time.tv_usec - start_time.tv_usec)/1000);
    printf(">> Time: %*.3f ms\n", 15, t_ms);

    free(array);
    main();
    return 0;
}

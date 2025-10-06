#include <stdio.h>
#include <stdlib.h>

void bubbleSort(int a[], int array_size){
        int i, j, temp;
        for (i = 0; i < (array_size - 1); ++i){
            for (j = 0; j < array_size - 1 - i; ++j ){
                if (a[j] > a[j+1]){
                    temp = a[j+1];
                    a[j+1] = a[j];
                    a[j] = temp;
                }
            }
        }
}

int main(int argc, char* args[]){
    int nums[3];
    
    for (int i=1; i<=3; i++) {
        nums[i-1] = atoi(args[i]);
    }
    
    bubbleSort(nums, 3);

    printf("min-%d, max-%d", nums[0], nums[2]);

    return 0;
}

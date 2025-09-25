#include<stdio.h>
#include<limits.h>
#include<stdbool.h>
void swap(int *a, int *b) {
    /*
    Function to swap two integers.

    Parameters:
    a - pointer to the first integer
    b - pointer to the second integer

    time complexity: O(1)
    space complexity: O(1)
    */
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bulltle_sort(int arr[], int n) {
    /*
    Function to sort an array using bubble sort algorithm.

    Parameters:
    arr - array of integers
    n - number of elements in the array

    time complexity: O(n^2)
    space complexity: O(1)
    */
    bool change = true;
    for(int i = n-1; i >=1 && change; i--) {
        change = false;
        for(int j = 0; j < i; j++) {
            if(arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
                change = true;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d integers(between %d and %d):\n", n, INT_MIN, INT_MAX);
    for(int i = 0; i < n; i++) {
        long long temp;
        scanf("%lld", &temp);
        if (temp > INT_MAX || temp < INT_MIN) {
            printf("Error: Input value %lld is out of int range.\n", temp);
            return -1;
        }
        arr[i] = (int)temp;
    }
    bulltle_sort(arr, n);

    printf("Sorted array: \n");
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
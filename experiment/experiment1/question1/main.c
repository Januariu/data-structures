#include<stdio.h>
#include<limits.h>

int find_array_max(int arr[], int n) {
    /*
    Function to find the maximum value in an array.

    Parameters:
    arr - array of integers
    n - number of elements in the array

    Returns:
    Maximum integer value in the array

    time complexity: O(n)
    space complexity: O(1)
    */
    int max = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int find_array_min(int arr[], int n) {
    /*
    Function to find the minimum value in an array.

    Parameters:
    arr - array of integers
    n - number of elements in the array

    Returns:
    Minimum integer value in the array

    time complexity: O(n)
    space complexity: O(1)
    */
    int min = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
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
    int max_value = find_array_max(arr, n);
    int min_value = find_array_min(arr, n);
    printf("The maximum value is: %d\n", max_value);
    printf("The minimum value is: %d\n", min_value);
    return 0;
}
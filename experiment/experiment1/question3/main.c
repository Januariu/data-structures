#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void){
    int n;
    printf("input the degree of polynomial:");
    scanf("%d",&n);
    double arr[n+1];
    // Initialize array elements to 0
    for(int i=0;i<=n;i++){
        arr[i]=0;
    }
    // Input degree and coefficients
    printf("input degree and the coefficients of term corresponding to degree (input -1 to end):\n");
    for(int i=0;i<=n;i++){
        long degree;
        double coefficient;
        scanf("%ld %lf",&degree,&coefficient);
        if(degree==-1 && coefficient==-1){
            break;
        }
        if(degree<0 || degree>n){
            printf("Error: Input degree %ld is out of range.\n",degree);
            return -1;
        }
        if(coefficient>DBL_MAX || coefficient< -DBL_MAX){
            printf("Error: Input coefficient %lf is out of double range.\n",coefficient);
            return -1;
        }
        arr[degree]=coefficient;
    }
    // Input x0
    printf("input x0(between %d and %d):", INT_MIN, INT_MAX);
    long long x0;
    scanf("%lld",&x0);
    if(x0>INT_MAX || x0<INT_MIN){
        printf("Error: Input value %lld is out of int range.\n",x0);
        return -1;
    }
    // Calculate polynomial value using Horner's method
    double result=0;
    for(int i=n;i>=0;i--){
        result = result * x0 + arr[i];
    }
    printf("P(%lld) = %lf\n", x0, result);
    return 0;
}
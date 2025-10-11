// base on dynamic array to implement set union intersection and complement
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct set{
    double *data;
    int size;
    int _capacity;
}*set;

set init_set(int capacity){
    set s = (set)malloc(sizeof(struct set));
    s->data = (double*)malloc(sizeof(double) * capacity);
    s->size = 0;
    s->_capacity = capacity;
    return s;
}

void free_set(set s){
    free(s->data);
    free(s);
}

void print_set(set s){
    if(s->size == 0){
        printf("Set is empty.\n");
        return;
    }
    printf("{ ");
    for(int i = 0; i < s->size; i++){
        printf("%lf ", s->data[i]);
    }
    printf("}\n");
}

bool is_contains(set s, double value){
    for(int i = 0; i < s->size; i++){
        if(s->data[i] == value) return true;
    }
    return false;
}

void resize(set s){
    s->_capacity *= 2;
    s->data = (double*)realloc(s->data, sizeof(double) * s->_capacity);
    if(s->data == NULL){
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
}

void insert_to_set(set s,double element){
    if(is_contains(s, element)) return;
    if(s->size == s->_capacity) resize(s);
    s->data[s->size++] = element;
    return;
}

bool union_set(set s1, set s2, set result){
    for(int i = 0; i < s1->size; i++){
        insert_to_set(result, s1->data[i]);
    }
    for(int i = 0; i < s2->size; i++){
        insert_to_set(result, s2->data[i]);
    }
    return true;
}

bool intersection_set(set s1, set s2, set result){
    for(int i = 0; i < s1->size; i++){
        if(is_contains(s2, s1->data[i])){
            insert_to_set(result, s1->data[i]);
        }
    }
    return true;
}

bool complement_set(set s_U, set s2, set result){
    if(s_U->size < s2->size) {
        printf("Universal set must be larger than the set to complement.\n");
        return false;
    }
        for(int i = 0; i < s_U->size; i++){
        if(!is_contains(s2, s_U->data[i])){
            insert_to_set(result, s_U->data[i]);
        }
    }
    return true;
}

bool input_set(set s){
    printf("Enter elements of the set, end is -1\n");
    double element;
    while(1){
        scanf("%lf", &element);
        if(element == -1) break;
        insert_to_set(s, element);
        printf("Current set: ");
        print_set(s);
    }
    return true;
}

int main(void){
    set A, B, U;
    A = B = U = NULL;

    A = init_set(10);
    B = init_set(10);
    U = init_set(10);

    while(1){
        printf("1. Input Set A\n");
        printf("2. Input Set B\n");
        printf("3. Input Universal Set U\n");
        printf("4. Union A U B\n");
        printf("5. Intersection A ∩ B\n");
        printf("6. Complement of B in U (U - B)\n");
        printf("7.print set\n");
        printf("8. Exit\n");
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            {            
                input_set(A);
                break;
            }
        case 2:
            {
                input_set(B);
            }           
            break;
        case 3:
            {
                input_set(U);
            }
            break;
        case 4:{
            if(A == NULL || B == NULL){
                printf("Please input sets A and B first.\n");
                break;
            }
            set result = init_set(10);
            union_set(A, B, result);
            printf("A U B = ");
            print_set(result);
            free_set(result);
            break;
        }
        case 5:{
            if(A == NULL || B == NULL){
                printf("Please input sets A and B first.\n");
                break;
            }
            set result = init_set(10);
            intersection_set(A, B, result);
            printf("A ∩ B = ");
            print_set(result);
            free_set(result);
            break;
        }
        case 6:{
            if(U == NULL || B == NULL || A == NULL){
                printf("Please input sets U A and B first.\n");
                break;
            }
            set result = init_set(10);
            complement_set(U, A, result);
            printf("U - A = ");
            print_set(result);
            
            complement_set(U, B, result);
            printf("U - B = ");
            print_set(result);


            free_set(result);
            break;
        }

        case 7:
            {
                if(A) {
                    printf("Set A: ");
                    print_set(A);
                } else {
                    printf("Set A is not initialized.\n");
                }
                if(B) {
                    printf("Set B: ");
                    print_set(B);
                } else {
                    printf("Set B is not initialized.\n");
                }
                if(U) {
                    printf("Universal Set U: ");
                    print_set(U);
                } else {
                    printf("Universal Set U is not initialized.\n");
                }
            }
            break;
        case 8:
            if(A) free_set(A);
            if(B) free_set(B);
            if(U) free_set(U);
            exit(0);
            break;  
        
        
        default:
            printf("Invalid choice.\n");
            break;
        }
    }
    return 0;
}
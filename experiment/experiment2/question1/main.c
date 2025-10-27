// base on dynamic linked list to implement polynomial addition subtraction multiplication and division
#include<stdio.h>
#include<stdlib.h>

typedef struct polynode{
    int exp; // exponent
    double coef; // coefficient
    struct polynode *next; // next node
}polynode, *polyptr; 

polyptr init_poly(){
    polyptr head = (polyptr)malloc(sizeof(polynode));
    head->next = NULL;
    return head;
}

void free_poly(polyptr head){
    polyptr temp;
    while(head){
        temp = head;
        head = head->next;
        free(temp);
    }
}

void insert_term(polyptr head, int exp, double coef){
    if(coef == 0) return; // skip zero coefficient terms
    polyptr new_node = (polyptr)malloc(sizeof(polynode));
    if(new_node == NULL){
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_node->exp = exp;
    new_node->coef = coef;
    new_node->next = NULL;

    // Insert in descending order of exponent
    polyptr current = head;
    while(current->next != NULL && current->next->exp > exp){
        current = current->next;
    }
    if(current->next != NULL && current->next->exp == exp){
        // If term with same exponent exists, add coefficients
        current->next->coef += coef;
        if(current->next->coef == 0){
            // Remove term if coefficient becomes zero

            // polyptr temp = current->next;
            // current->next = temp->next;
            // free(temp);
            current->next = current->next->next;
        }
        free(new_node);
    } else {
        new_node->next = current->next;
        current->next = new_node;
    }
}

void delete_term(polyptr head, int exp){
    polyptr current = head;
    while(current->next != NULL && current->next->exp != exp){
        current->next = current->next->next;
    }
    if(current->next == NULL){
        printf("Term with exponent %d not found.\n", exp);
        return;
    }
    current->next = current->next->next;
    return;
}

void print_poly(polyptr head){
    polyptr current = head->next; // skip head node
    if(current == NULL) printf("0\n");

    while(current !=NULL){
        if(current->coef > 0 && current != head->next) printf("+");
        if(current->exp == 0){
            printf("%.2lf", current->coef);
        } else if(current->exp == 1){
            printf("%.2lfx", current->coef);
        } else {
            printf("%.2lfx^%d", current->coef, current->exp);
        }
        current = current->next;
    }
}

void add_poly(polyptr poly1, polyptr poly2, polyptr result){
    polyptr p1 = poly1->next; // skip head
    polyptr p2 = poly2->next; // skip head

    while(p1 != NULL && p2 != NULL){
        if(p1->exp > p2->exp){
            insert_term(result, p1->exp, p1->coef);
            p1 = p1->next;
        } else if(p1->exp < p2->exp){
            insert_term(result, p2->exp, p2->coef);
            p2 = p2->next;
        } else {
            double sum_coef = p1->coef + p2->coef;
            if(sum_coef != 0){
                insert_term(result, p1->exp, sum_coef);
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
}

void sub_poly(polyptr poly1, polyptr poly2, polyptr result){
    polyptr p1 = poly1->next; // skip head
    polyptr p2 = poly2->next; // skip head

    while(p1 != NULL && p2 != NULL){
        if(p1->exp > p2->exp){
            insert_term(result, p1->exp, p1->coef);
            p1 = p1->next;
        } else if(p1->exp < p2->exp){
            insert_term(result, p2->exp, -p2->coef);
            p2 = p2->next;
        } else {
            double diff_coef = p1->coef - p2->coef;
            if(diff_coef != 0){
                insert_term(result, p1->exp, diff_coef);
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
}

void mul_poly(polyptr poly1, polyptr poly2, polyptr result){
    polyptr p1 = poly1->next; // skip head
    polyptr p2 = poly2->next; // skip head

    while(p1 != NULL){
        p2 = poly2->next; // reset p2 for each term in p1
        while(p2 != NULL){
            int new_exp = p1->exp + p2->exp;
            double new_coef = p1->coef * p2->coef;
            insert_term(result, new_exp, new_coef);
            p2 = p2->next;
        }
        p1 = p1->next;
    }
}

void div_poly(polyptr poly1, polyptr poly2, polyptr quotient, polyptr remainder){
    // Polynomial division: poly1 / poly2 = quotient ... remainder
    int n1 = poly1->next->exp, n2 = poly2->next->exp;
    polyptr temp1 = poly1->next;
    polyptr temp2 = poly2->next;
    double poly1_coef[n1 + 1];
    double poly2_coef[n2 + 1];

    for(int i = 0; i <= n1; i++) poly1_coef[i] = 0;
    for(int i = 0; i <= n2; i++) poly2_coef[i] = 0;
    while(temp1 != NULL){
        poly1_coef[temp1->exp] = temp1->coef;
        temp1 = temp1->next;
    }
    while(temp2 != NULL){
        poly2_coef[temp2->exp] = temp2->coef;
        temp2 = temp2->next;
    }

    int deg_q = n1 - n2;
    double coef_q[deg_q + 1];
    for(int i = 0; i <= deg_q; i++) coef_q[i] = 0;
    for(int i = deg_q; i >= 0; i--){
        coef_q[i] = poly1_coef[i + n2] / poly2_coef[n2];
        for(int j = 0; j <= n2; j++){
            poly1_coef[i + j] -= coef_q[i] * poly2_coef[j];
        }
    }

    for(int i = deg_q; i >= 0; i--){
        if(coef_q[i] != 0)
            insert_term(quotient, i, coef_q[i]);
    }

    for(int i = n2 - 1; i >= 0; i--){
        if(poly1_coef[i] != 0)
            insert_term(remainder, i, poly1_coef[i]);
    }
}

void create_poly(polyptr head){
    int exp;
    double coef;
    printf("Enter terms (exp coef), end with -1 -1:\n");
    while(1){
        scanf("%d %lf", &exp, &coef);
        if(exp == -1 && coef == -1) break;
        if(exp < 0){
            printf("Exponent must be non-negative.\n");
            continue;
        }
        insert_term(head, exp, coef);
        print_poly(head);
        printf("\n");
    }
}

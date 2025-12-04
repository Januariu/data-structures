#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 4
typedef struct
{
    int x, y;
    double value;
} Triple; //(row, col, value)

typedef struct
{
    Triple *data;
    size_t length, rows, cols, capacity;
} *Matrix;

static int compare_triples(const void *a, const void *b)
{
    Triple *t1 = (Triple *)a;
    Triple *t2 = (Triple *)b;
    if (t1->x != t2->x)
        return t1->x - t2->x;
    return t1->y - t2->y;
}

Matrix init_matrix(int num_elem, int rows, int cols)
{
    Matrix A = (Matrix)malloc(sizeof(*A));

    A->data = (Triple *)malloc(sizeof(Triple) * (num_elem > INITIAL_CAPACITY ? num_elem : INITIAL_CAPACITY));
    if (A->data == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    A->length = num_elem;
    A->rows = rows;
    A->cols = cols;
    A->capacity = (num_elem > INITIAL_CAPACITY ? num_elem : INITIAL_CAPACITY);
    return A;
}

Matrix creat_from_array(double **arr, int rows, int cols)
{
    int count = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (arr[i][j] != 0.0)
                count++;

    Matrix A = init_matrix(count, rows, cols);
    int idx = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (arr[i][j] != 0.0)
            {
                A->data[idx].x = i;
                A->data[idx].y = j;
                A->data[idx].value = arr[i][j];
                idx++;
            }
    return A;
}

void free_matrix(Matrix *A)
{
    free((*A)->data);
    free(*A);
    *A = NULL;
}

void resize_matrix(Matrix A, size_t new_capacity)
{
    Triple *new_data = (Triple *)realloc(A->data, sizeof(Triple) * new_capacity);
    if (new_data == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(1);
    }
    A->data = new_data;
    A->capacity = new_capacity;
}

void input_matrix(Matrix A)
{
    printf("Enter matrix elements in format (row col value), one per line. Enter -1 -1 -1 to end input.\n");
    int r, c;
    double v;
    int count = 0;
    while (1)
    {
        scanf("%d %d %lf", &r, &c, &v);
        if (r == -1 && c == -1 && v == -1)
            break;
        if (r < 0 || r >= A->rows || c < 0 || c >= A->cols)
        {
            printf("Invalid row or column index. Please try again.\n");
            continue;
        }
        if (count >= A->capacity)
        {
            resize_matrix(A, A->capacity * 2);
        }
        A->data[count].x = r;
        A->data[count].y = c;
        A->data[count].value = v;
        count++;
    }
    A->length = count; // update size to actual number of elements

    qsort(A->data, A->length, sizeof(Triple), compare_triples);
}

void print_matrix(const Matrix A, bool print_full)
{
    if (print_full)
    {
        double mat[A->rows][A->cols];
        for (int i = 0; i < A->rows; i++)
            for (int j = 0; j < A->cols; j++)
                mat[i][j] = 0.0;
        for (int k = 0; k < A->length; k++)
            mat[(A->data[k].x)][(A->data[k].y)] = A->data[k].value;

        for (int i = 0; i < A->rows; i++)
        {
            for (int j = 0; j < A->cols; j++)
            {
                printf("%lf ", mat[i][j]);
            }
            printf("\n");
        }
    }
    else
    {
        printf("Sparse Matrix Representation (row col value):\n");
        for (int k = 0; k < A->length; k++)
        {
            printf("(%d %d %lf)\n", A->data[k].x, A->data[k].y, A->data[k].value);
        }
    }
}

void add_matrix(const Matrix A, const Matrix B, Matrix C)
{
    // union of sorted lists
    if (A->cols != B->cols || A->rows != B->rows)
    {
        printf("Matrix dimension mismatch\n");
        exit(1);
    }
    resize_matrix(C, A->length + B->length);
    size_t A_idx = 0, B_idx = 0, C_idx = 0;
    while (A_idx < A->length && B_idx < B->length)
    {
        Triple a = A->data[A_idx];
        Triple b = B->data[B_idx];
        if (compare_triples(&a, &b) < 0)
        {
            C->data[C_idx++] = a;
            A_idx++;
        }
        else if (compare_triples(&a, &b) > 0)
        {
            C->data[C_idx++] = b;
            B_idx++;
        }
        else
        {
            double summed_value = a.value + b.value;
            if (summed_value != 0.0)
            {
                C->data[C_idx].x = a.x;
                C->data[C_idx].y = a.y;
                C->data[C_idx].value = summed_value;
                C_idx++;
            }
            A_idx++;
            B_idx++;
        }
    }
    while (A_idx < A->length)
    {
        C->data[C_idx++] = A->data[A_idx++];
    }
    while (B_idx < B->length)
    {
        C->data[C_idx++] = B->data[B_idx++];
    }
    C->length = C_idx;
    resize_matrix(C, C_idx);
}

void transpose_matrix(const Matrix A, Matrix result)
{
    int col_elem_count[A->cols];
    for (int i = 0; i < A->cols; i++)
        col_elem_count[i] = 0;

    for (int i = 0; i < A->length; i++)
        col_elem_count[A->data[i].y]++;

    int col_start_index[A->cols];
    col_start_index[0] = 0;
    for (int i = 1; i < A->cols; i++)
        col_start_index[i] = col_start_index[i - 1] + col_elem_count[i - 1];

    resize_matrix(result, A->length);

    for (int i = 0; i < A->length; i++)
    {
        Triple elem = A->data[i];
        int pos = col_start_index[elem.y]++;
        result->data[pos].x = elem.y;
        result->data[pos].y = elem.x;
        result->data[pos].value = elem.value;
    }
    result->length = A->length;
    result->rows = A->cols;
    result->cols = A->rows;
}
/*
void multiply_matrix(const Matrix A, const Matrix B, Matrix C)
{
    Matrix B_T = init_matrix(B->length, B->cols, B->rows);
    transpose_matrix(B, B_T);
    resize_matrix(C, A->length * B->length);
    size_t A_idx = 0, B_idx = 0, C_idx = 0;
    while (A_idx < A->length)
    {
        size_t prev_A_idx = A_idx;
        double sum = 0;
        B_idx = 0;
        // transverse B_T for someone row in A, calculate the a row values in C
        while (B_idx < B_T->length)
        {
            if (A->data[A_idx].y != B_T->data[B_idx].y)
            {
                if (A->data[A_idx + 1].x != A->data[A_idx].x || B_T->data[B_idx + 1].x != B_T->data[B_idx].x)
                {
                    // settle current sum
                    if (sum != 0.0)
                    {
                        C->data[C_idx].x = A->data[A_idx].x;
                        C->data[C_idx].y = B_T->data[B_idx].x;
                        C->data[C_idx].value = sum;
                        C_idx++;
                    }
                    sum = 0;
                    // initialize A_idx
                    A_idx = prev_A_idx;
                    // move to next row in B_T
                    while (B_T->data[B_idx + 1].x == B_T->data[B_idx].x)
                        B_idx++;
                }
                else if (A->data[A_idx].y < B_T->data[B_idx].y)
                {
                    A_idx++;
                }
                else // A->data[A_idx].y > B_T->data[B_idx].y
                {
                    B_idx++;
                }
            }
            else // A->data[A_idx].y == B_T->data[B_idx].y
            {
                sum += A->data[A_idx].value * B_T->data[B_idx].value;
                A_idx++;
                B_idx++;
            }
        }
        // move to next row in A
        while (A_idx < A->length && A->data[A_idx].x == A->data[prev_A_idx].x)
            A_idx++;
    }
}
*/

void multiply_matrix(const Matrix A, const Matrix B, Matrix C)
{
    Matrix B_T = init_matrix(B->length, B->cols, B->rows);
    transpose_matrix(B, B_T);
    resize_matrix(C, A->length * B->length);
    C->rows = A->rows;
    C->cols = B->cols;
    C->length = 0;

    size_t A_idx = 0, B_idx = 0, C_idx = 0;
    while (A_idx < A->length)
    {
        size_t A_row_start = A_idx;

        B_idx = 0;
        // transverse B_T for someone row in A, calculate the a row values in C
        while (B_idx < B_T->length)
        {
            int current_B_row = B_T->data[B_idx].x;
            size_t B_row_start = B_idx;
            double sum = 0.0;
            // merge A's current row and B_T's current row
            while (A_idx < A->length && A->data[A_idx].x == A->data[A_row_start].x &&
                   B_idx < B_T->length && B_T->data[B_idx].x == current_B_row)
            {
                if (A->data[A_idx].y < B_T->data[B_idx].y)
                {
                    A_idx++;
                }
                else if (A->data[A_idx].y > B_T->data[B_idx].y)
                {
                    B_idx++;
                }
                else // equal
                {
                    sum += A->data[A_idx].value * B_T->data[B_idx].value;
                    A_idx++;
                    B_idx++;
                }
            }
            // save non-zero result
            if (sum != 0.0)
            {
                C->data[C_idx].x = A->data[A_row_start].x;
                C->data[C_idx].y = current_B_row;
                C->data[C_idx].value = sum;
                C->length++;
                C_idx++;
            }
            // skip to next row in B_T
            while (B_idx < B_T->length && B_T->data[B_idx].x == current_B_row)
                B_idx++;
            // reset A_idx to the beginning of current row
            A_idx = A_row_start;
        }
        // move to next row in A
        while (A_idx < A->length && A->data[A_idx].x == A->data[A_row_start].x)
            A_idx++;
    }
}

/*
void multiply_matrix(const Matrix A, const Matrix B, Matrix C)
{
    if (A->cols != B->rows)
    {
        printf("Matrix dimension mismatch for multiplication\n");
        exit(1);
    }
    // 转置 B 矩阵
    Matrix B_T = init_matrix(B->length, B->cols, B->rows);
    transpose_matrix(B, B_T);

    // 预分配空间
    resize_matrix(C, A->rows * B->cols);
    C->rows = A->rows;
    C->cols = B->cols;
    C->length = 0;

    if (A->length == 0 || B_T->length == 0)
    {
        free_matrix(&B_T);
        return;
    }

    size_t C_idx = 0;
    size_t A_idx = 0;

    while (A_idx < A->length)
    {
        int current_row = A->data[A_idx].x; // A的当前行号
        size_t A_row_start = A_idx;         // A当前行的起始位置

        // B_T从头开始扫描
        size_t B_idx = 0;
        size_t A_temp = A_row_start;
        double sum = 0.0;

        while (B_idx < B_T->length)
        {
            int B_current_row = B_T->data[B_idx].x; // B_T的当前行号（对应B的列号）

            // 归并A的当前行与B_T的当前行
            A_temp = A_row_start; // 重置A的指针到行首
            sum = 0.0;

            while (A_temp < A->length && A->data[A_temp].x == current_row &&
                   B_idx < B_T->length && B_T->data[B_idx].x == B_current_row)
            {
                if (A->data[A_temp].y < B_T->data[B_idx].y)
                {
                    A_temp++;
                }
                else if (A->data[A_temp].y > B_T->data[B_idx].y)
                {
                    B_idx++;
                }
                else // 列索引相等，累加
                {
                    sum += A->data[A_temp].value * B_T->data[B_idx].value;
                    A_temp++;
                    B_idx++;
                }
            }

            // B_T当前行处理完毕，检查是否需要保存结果
            // 跳过B_T当前行剩余元素
            while (B_idx < B_T->length && B_T->data[B_idx].x == B_current_row)
                B_idx++;

            // 保存非零结果
            if (sum != 0.0)
            {
                C->data[C_idx].x = current_row;
                C->data[C_idx].y = B_current_row;
                C->data[C_idx].value = sum;
                C_idx++;
            }
        }

        // A移动到下一行
        while (A_idx < A->length && A->data[A_idx].x == current_row)
            A_idx++;
    }

    C->length = C_idx;
    resize_matrix(C, C_idx > 0 ? C_idx : 1);
    free_matrix(&B_T);
    }
*/

// text funtion
void text_add_matrix()
{
    printf("Add Matrix Test:\n");
    int rows, cols;
    int num_elem_A, num_elem_B;

    rows = 3;
    cols = 3;
    Matrix A, B, C;
    A = creat_from_array((double *[]){
                             (double[]){1, 0, 0},
                             (double[]){0, 2, 0},
                             (double[]){0, 0, 3}},
                         rows, cols);

    B = creat_from_array((double *[]){
                             (double[]){0, 1, 0},
                             (double[]){0, 0, 2},
                             (double[]){3, 0, 0}},
                         rows, cols);

    C = init_matrix(0, rows, cols);

    add_matrix(A, B, C);

    printf("Resultant Matrix C:\n");
    print_matrix(C, true);

    free_matrix(&A);
    free_matrix(&B);
    free_matrix(&C);
}

void text_transpose_matrix()
{
    printf("Transpose Matrix Test:\n");
    int rows, cols;
    int num_elem_A, num_elem_B;

    rows = 3;
    cols = 3;
    Matrix A, B, C;
    A = creat_from_array((double *[]){
                             (double[]){1, 1, 0},
                             (double[]){0, 2, 0},
                             (double[]){1, 0, 3}},
                         rows, cols);

    C = init_matrix(0, rows, cols);

    transpose_matrix(A, C);

    printf("Resultant Matrix C:\n");
    print_matrix(C, true);

    free_matrix(&A);
    free_matrix(&C);
}

void text_multiply_matrix()
{
    printf("Multiply Matrix Test:\n");
    int rows_A = 2, cols_A = 3;
    int rows_B = 3, cols_B = 2;

    Matrix A, B, C;
    A = creat_from_array((double *[]){
                             (double[]){1, 0, 2},
                             (double[]){0, 3, 0}},
                         rows_A, cols_A);

    B = creat_from_array((double *[]){
                             (double[]){2, 4},
                             (double[]){5, 0},
                             (double[]){0, 6}},
                         rows_B, cols_B);

    C = init_matrix(0, rows_A, cols_B);

    multiply_matrix(A, B, C);

    printf("Resultant Matrix C:\n");
    print_matrix(C, true);

    free_matrix(&A);
    free_matrix(&B);
    free_matrix(&C);
}

void menu()
{
    int choice;
    Matrix A = NULL, B = NULL, C = NULL;

    while (1)
    {
        printf("\n====== Sparse Matrix Menu ======\n");
        printf("1. Create Matrix A from Input\n");
        printf("2. Create Matrix B from Input\n");
        printf("3. Print Matrix A\n");
        printf("4. Print Matrix B\n");
        printf("5. Add A + B\n");
        printf("6. Transpose A\n");
        printf("7. Multiply A * B\n");
        printf("0. Exit\n");
        printf("================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 0)
        {
            printf("Exiting...\n");
            break;
        }

        int rows, cols;

        switch (choice)
        {
        case 1:
            printf("Enter rows and cols for Matrix A: ");
            scanf("%d %d", &rows, &cols);
            if (A != NULL)
                free_matrix(&A);
            A = init_matrix(0, rows, cols);
            input_matrix(A);
            break;

        case 2:
            printf("Enter rows and cols for Matrix B: ");
            scanf("%d %d", &rows, &cols);
            if (B != NULL)
                free_matrix(&B);
            B = init_matrix(0, rows, cols);
            input_matrix(B);
            break;

        case 3:
            if (A == NULL)
                printf("Matrix A not created!\n");
            else
                print_matrix(A, true);
            break;

        case 4:
            if (B == NULL)
                printf("Matrix B not created!\n");
            else
                print_matrix(B, true);
            break;

        case 5:
            if (A == NULL || B == NULL)
            {
                printf("Please create both A and B first!\n");
                break;
            }
            if (C != NULL)
                free_matrix(&C);
            C = init_matrix(A->length + B->length, A->rows, A->cols);
            add_matrix(A, B, C);
            printf("Result of A + B:\n");
            print_matrix(C, true);
            break;

        case 6:
            if (A == NULL)
            {
                printf("Matrix A not created!\n");
                break;
            }
            if (C != NULL)
                free_matrix(&C);
            C = init_matrix(A->length, A->cols, A->rows);
            transpose_matrix(A, C);
            printf("Transpose of A:\n");
            print_matrix(C, true);
            break;

        case 7:
            if (A == NULL || B == NULL)
            {
                printf("Please create both A and B first!\n");
                break;
            }
            if (A->cols != B->rows)
            {
                printf("Matrix dimensions do not match for multiplication!\n");
                break;
            }
            if (C != NULL)
                free_matrix(&C);
            C = init_matrix(A->rows * B->cols, A->rows, B->cols);
            multiply_matrix(A, B, C);
            printf("Result of A * B:\n");
            print_matrix(C, true);
            break;

        default:
            printf("Invalid choice! Please select again.\n");
            break;
        }
    }

    // Clean up
    if (A != NULL)
        free_matrix(&A);
    if (B != NULL)
        free_matrix(&B);
    if (C != NULL)
        free_matrix(&C);
}

// main function

int main()
{
    // text_add_matrix();
    // text_transpose_matrix();
    // text_multiply_matrix();
    menu();
    return 0;
}

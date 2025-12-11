#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*----------------------------------------------
 * BiTree
 *----------------------------------------------*/

typedef int ElemType;
typedef struct BiTNode
{
    struct BiTNode *lchild, *rchild;
    ElemType data;
} BiTNode, *BiTree;

// ------------------lifecycle management------------------

BiTree creat_BiTree()
{
    BiTree tree = (BiTree)malloc(sizeof(BiTNode));
    // if(tree == NULL)return NULL;
    return tree;
}

BiTree creat_BiTree_from_array(ElemType arr[], int n, int index)
{
    if (arr == NULL || index >= n)
        return NULL;
    BiTree node = (BiTree)malloc(sizeof(BiTNode));
    if (!node)
        return NULL;
    node->data = arr[index];
    node->lchild = creat_BiTree_from_array(arr, n, 2 * index + 1);
    node->rchild = creat_BiTree_from_array(arr, n, 2 * index + 2);
    return node;
}

bool destroy_BiTree(BiTree *T)
{
    if (T == NULL || *T == NULL)
        return false;
    destroy_BiTree(&((*T)->lchild));
    destroy_BiTree(&((*T)->rchild));
    free(T);
    *T = NULL;
    return true;
}

// --------------------basic property----------------------

size_t BiTree_depth(BiTree T)
{
    if (T == NULL)
        return 0;
    size_t left_depth = BiTree_depth(T->lchild) + 1;
    size_t right_depth = BiTree_depth(T->rchild) + 1;
    return left_depth > right_depth ? left_depth : right_depth;
}

bool BiTree_empty(BiTree T)
{
    if (T == NULL)
        return true;
    else
        return false;
}

//--------------------- change operation-------------------

bool insert_replace_BiTnode(BiTree parent, ElemType value, bool is_insert_left)
{
    if (parent == NULL)
        return false;
    BiTree new_node = (BiTree)malloc(sizeof(BiTNode));
    if (!new_node)
        return false;

    new_node->lchild = parent->lchild;
    new_node->rchild = parent->rchild;
    new_node->data = value;
    if (is_insert_left)
    {
        parent->lchild = new_node;
        parent->rchild = NULL;
    }
    else
    {
        parent->rchild = new_node;
        parent->lchild = NULL;
    }
    return true;
}

bool insert_by_path(BiTree *T, const char *path, ElemType value)
{
    if (T == NULL)
        return false;

    // 如果树根不存在 → 创建根
    if (*T == NULL)
    {
        *T = (BiTree)malloc(sizeof(BiTNode));
        if (!(*T))
            return false;
        (*T)->data = -1; // 给根一个默认值
        (*T)->lchild = (*T)->rchild = NULL;
    }

    BiTree current = *T;
    const char *p = path;

    // 遍历到路径的倒数第一个节点
    while (*(p + 1) != '\0')
    {
        if (*p == 'L')
        {
            if (!current->lchild)
            {
                current->lchild = (BiTree)malloc(sizeof(BiTNode));
                current->lchild->data = -1;
                current->lchild->lchild = current->lchild->rchild = NULL;
            }
            current = current->lchild;
        }
        else if (*p == 'R')
        {
            if (!current->rchild)
            {
                current->rchild = (BiTree)malloc(sizeof(BiTNode));
                current->rchild->data = -1;
                current->rchild->lchild = current->rchild->rchild = NULL;
            }
            current = current->rchild;
        }
        else
        {
            return false; // 非法字符
        }
        p++;
    }

    // 最后一个字符决定插入位置
    if (*p == 'L')
    {
        if (!current->lchild)
        {
            current->lchild = (BiTree)malloc(sizeof(BiTNode));
            current->lchild->lchild = current->lchild->rchild = NULL;
        }
        current->lchild->data = value;
    }
    else if (*p == 'R')
    {
        if (!current->rchild)
        {
            current->rchild = (BiTree)malloc(sizeof(BiTNode));
            current->rchild->lchild = current->rchild->rchild = NULL;
        }
        current->rchild->data = value;
    }
    else
    {
        return false;
    }

    return true;
}

// ---------------------- traverse ------------------------

bool pre_order_traverse(BiTree tree, void (*visit)(BiTNode *node))
{
    if (!tree)
        return false;

    visit(tree);
    pre_order_traverse(tree->lchild, visit);
    pre_order_traverse(tree->rchild, visit);
    return true;
}

bool in_order_traverse(BiTree tree, void (*visit)(BiTNode *node))
{
    if (!tree)
        return false;

    in_order_traverse(tree->lchild, visit);
    visit(tree);
    in_order_traverse(tree->rchild, visit);
    return true;
}

bool post_order_traverse(BiTree tree, void (*visit)(BiTNode *node))
{
    if (!tree)
        return false;

    post_order_traverse(tree->lchild, visit);
    post_order_traverse(tree->rchild, visit);
    visit(tree);
    return true;
}

// --------------------- access ---------------------------

BiTree get_left_tree(BiTree parent)
{
    if (!parent)
        return NULL;
    return parent->lchild;
}

BiTree get_right_tree(BiTree parent)
{
    if (!parent)
        return NULL;
    return parent->rchild;
}

// --------------------  output ---------------------------

static void print_node(BiTNode *node)
{
    printf("%d ", node->data);
}

void pre_order_print(BiTree tree)
{
    pre_order_traverse(tree, print_node);
    printf("\n");
}

void in_order_print(BiTree tree)
{
    in_order_traverse(tree, print_node);
    printf("\n");
}

void post_order_print(BiTree tree)
{
    post_order_traverse(tree, print_node);
    printf("\n");
}

#define INDENT_STEP 4

void print_BiTree(const BiTree root, int indent, char branch)
{
    if (root == NULL)
        return;

    print_BiTree(root->rchild, indent + INDENT_STEP, '/');

    for (int i = 0; i < indent; i++)
        printf(" ");
    if (branch == '/')
        printf("┌── ");
    else if (branch == '\\')
        printf("└── ");
    printf("%d\n", root->data);

    print_BiTree(root->lchild, indent + INDENT_STEP, '\\');
}

// void print_current_node(int current_deep, BiTNode* current_node, bool is_left){
//     for(int i=0; i<INDENT_STEP*current_deep; i++) printf(" ");
//     if(is_left ==true)printf("┌── ");
//     else printf("└── ");
//     printf("%d\n", current_node->data);
// }

void print_BiTree_pretty(BiTree root)
{
    print_BiTree(root, -INDENT_STEP, ' ');
}

// ------------------------menu----------------------------

void menu()
{
    BiTree root = NULL;
    int choice;

    while (1)
    {
        printf("\n======= Binary Tree Operation Menu =======\n");
        printf("1. Insert node by path (e.g., LLR)\n");
        printf("2. Print tree structure (ASCII graph)\n");
        printf("3. Preorder traversal\n");
        printf("4. Inorder traversal\n");
        printf("5. Postorder traversal\n");
        printf("6. Print tree depth\n");
        printf("7. Check if tree is empty\n");
        printf("8. Destroy the entire tree\n");
        printf("9. Build a complete binary tree from array\n");
        printf("0. Exit\n");
        printf("===========================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input, please try again.\n");
            while (getchar() != '\n')
                ; // clear buffer
            continue;
        }

        if (choice == 1)
        {
            char path[110];
            ElemType value;

            printf("Path (use # for empty string): ");
            scanf("%s", path);
            if (path[0] == '#')
                path[0] = '\0';

            printf("Value to insert: ");
            scanf("%d", &value);

            if (insert_by_path(&root, path, value))
                printf("Insert successful!\n");
            else
                printf("Insertion failed, invalid path.\n");
        }
        else if (choice == 2)
        {
            printf("\nCurrent tree structure:\n");
            print_BiTree_pretty(root);
        }
        else if (choice == 3)
        {
            printf("Preorder traversal: ");
            pre_order_print(root);
        }
        else if (choice == 4)
        {
            printf("Inorder traversal: ");
            in_order_print(root);
        }
        else if (choice == 5)
        {
            printf("Postorder traversal: ");
            post_order_print(root);
        }
        else if (choice == 6)
        {
            printf("Tree depth = %zu\n", BiTree_depth(root));
        }
        else if (choice == 7)
        {
            printf("Is tree empty: %s\n", BiTree_empty(root) ? "Yes" : "No");
        }
        else if (choice == 8)
        {
            destroy_BiTree(&root);
            printf("Tree destroyed.\n");
        }
        else if (choice == 9)
        {
            int n;
            printf("Enter array length: ");
            scanf("%d", &n);

            ElemType *arr = malloc(sizeof(ElemType) * n);
            printf("Enter %d integers: ", n);
            for (int i = 0; i < n; i++)
                scanf("%d", &arr[i]);

            destroy_BiTree(&root); // remove old tree
            root = creat_BiTree_from_array(arr, n, 0);

            printf("Build complete.\n");
            free(arr);
        }
        else if (choice == 0)
        {
            printf("Exiting program.\n");
            break;
        }
        else
        {
            printf("Invalid choice, please try again.\n");
        }
    }
}

// -------------------text function------------------------

void test_bitree_depth()
{
    ElemType arr[] = {1, 2, 3, 4, 5, 6, 7};
    BiTree tree = creat_BiTree_from_array(arr, 3, 0);
    printf("the BiTree depth is::%zu \n", BiTree_depth(tree));
}

void test_traverse()
{
    ElemType arr[] = {1, 2, 3, 4, 5, 6, 7};
    BiTree tree = creat_BiTree_from_array(arr, 7, 0);
    // BiTree nodeptr = NULL;
    // nodeptr = get_left_tree(tree);
    // insert_replace_BiTnode(nodeptr, 9, true);
    pre_order_print(tree);
    in_order_print(tree);
    post_order_print(tree);
}

void test_insert()
{
    ElemType arr[] = {1, 2, 3, 4, 5, 6, 7};
    BiTree tree = creat_BiTree_from_array(arr, 7, 0);
    BiTree nodeptr = NULL;
    nodeptr = get_left_tree(tree);
    insert_replace_BiTnode(nodeptr, 9, true);
    pre_order_print(tree);

    // printf("the BiTree depth is::%zu", BiTree_depth(tree));
}

// -----------------------main-----------------------------
int main()
{
    // test_bitree_depth();
    // printf("\n");
    // test_traverse();
    // printf("\n");
    // test_insert();
    // printf("\n");
    menu();
    return 0;
}

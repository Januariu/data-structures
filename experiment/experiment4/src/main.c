#include "blockchain.h"

void test_function()
{
    String s = str_create_from("Hello World");
    String old = str_create_from("World");
    String new = str_create_from("Blockchain。      ");
    str_replace_all(s, old, new);
    str_println(s, stdout);
    str_print_per_block(s, stdout);
    str_destroy(&s);
    str_destroy(&old);
    str_destroy(&new);
}

void menu_string()
{
    String s = str_create();
    printf("\n===== String Menu =====\n");

    while (1)
    {
        printf("\n===== Operations =====\n");
        printf("1. Create from C-string\n");
        printf("2. Print string\n");
        printf("3. Append C-string\n");
        printf("4. Append char\n");
        printf("5. Insert C-string\n");
        printf("6. Insert char\n");
        printf("7. Delete substring\n");
        printf("8. Find substring\n");
        printf("9. Replace first occurrence\n");
        printf("10. Replace all occurrences\n");
        printf("11. Clear string\n");
        printf("12. Destroy and exit\n");
        printf("Enter choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 12)
        {
            str_destroy(&s);
            return;
        }

        char buffer[256];
        size_t pos, len;
        char c;

        switch (choice)
        {
        case 1:
        {
            printf("Enter C-string: ");
            scanf(" %255s", buffer);
            str_clear(s);
            String temp = str_create_from(buffer);
            str_append_str(s, temp);
            str_destroy(&temp);
            printf("Created.\n");
        }
        break;

        case 2:
            printf("String = ");
            str_println(s, stdout);
            break;

        case 3:
        {
            printf("Enter C-string to append: ");
            scanf(" %255s", buffer);
            String temp = str_create_from(buffer);
            str_append_str(s, temp);
            str_destroy(&temp);
            printf("Appended.\n");
        }
        break;

        case 4:
            printf("Enter char: ");
            scanf(" %c", &c);
            str_push_back(s, c);
            printf("Char appended.\n");
            break;

        case 5:
        {
            printf("Insert position: ");
            scanf("%zu", &pos);
            printf("C-string to insert: ");
            scanf(" %255s", buffer);
            String tmp = str_create_from(buffer);
            if (str_insert(s, pos, tmp))
                printf("Inserted.\n");
            else
                printf("Insert failed.\n");
            str_destroy(&tmp);
        }
        break;

        case 6:
            printf("Insert position: ");
            scanf("%zu", &pos);
            printf("Char to insert: ");
            scanf(" %c", &c);
            if (str_insert_char(s, pos, c))
                printf("Inserted.\n");
            else
                printf("Insert failed.\n");
            break;

        case 7:
            printf("Delete start pos: ");
            scanf("%zu", &pos);
            printf("Delete length: ");
            scanf("%zu", &len);
            if (str_delete(s, pos, len))
                printf("Deleted.\n");
            else
                printf("Delete failed.\n");
            break;

        case 8:
        {
            printf("Enter substring to find: ");
            scanf(" %255s", buffer);
            String pat = str_create_from(buffer);
            int pos = str_find_first(s, pat, 0);
            if (pos >= 0)
                printf("Found at position: %d\n", pos);
            else
                printf("Not found.\n");
            str_destroy(&pat);
        }
        break;

        case 9:
        {
            printf("Old substring: ");
            scanf(" %255s", buffer);
            String old = str_create_from(buffer);
            printf("New substring: ");
            scanf(" %255s", buffer);
            String new = str_create_from(buffer);

            if (str_replace_first(s, old, new))
                printf("Replaced first.\n");
            else
                printf("No match found.\n");

            str_destroy(&old);
            str_destroy(&new);
        }
        break;

        case 10:
        {
            printf("Old substring: ");
            scanf(" %255s", buffer);
            String old = str_create_from(buffer);
            printf("New substring: ");
            scanf(" %255s", buffer);
            String new = str_create_from(buffer);

            int cnt = str_replace_all(s, old, new);
            printf("Replaced %d times.\n", cnt);

            str_destroy(&old);
            str_destroy(&new);
        }
        break;

        case 11:
            str_clear(s);
            printf("String cleared.\n");
            break;

        default:
            printf("Invalid choice.\n");
        }
    }
}

int main()
{
    test_function();
    return 0;
}
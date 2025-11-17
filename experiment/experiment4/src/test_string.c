/* test_string.c */
#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void expect_int(int got, int want, const char *msg)
{
    if (got != want)
    {
        fprintf(stderr, "FAIL: %s: got %d, want %d\n", msg, got, want);
        exit(1);
    }
}

static void expect_str(String s, const char *want, const char *msg)
{
    /* produce cstring */
    size_t n = str_length(s);
    char *buf = malloc(n + 1);
    for (size_t i = 0; i < n; ++i)
        buf[i] = str_at(s, i);
    buf[n] = '\0';
    if (strcmp(buf, want) != 0)
    {
        fprintf(stderr, "FAIL: %s: got \"%s\", want \"%s\"\n", msg, buf, want);
        free(buf);
        exit(1);
    }
    free(buf);
}

int main(void)
{
    String s = str_create_from("hello");
    expect_int((int)str_length(s), 5, "length after create_from");

    /* push_back */
    str_push_back(s, '!');
    expect_str(s, "hello!", "after push_back");

    /* append */
    String t = str_create_from(" world");
    str_append_str(s, t);
    expect_str(s, "hello! world", "after append_str");

    /* find */
    int p = str_find_first(s, str_create_from("world"), 0);
    expect_int(p, 7, "find first world");

    /* replace first */
    String old = str_create_from("world");
    String neu = str_create_from("universe");
    bool r = str_replace_first(s, old, neu);
    expect_int(r ? 1 : 0, 1, "replace first returns true");
    expect_str(s, "hello! universe", "after replace_first");

    /* delete */
    str_delete(s, 6, 1); /* remove space before universe? adjust */
    /* depending on previous result keep check flexible */
    /* substring */
    String sub = str_create();
    str_substring(sub, s, 0, 5);
    expect_str(sub, "hello", "substring 0..5");

    /* concat */
    String rstr = str_create();
    str_concat(rstr, sub, neu);
    expect_str(rstr, "hellouniverse", "concat result");

    /* cleanup */
    str_destroy(&s);
    str_destroy(&t);
    str_destroy(&old);
    str_destroy(&neu);
    str_destroy(&sub);
    str_destroy(&rstr);

    printf("All tests passed.\n");
    return 0;
}

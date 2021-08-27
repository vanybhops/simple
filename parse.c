#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct string_t {
    char *value;
    int start_pos;
    int end_pos;
    size_t len;
} String;


size_t ParseCodeString(String *str, char *code, size_t len)
{
    str->start_pos = 0;
    str->end_pos = 0;
    str->len = 0;
    str->value = NULL;

    for (size_t i = 0; i < len; i++)
    {
        if (code[i] == '"')
        {
            if (str->start_pos == 0)
                str->start_pos = i;
            else
            {
                if (str->end_pos == 0)
                    str->end_pos = i;
                else
                {
                    fprintf(stderr, "Syntax error - multiple strings detected for one statement\n");
                    return -1;
                }
            }
        } else if (code[i] == '\n')
        {
            if ((str->end_pos == 0 && str->start_pos != 0) || (str->end_pos == 0 && str->start_pos != 0 && i == len - 2))
            {
                fprintf(stderr, "Syntax error - could not finish scanning string\n");
                return -1;
            }
        }
    }

    if (str->end_pos == 0 && str->start_pos != 0)
    {
        fprintf(stderr, "Syntax error - could not finish scanning string\n");
        return -1;
    }

    if (str->start_pos == 0)
        return 0;

    str->len = str->end_pos++ - str->start_pos - 1;
    str->value = (char*)malloc(str->len + 1);
    memcpy(str->value, code + str->start_pos++ + 1, str->len);

    return str->len;
}

int main()
{
    char *example_code = "print \"Hello, world!\"";

    String s;

    printf("Parsing the following line:\n\x1b[01;32m%s\x1b[0m\n\n", example_code);

    if (ParseCodeString(&s, example_code, strlen(example_code)) < 0)
    {
        fprintf(stderr, "Error parsing string\n");
        return -1;
    }

    printf("String start position: %d\n", s.start_pos);
    printf("String end   position: %d\n", s.end_pos);
    printf("String length        : %ld\n", s.len);
    printf("String value         : %s\n", s.value);

    return 0;
}

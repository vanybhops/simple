#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *keywords[] = {
    "print",
    "sys"
};

char operators[] = "+-*/%=";

int isKeyword(char *buf)
{
    size_t n_keywords = sizeof(keywords)/8;

    for (int i = 0; i < n_keywords; i++)
    {
        if (!strcmp(keywords[i], buf))
            return 1;
    }

    return 0;
}

int isOperator(char ch)
{
    for (int i = 0; i < strlen(operators); i++)
    {
        if (operators[i] == ch)
            return 1;
    }

    return 0;
}

int main()
{
    int counter=1;
    char ch;
    char buf[16];
    int i = 0;
    int break_detected = 0;

    FILE *fptr = fopen("file.txt", "r");

    if (fptr == NULL)
    {
        printf("Can't open file - %s\n", strerror(errno));
        return -1;
    }

    while (ch != EOF) {
        
        ch = fgetc(fptr);
        printf("READ CHARACTER: %s%c [%02X]\n", (ch==EOF)?"(end of file)":"", (ch==EOF)?0:ch, ch & 0xFF);
        if(ch=='\n'){
            counter++;
        }
        if (isOperator(ch)) {
            printf("%c is operator line number: %d\n", ch,counter-1);
            buf[i+1] = '\0';
            i = 0;

            buf[i-1] = '\0';
            i = 0;

            if (isKeyword(buf))
                printf("%s is a keyword line number: %d\n", buf,counter-1);
            else
                printf("%s is not recognized line number: %d\n", buf,counter-1);
        }

        if ((ch == ' ' || ch == '\n') && (i != 0)) {
            switch (ch) {
                case 0x20:
                    printf("Break detected (space) line number: %d\n",counter-1);
                    break;
                case 0x10:
                    printf("Break detected (newline) line number: %d\n",counter-1);
                    break;
                default:
                    break;
            }

            buf[i] = '\0';
            i = 0;
            break_detected = 1;

            if (isKeyword(buf))
                printf("%s is a keyword line number: %d\n", buf,counter-1);
            else
                printf("%s is not recognized line number: %d\n", buf,counter-1);

            memset(buf, '\0', sizeof(buf));
        }

        if (!break_detected) {
            buf[i] = ch;
            i++;
        }
        

        break_detected = 0;
    }

    return 0;
}

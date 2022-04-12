#include <stdio.h>
#include <ctype.h>

int main(void) {

    int c;
    int next;

    while ((c = getchar()) != EOF) {

        if (c != '\n') {

            next = getchar();

            if (islower(c) && islower(next)) {

                if ((c % 7) == (next % 7)) {
                    printf("%c%c", next, c);
                }
                
                else {
                    printf("%c", c);
                    ungetc(next, stdin);
                }
            }

            else {
                printf("%c", c);
                ungetc(next, stdin);
            }
        }

        else {
            printf("\n");
        }
    }

    return 0;
}

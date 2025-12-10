#include <stdio.h>
#include <string.h>

#define NUM_STATES 8
#define NUM_INPUTS 4

int get_input(char c)
{
    if(c == 'a') return 0;
    if(c == 'b') return 1;
    if(c == 'c') return 2;
    return 3;
}

enum { D0, D1, D2, D3, D4, D5, D6, D7, DEAD };

const char* tokens[] =
{
    "",      // D0
    "",      // D1
    "aa+",    // D2
    "ab",    // D3
    "abc",   // D4
    "", "", "",
    NULL
};

int main()
{

    int next[NUM_STATES][NUM_INPUTS] =
    {

        /* D0 */ { D1, DEAD, DEAD,DEAD},

        /* D1 */ { D2, D3, DEAD,DEAD },
        /* D2 */ { D2, DEAD, DEAD,DEAD},


        /* D3 */ { DEAD, DEAD, D4,DEAD},
        /* D4 */ { DEAD, DEAD, DEAD,DEAD },

        /* D5 */ { DEAD, DEAD, DEAD,DEAD },
        /* D6 */ { DEAD, DEAD, DEAD,DEAD },
        /* D7 */ { DEAD, DEAD, DEAD,DEAD }
    };

    char str[100];
    printf("Enter a string: ");
    scanf("%s", str);

    int state = D0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        int input = get_input(str[i]);
        state = next[state][input];
        if (state == DEAD) break;
    }

    if (tokens[state] != NULL && strlen(tokens[state]) > 0)
        printf("Accepted as pattern: %s\n", tokens[state]);
    else
        printf("Rejected\n");

    return 0;
}

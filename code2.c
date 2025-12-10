#include <stdio.h>
#include <string.h>

#define NUM_STATES 9
#define NUM_INPUTS 4

// DFA states
enum { D0, D1, D2, D3, D4, D5, D6, D7, DEAD };


int get_input(char c)
{
    if (c == 'a') return 0;
    if (c == 'b') return 1;
    if (c == 'c') return 2;
    return 3; // invalid
}

// Accepting tokens
const char* accepting_tokens[NUM_STATES] =
{
    "",     // D0
    "",     // D1
    "bb+",  // D2
    "",     // D3
    "aa",   // D4
    "c+",   // D5
    "", "",
    NULL    // DEAD
};

int main()
{

    // Transition table
    int next_state[NUM_STATES][NUM_INPUTS] =
    {

        /* D0 */ { D3, D1, D5, DEAD },

        /* D1 */ { DEAD, D2, DEAD, DEAD },
        /* D2 */ { DEAD, D2, DEAD, DEAD },

        /* aa */
        /* D3 */ { D4, DEAD, DEAD, DEAD },
        /* D4 */ { DEAD, DEAD, DEAD, DEAD },

        /* c* */
        /* D5 */ { DEAD, DEAD, D5, DEAD },

        /* D6 */ { DEAD, DEAD, DEAD, DEAD },
        /* D7 */ { DEAD, DEAD, DEAD, DEAD },
        /* DEAD */{ DEAD, DEAD, DEAD, DEAD }
    };

    char str[100];
    printf("Enter a string: ");
    scanf("%s", str);

    int state = D0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        int input = get_input(str[i]);
        state = next_state[state][input];
        if (state == DEAD) break;
    }

    if (accepting_tokens[state] != NULL && strlen(accepting_tokens[state]) > 0)
        printf("%s Accepted by DFA: %s\n", str, accepting_tokens[state]);
    else
        printf("%s Rejected by all patterns\n", str);

    return 0;
}

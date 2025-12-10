#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define NUM_STATES 65
#define NUM_INPUTS 12

int get_input(char c)
{
    if(c=='_') return 0;
    if(isalpha((unsigned char)c)) return 1;
    if(isdigit((unsigned char)c)) return 2;
    if(c=='F') return 3;
    if(c=='n') return 4;
    if(c=='.') return 5;
    if(c==':') return 6;
    if(c=='/') return 7;
    if(c=='<') return 8;
    if(isspace((unsigned char)c)) return 9;
    if(c=='('||c==')'||c=='{'||c=='}'||c=='='||c=='+'||c==',') return 10;
    return 11;
}

enum {
    S0=0, V1,V2,V3,V4,F1,F2,F3,N1,D1,D2,
    L0,L1,L2,L3,L4,L5,L6,L7,L8,
    C1,C2,LT1,SYM1,
    R23,R24,R25,R26,R27,R28,R29,R30,R31,R32,
    R33,R34,R35,R36,R37,R38,R39,R40,R41,R42,
    R43,R44,R45,R46,R47,R48,R49,R50,R51,R52,
    R53,R54,R55,R56,R57,R58,R59,R60,R61,R62,R63,
    DEAD=64
};

const char *accepting_tokens[NUM_STATES] = {
NULL,NULL,NULL,NULL,"VARIABLE",
NULL,NULL,"FUNCTION","NUMBER",NULL,"TERMINATOR",
NULL,NULL,NULL,NULL,NULL,NULL,NULL,"LOOP_LABEL",
NULL,"COMMENT","OP_LT","SYMBOL",
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL
};

int main()
{
    int next_state[NUM_STATES][NUM_INPUTS] = {
/*_   L   D   F   n   .   :   /   <   sp  sym  oth*/
{V1, L0, N1,DEAD,DEAD, D1,DEAD, C1, LT1, S0, SYM1,DEAD},
{DEAD,V2,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,V2, V3,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,V4,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,F1,DEAD, F2,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD, F3, DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD, N1, DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD,DEAD, D2, DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{ L0,  L0, L0, L0, L0, DEAD, L0, DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD, L2,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD, L3,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{ L4,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD, L5,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD, L5, L6, DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD, L7, DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD,DEAD,DEAD, L8, DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD, C2,DEAD,DEAD,DEAD,DEAD},
{ C2, C2, C2, C2, C2, C2, C2, C2, C2, C2, C2, C2},
{DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},
{DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD},

/* filler states */
#define ROW {DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD,DEAD}
ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,
ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,
ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,
ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,ROW,
ROW,
/*DEAD*/ROW
    };

    FILE *fp=fopen("test.c","r");
    if(!fp){ puts("Cannot open test.c"); return 1; }

    char lex[300]; int len=0, st=S0, ch;

    printf("\n---- DFA LEXER OUTPUT ----\n\n");

    while((ch=fgetc(fp))!=EOF)
    {
        if(st==S0 && isspace(ch)) continue;     // FIXED

        int in=get_input((char)ch);
        int nx=next_state[st][in];

        if(nx==DEAD)
        {
            if(len){
                lex[len]='\0';

                if(accepting_tokens[st])
                    printf("%-15s %s \n", lex, accepting_tokens[st]);
                else
                    printf("%-15s Rejected \n", lex);

                len=0; st=S0;
                ungetc(ch,fp);
            }
        }
        else
        {
            if(st==S0 && (in==8 || in==10)){    // < or SYMBOL
                lex[0]=ch; lex[1]='\0';

                if(accepting_tokens[nx])
                    printf("%-15s %s \n", lex, accepting_tokens[nx]);
                else
                    printf("%-15s SYMBOL \n", lex);   // FIXED

                len=0; st=S0;
            }
            else
            {
                lex[len++]=ch;
                st=nx;
            }
        }
    }

    if(len){
        lex[len]='\0';
        if(accepting_tokens[st])
            printf("%-15s %s \n", lex, accepting_tokens[st]);
        else
            printf("%-15s Rejected \n", lex);
    }

    fclose(fp);
    return 0;
}

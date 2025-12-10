#include <stdio.h>
#include <string.h>

#define MAXSTACK 200
#define MAXTOK 200
#define MAXSYM 50

// ----------------------
// NEW GRAMMAR
// S  → A Sprime
// Sprime → ; A Sprime | ε
// A  → id Aprime
// Aprime → , id Aprime | ε
// ----------------------

char *NT[] = {"S","Sprime","A","Aprime"};
#define NNT 4

char *TERMINALS[] = {"id",",",";","$"};
#define NTER 4

char *RHS[] = {
    "A Sprime",     // 1
    "; A Sprime",   // 2
    "",             // 3 epsilon
    "id Aprime",    // 4
    ", id Aprime",  // 5
    ""              // 6 epsilon
};

// LL(1) Table
int TABLE[NNT][NTER] = {
    // id ,  ;  $
    {1, 0, 0, 0},   // S
    {0, 0, 2, 3},   // Sprime
    {4, 0, 0, 0},   // A
    {6, 5, 6, 6}    // Aprime
};

// ----------------------
// Stack
// ----------------------
char stack[MAXSTACK][MAXSYM];
int top = -1;
void push(char *s){ strcpy(stack[++top], s); }
char* pop(){ return (top>=0)?stack[top--]:NULL; }

void print_stack(){
    printf("[");
    for(int i=top;i>=0;i--){
        printf("%s",stack[i]);
        if(i>0) printf(", ");
    }
    printf("]");
}

// ----------------------
// Helpers
// ----------------------
int find_nt(char *x){
    for(int i=0;i<NNT;i++)
        if(strcmp(NT[i],x)==0) return i;
    return -1;
}

int find_t(char *x){
    for(int i=0;i<NTER;i++)
        if(strcmp(TERMINALS[i],x)==0) return i;
    return -1;
}

int tokenize(char *line, char tokens[][MAXSYM]){
    int n=0;
    char *p = strtok(line," \t\n");
    while(p){
        strcpy(tokens[n++], p);
        p=strtok(NULL," \t\n");
    }
    if(n==0 || strcmp(tokens[n-1],"$")!=0)
        strcpy(tokens[n++], "$");
    return n;
}

// ----------------------
// PARSER
// ----------------------
int main(){
    char line[500];
    printf("Enter declaration input (example: 'id , id ; id'):\n");
    fgets(line,sizeof(line),stdin);

    char input[MAXTOK][MAXSYM];
    int n = tokenize(line, input);

    printf("\nTokens:\n");
    for(int i=0;i<n;i++) printf("%s ",input[i]);
    printf("\n\n");

    push("$");
    push(NT[0]);   // S is start symbol
    int ip=0;

    printf("%-25s %-10s %-10s %-25s\n","Stack","Lookahead","Top","Production Applied");
    printf("-------------------------------------------------------------------\n");

    while(top>=0){
        char X[MAXSYM];
        strcpy(X,pop());
        char *a = input[ip];

        printf("%-25s %-10s %-10s ","", a, X);

        int tindex = find_t(X);

        if(tindex != -1){
            if(strcmp(X,a)==0){
                printf("%-25s\n","match");
                ip++;
            } else {
                printf("REJECTED\n");
                return 0;
            }
            print_stack(); printf("\n");
            continue;
        }

        int ntindex = find_nt(X);
        int aindex = find_t(a);

        if(ntindex==-1 || aindex==-1){
            printf("REJECTED\n");
            return 0;
        }

        int prod = TABLE[ntindex][aindex];

        if(prod==0){
            printf("REJECTED\nNo rule for (%s,%s)\n", X,a);
            return 0;
        }

        if(strlen(RHS[prod-1])==0)
            printf("%-25s\n","epsilon");
        else
            printf("%-25s\n", RHS[prod-1]);

        if(strlen(RHS[prod-1])>0){
            char temp[200]; strcpy(temp,RHS[prod-1]);
            char *p = strtok(temp," ");
            char symbols[10][MAXSYM];
            int k=0;
            while(p){ strcpy(symbols[k++],p); p=strtok(NULL," "); }
            for(int i=k-1;i>=0;i--) push(symbols[i]);
        }

        print_stack(); printf("\n");
    }

    printf("\nACCEPTED\n");
    return 0;
}

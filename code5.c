#include <stdio.h>
#include <string.h>

#define MAXSTACK 200
#define MAXTOK 200
#define MAXSYM 50

// ----------------------
// Grammar 3 (Boolean Expressions)
// B  → T B'
// B' → or T B' | ε
// T  → F T'
// T' → and F T' | ε
// F  → ( B ) | id
// ----------------------

char *NT[] = {"B","Bprime","T","Tprime","F"};
#define NNT 5

char *TERMINALS[] = {"id","and","or","("," )","$"};
#define NTER 6

char *RHS[] = {
    "T Bprime",        // 1
    "or T Bprime",     // 2
    "",                // 3 epsilon
    "F Tprime",        // 4
    "and F Tprime",    // 5
    "",                // 6 epsilon
    "( B )",           // 7
    "id"               // 8
};

// LL(1) Parsing Table
int TABLE[NNT][NTER] = {
    // id and or ( ) $
    {1, 0, 0, 1, 0, 0},   // B
    {0, 0, 2, 0, 3, 3},   // B'
    {4, 0, 0, 4, 0, 0},   // T
    {0, 5, 6, 0, 6, 6},   // T'
    {8, 0, 0, 7, 0, 0}    // F
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
int find_nt(char *x){ for(int i=0;i<NNT;i++) if(strcmp(NT[i],x)==0) return i; return -1; }
int find_t(char *x){ for(int i=0;i<NTER;i++) if(strcmp(TERMINALS[i],x)==0) return i; return -1; }

int tokenize(char *line, char tokens[][MAXSYM]){
    int n=0;
    char *p=strtok(line," \t\n");
    while(p){
        strcpy(tokens[n++],p);
        p=strtok(NULL," \t\n");
    }
    if(n==0 || strcmp(tokens[n-1],"$")!=0)
        strcpy(tokens[n++],"$");
    return n;
}

// ----------------------
// Parser
// ----------------------
int main(){
    char line[500];
    printf("Enter boolean expression tokens (e.g. 'id or id and id'):\n");
    fgets(line,sizeof(line),stdin);

    char input[MAXTOK][MAXSYM];
    int n = tokenize(line,input);

    printf("\nTokens:\n");
    for(int i=0;i<n;i++) printf("%s ",input[i]);
    printf("\n\n");

    push("$");
    push(NT[0]); // start with B
    int ip = 0;

    printf("%-25s %-10s %-10s %-25s\n","Stack","Lookahead","Top","Production");
    printf("-------------------------------------------------------------------\n");

    while(top >= 0){
        char X[MAXSYM];
        strcpy(X, pop());
        char *a = input[ip];

        printf("%-25s %-10s %-10s ","", a, X);

        int tindex = find_t(X);

        if(tindex != -1){
            if(strcmp(X,a)==0){
                printf("match\n");
                ip++;
            } else {
                printf("REJECTED\n");
                return 0;
            }
            print_stack(); printf("\n");
            continue;
        }

        int nt = find_nt(X);
        int ai = find_t(a);

        if(nt==-1 || ai==-1){
            printf("REJECTED\n");
            return 0;
        }

        int prod = TABLE[nt][ai];

        if(prod == 0){
            printf("REJECTED (no rule for %s, %s)\n",X,a);
            return 0;
        }

        if(strlen(RHS[prod-1])==0)
            printf("epsilon\n");
        else
            printf("%s\n", RHS[prod-1]);

        if(strlen(RHS[prod-1]) > 0){
            char temp[200];
            strcpy(temp, RHS[prod-1]);
            char *p = strtok(temp," ");
            char arr[10][MAXSYM];
            int k=0;
            while(p){
                strcpy(arr[k++],p);
                p=strtok(NULL," ");
            }
            for(int i=k-1;i>=0;i--) push(arr[i]);
        }

        print_stack(); printf("\n");
    }

    printf("\nACCEPTED\n");
    return 0;
}

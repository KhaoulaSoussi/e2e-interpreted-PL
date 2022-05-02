#include <stdio.h>
#include <string.h>
#include <math.h>
#define LINES 60
#define INPFILE "Parse Tree.txt"
#define OUTFILE "AL Code.txt"

char dataInit[LINES][16], code[LINES][16];  //These two arrays are used to store the final AL code
int codeLine = 0, dataLine = 0;
char token[20];
int level;
FILE* inp;

void getNextToken(int l);
void fillDataInit();
void fillCodeMemory();
void assignment();
void print();
int isColon();
int isComma();
int isDot();
int isCloseBrack();

void main(){
    FILE* out;
    inp = fopen(INPFILE, "r");
    out = fopen(OUTFILE, "w");
    //Here we loop over the parse tree and we keep reading tokes until we reach the end of the file
    while(!feof(inp)){
        getNextToken(1);
        //If the token is a variable declaration and the current level of the tree is 2
        //That means this must be a global variable declaration
        //Therefore we call upon the function that fills in the AL data memory part
        if(strcmp(token, "<VariableDeclaration>") == 0 && level == 2){
            fillDataInit();
        }
        //If the token is the keyword "<Main>"
        //That means we are about to read code memory lines
        //Therefore we call upon the function that fills in the AL code memory part
        else if(strcmp(token, "<Main>") == 0){
            fillCodeMemory();
        }
    }
    printf("AL Code Generated Successfully!");
    //Here we print out the AL codes to the output file
    for(int i = 0; i < dataLine; i++){
        fprintf(out, "%s\n", dataInit[i]);
    }
    for(int i = 0; i < codeLine; i++){
        fprintf(out,"%s\n", code[i]);
    }
    fclose(inp);
    fclose(out);
}

//This is the same function used in the Static Semantics phases
void getNextToken(int l){
    for(int i = 0; i < l; i++){
        fscanf(inp,"%d", &level);
        getc(inp);getc(inp);
        fscanf(inp, "%s", token);
    }
}

//This function is responsibl for filling in the data initialization part of the AL code
void fillDataInit(){
    char size[5], var[4], opcode[4], initVal[16];
    getNextToken(1);
    if(strcmp(token, "<entierDeclaration>") == 0){
        getNextToken(1);
        while(isDot() == 0){
            getNextToken(2);
            strcpy(opcode, "DEC");
            sprintf(var, "% *s", 4, token);
            getNextToken(1);
            if(isColon(token)){
                getNextToken(2);
                sprintf(initVal, "+0  0 0000 %0*s", 4, token);
                getNextToken(1);
            }
            else{
                strcpy(initVal, "+0  0 0000 0000");
            }
            sprintf(dataInit[dataLine++],"%s   %s 0004", opcode, var);
            sprintf(dataInit[dataLine++],"%s", initVal);
            if(isComma()){
                continue;
            }
            else if(isDot()){
                sprintf(dataInit[dataLine++],"%s", "SPR");
                return;
            }
        }
    }
}

//This function is responsible for filling in the code memory part of the AL code
void fillCodeMemory(){
    getNextToken(3);
    while(!feof(inp)){
        getNextToken(1);
        if(strcmp(token, "<Statement>") == 0){
            getNextToken(1);
            if(strcmp(token, "<AssignmentStatement>") == 0){
                assignment();
            }
            else if(strcmp(token, "<FunctionCall>") == 0){
                getNextToken(1);
                if(strcmp(token, "<PredefinedFunction>") == 0){
                    getNextToken(1);
                    if(strcmp(token, "affiche") == 0){
                        print();
                    }
                }
            }
        }
    }
    sprintf(code[codeLine++],"%s", "HLT");
    sprintf(code[codeLine++],"%s", "SPR");
}

void assignment(){
    char LHS[5], operand1[5] = "0000", operand2[5] = "0000", opcode[4];
    char type1 = '0', type2 = '0', type = '0';
    int flag = 0;
    getNextToken(2);
    sprintf(LHS, "% *s", 4, token);
    getNextToken(2);
    if(strcmp(token, "<ArithmeticExpression>") == 0){
        getNextToken(3);
        while(isCloseBrack() == 0){
            getNextToken(1);
            if(strcmp(token, "<ID>") == 0){
                flag++;
                getNextToken(1);
                if(flag == 1){
                    type1 = 'A';
                    sprintf(operand1, "% *s", 4, token);
                }
                else if(flag == 2){
                    type2 = 'A';
                    sprintf(operand2, "% *s", 4, token);
                }
                getNextToken(1);
            }
            if(strcmp(token, "+") == 0){
                strcpy(opcode, "ADD");
                getNextToken(2);
            }
            else if(strcmp(token, "-") == 0){
                strcpy(opcode, "SUB");
                getNextToken(2);
            }
            else if(strcmp(token, "*") == 0){
                strcpy(opcode, "MUL");
            }
            else if(strcmp(token, "/") == 0){
                strcpy(opcode, "DIV");
            }
            if(flag == 2){
                if(type1 == 'A' && type2 == 'A'){
                    type = '9';
                }
                sprintf(code[codeLine++],"%s %c %s %s", opcode, type, operand1, operand2);
                sprintf(code[codeLine++],"MOV 0 0000 %s", LHS);
            }
        }
        getNextToken(1);
    }
}

void print(){
    char operand1[5] = "0000", operand2[5] = "0000", opcode[4];
    char type = '9';
    int flag = 0;
    strcpy(opcode, "OUT");
    getNextToken(3);
    sprintf(operand1, "% *s", 4, token);
    sprintf(code[codeLine++],"%s %c %s %s", opcode, type, operand1, operand2);
}

int isDot(){
    if(strcmp(token, ".") == 0){
        return 1;
    }
    return 0;
}

int isColon(){
    if(strcmp(token, ":") == 0){
        return 1;
    }
    return 0;
}

int isComma(){
    if(strcmp(token, ",") == 0){
        return 1;
    }
    return 0;
}

int isCloseBrack(){
    if(strcmp(token, "]") == 0){
        return 1;
    }
    return 0;
}

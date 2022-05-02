#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define ROWS 20
#define INPFILE "Parse Tree.txt"
#define SYMFILE "Simple Symbol Table.txt"

//Symbol table for the simple program: Simple Symbol Table.txt
//Symbol table for the medium complexity program: Medium Symbol Table.txt
//Symbol table for the first complex program: Complex1 Symbol Table.txt
//Symbol table for the second complex program: Complex2 Symbol Table.txt

void fillSymbolTable(FILE* sym);
char* identifiersChecking(char id[], char scope[]);      //Checks if a variable has been declared before its used
int isIdentifier(char token[]);
int functionsChecking(char funName[]);      //Checks if a function has been defined before it's called
void typeChecking();         //Checks if all the assignments have the right types of operands
void getNextToken(int l);
char* getDataType(char id[]);    //Returns the data type of the passed identifier by searching in the symbol table

char symbolTable[ROWS][6][20];  //This array is used to store what's in the symbol table input file sym
int symbolsCount = 0;
char token[30];
int level;
char currentScope[20] = "%Global";
FILE* inp;
int errorCount = 0; //This variable is used to store the error count

void main(){
    FILE* sym;
    char* idCheck;
    int funCheck;
    char funName[30], temp[30];
    //This variable is used to store the current scope, it is initialized to the global scope
    //Note that in the symbol table scopes start with a % sign followed by the function name
    inp = fopen(INPFILE, "r");
    sym = fopen(SYMFILE, "r");
    if(inp == NULL){
        printf("File does not exist! Please make sure to change the file name in the macro above to the desired file!");
        return;
    }
    if(sym == NULL){
        printf("File does not exist! Please make sure to change the file name in the macro above to the desired file!");
        return;
    }
    //Call the function that reads the symbol table and stores it in the array symbolTable
    fillSymbolTable(sym);
    //Here we loop over the parse tree input file and we keep getting tokens until we reach the end of the file

    while(!feof(inp)){
        //Once we reach a function call and it is not a call to a predefined function
        //we need to check if its scope exists only if it is not a predefined function
        //For that, we call upon the function "functionsChecking"
        if(strcmp(token , "<FunctionCall>") == 0){
            getNextToken(1);
            if(strcmp(token , "<PredefinedFunction>")){
                getNextToken(2);
                sprintf(funName,"%s%s", "%", token);
                funCheck = functionsChecking(funName);
                if(funCheck == -1){
                    errorCount++;
                    printf("ERROR: Undefined reference to function %s!\n", token);
                }
            }
        }
        getNextToken(1);
        //Once we reach the Main program, we change the current scope
        if(strcmp(token, "demarre") == 0){
            strcpy(currentScope, "%Main");
        }
        //Once we reach the keyword "fonction" that denotes the start of a function definition
        //we change the current scope to that function name
        else if(strcmp(token, "fonction") == 0){
            getNextToken(2);
            sprintf(currentScope,"%s%s", "%", token);
        }

        //Once we reach an identifier, we need to check if it has been declared in the current scope
        //If it's not, then we look for it in the global scope
        //For that, we call upon the function "identifiersChecking"
        else if(strcmp(token , "<ID>") == 0){
            getNextToken(1);
            idCheck = identifiersChecking(token, currentScope);
            if(idCheck == NULL){
                errorCount++;
                printf("ERROR: Undeclared variable %s!\n", token);
            }
        }
        else if(strcmp(token, "<AssignmentStatement>") == 0){
            typeChecking();
        }
    }
    if(errorCount == 0){
        printf("Successful Static Semantics\n");
    }
}

//This function reads the token from the parse tree
//It takes as parameter the number of lines to be read because there are many lines in the parse tree that should be ignored
void getNextToken(int l){
    for(int i = 0; i < l; i++){
        fscanf(inp,"%d", &level);
        getc(inp);getc(inp);
        fscanf(inp, "%s", token);
    }
}

//This functions reads from the symbol table and stores the symbols and the scopes in the corresponding array
void fillSymbolTable(FILE* sym){
    char ch;
    char str[20];
    int i = 0;
    while(!feof(sym)){
        fscanf(sym, "%s", str);
        if(strcmp(str, "entier") == 0 || strcmp(str, "car") == 0 || strcmp(str, "CONST") == 0){
            strcpy(symbolTable[i][0], str);
            fscanf(sym, "%s", symbolTable[i][1]);
        }
        else if(str[0] == '%'){
            strcpy(symbolTable[i][0], str);
            strcpy(symbolTable[i][1], "null");

        }
        i++;
    }
    symbolsCount = i - 1;
}

//This function takes as argument a user defined function name and checks if its scope exists or not
int functionsChecking(char funName[]){
    int i;
    char symbol[20];
    for(i = 0; i < symbolsCount; i++){
        strcpy(symbol, symbolTable[i][0]);
        if(strcmp(symbol, funName) == 0){
            return 1;
        }
    }
    return -1;
}

//This function takes as arguments an identifier and the scope in which it was found and it checks if that
//identifier has been declared in that scope or in the global scope (i.e., exists in the symbol table)
//If the identifier has been declared, it returns its data type, otherwise it returns NULL
char* identifiersChecking(char id[], char scope[]){
    int i, j, k;
    char symbol[20];
    if(strcmp(scope, "%Global") == 0){
        L:
            i = 0;
            while(strcmp(symbolTable[i][1], "null") != 0){
                strcpy(symbol, symbolTable[i][1]);
                if(strcmp(symbol, id) == 0){
                    return symbolTable[i][0];
                }
                i++;
            }
            return NULL;
    }
    for(j = 0; j < symbolsCount; j++){
        if(strcmp(scope, symbolTable[j][0]) == 0){
            break;
        }
    }
    int flag = 0;
    k = j + 1;
    while(strcmp(symbolTable[k][1], "null") != 0 && k <= symbolsCount){
        strcpy(symbol, symbolTable[k][1]);
        if(strcmp(symbol, id) == 0){
            flag = 1;
            return symbolTable[k][0];
        }
        k++;
    }
    if(flag == 0){
        goto L;
    }
}

void typeChecking(){
    char* idCheck;
    getNextToken(2);
    idCheck = identifiersChecking(token, currentScope);
    if(idCheck == NULL){
        errorCount++;
        printf("ERROR: Undeclared variable %s!\n", token);
    }
    else{
        getNextToken(2);
        if(strcmp(token, "<FunctionCall>") == 0){
            return;
        }
        else if((strcmp(token, "<NumLiteral>") == 0 && strcmp(idCheck, "entier") != 0) || (strcmp(token, "<CharLiteral>") == 0 && strcmp(idCheck, "car") != 0)){
            printf("ERROR: Assignment has incompatible types! ");
        }
        else if(strcmp(token, "<ArithmeticExpression>") == 0){
            while(strcmp(token, ".")){
                getNextToken(1);
                if(strcmp(token, "<ID>") == 0){
                    getNextToken(1);
                    idCheck = identifiersChecking(token, currentScope);
                    if(idCheck == NULL){
                        errorCount++;
                        printf("ERROR: Undeclared variable %s!\n", token);
                    }
                    else if(strcmp(idCheck, "entier")){
                        printf("ERROR: Arithmetic expression with a non-integer operand : %s!", token);
                    }
                }

            }
        }
    }
}


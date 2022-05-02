#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define RWC 15      //Reserved Words Count
#define INPFILE "Simple Token Stream.txt"
#define OUTFILE "Parse Tree.txt"

// Main program and global variables only: Simple Token Stream.txt
// Subprogram call and return (no parameters or local variables) : Medium Token Stream.txt
// Subprogram call and return with parameters & local variables only : Complex1 Token Stream.txt
// Non-local references to globally defined variables only : Complex2 Token Stream.txt

//The output file has the same title for all the input files

typedef struct n{
    char value[20];
    struct n* child;
    struct n* sibling;
}node;

//This variable is used to store the level of the tree while traversing it
int level = 1;
char nextToken[20];
FILE* src;
/*These are just temporary variables that hold unnecessary strings read from the token stream
(e.g., "Line" "Token")*/
char l[5], n[2], t[6], d[5];
//This is an array that stores all the reserved words used by our language
char reservedWords[15][10] = {"demarre", "definis", "si", "sinon", "tantque", "debut", "fin", "entier", "car", "renvoi", "pas", "et", "ou", "scanne", "affiche"};

node* newNode(char data[]);
node* addSibling(node* root, char data[]);
node* addChild(node* root, char data[]);
void traverseTree(node* root, int level, FILE* out);
void getNextToken();
void startSub();
void defineSub(node* root);
int isIdentifier();
int isCharacter();
int isNumLiteral();
void error();
void varDec(node* root);
int isStringLiteral();
void carDec(node* root);
void entierDec(node* root);
int isDot();
int isColon();
int isComma();
void funDefSub(node* root);
int isDataType();
int isOpenParen();
int isCloseParen();
int isOpenBrack();
int isCloseBrack();
void parameterSub(node* root);
void blockSub();
int isStatement();
void condSub(node* root);
void exprSub();
void relExprSub();
void arithExprSub(node* root);
int isRelOp();
int isArithOp();
void loopSub(node* root);
void asgSub(node* root);
int isFunCall();
int isPas();
int isOu();
int isEt();
void scanSub(node* root);
void afficheSub(node* root);
void termSub(node* root);
void logExprSub(node* root);
void funCallSub(node* root);
void andFirstSub(node* root);

void main(){
    src = fopen(INPFILE, "r");
    if(src == NULL){
        printf("File does not exist! Please make sure to change the file name in the macro above to the desired file!");
        return;
    }
    getNextToken();
    startSub();
    fclose(src);
}

void getNextToken(){
    fscanf(src, "%s %s %s %s %s\n", l, n, t, d, nextToken);
}

//This function starts by reading the program and calling upon the necessecary functions depending on the token found
void startSub(){
    FILE* parseTree;
    parseTree = fopen(OUTFILE, "w");
    node* root ,*child;
    root = newNode("<Program>");        //This is the root of the whole parse tree
    //As long as the token encountered in the token stream is a "define" keyword
    //Create a child node for the subTree "<defined>"
    //Call upon the function responsible for checking the <define> rule and pass to it its subTree root
    //Then get the next token
    while(strcmp(nextToken, "definis") == 0) {
        child = addChild(root, "<define>");
        defineSub(child);
        getNextToken();
    }
    //As long as the token encountered in the token stream is a data type
    //That means we're about to declare a variable
    //Create a child node for the subTree "<VariableDeclaration>"
    //Call upon the function responsible for checking the <VariableDeclaration> rule and pass to it its subTree root
    //Then get the next token
    while(isDataType() == 1){
        child = addChild(root, "<VariableDeclaration>");
        varDec(child);
        getNextToken();
    }
    //As long as the token encountered in the token stream is a "fonction" keyword
    //That means we're about to define a function
    //Create a child node for the subTree "<Function Definition>"
    //Call upon the function responsible for checking the <FunctionDefinition> rule and pass to it its subTree root
    //Then get the next token
    while(strcmp(nextToken, "fonction") == 0){
        child = addChild(root, "<FunctionDefinition>");
        funDefSub(child);
        getNextToken();
    }
    //Now we're supposed to reach the Main block with the keyword "demarre"
    //Create a child node for the subTree "<Main>"
    //Add to the subtree Main the keywords "demarre" and "<block>" as children
    //Call upon teh function responsible for checking the <block> rule and pass to it its subTree root
    if(strcmp(nextToken, "demarre") == 0){
        child = addChild(root, "<Main>");
        addChild(child, "demarre");
        child = addChild(child, "<block>");
        getNextToken();
        blockSub(child);
    }
    //If none of the previous conditions are satisfied, then there must be an error in the token stream
    else
        error();
    //If we reach this line, that means no error was reported, and thus the code is accepted
    printf("ACCEPTED!\n");
    //We can now traverse the generated tree
    traverseTree(root, 1, parseTree);
}
//This function checks if the subsequence of the token stream that's used to define a constant adheres to the rules of the grammar
//This is how its token stream should look like to pass the test:
//"definis" <ID> ":" <char literal> OR <StrLiteral> OR "<NumLiteral>"
void defineSub(node* root){
    node* n1, *n2;
    addChild(root, "definis");
    getNextToken();
    if(isIdentifier() == 1){
        n1 = addChild(root, "<ID>");
        addChild(n1, nextToken);
        getNextToken();
        if(strcmp(nextToken, ":") == 0){
            addChild(root, ":");
            getNextToken();
            if(isCharacter() == 1 || isStringLiteral() == 1 || isNumLiteral() == 1){
                if(isCharacter() == 1){
                    n2 = addChild(root, "<CharLiteral>");
                }
                else if(isStringLiteral() == 1){
                    n2 = addChild(root, "<StrLiteral>");
                }
                else if(isNumLiteral() == 1){
                    n2 = addChild(root, "<NumLiteral>");
                }
                addChild(n2, nextToken);
                return;
            }
        }
    }
    error();
}

//This function checks if the subsequence of the token stream that's used to define a function adheres to the rules of the grammar
//This is how its token stream should look like to pass the test:
//"fonction" <ID> "(" [<parameter>]{, <parameter>} ")" "renvoi" <Data type> <block>
//The success of the function definition depends on the success of the "block" subprogram
void funDefSub(node* root){
    int flag;
    node* n1, *n2, *child;
    addChild(root, "fonction");
    getNextToken();
    if(isIdentifier() == 1){
        n1 = addChild(root, "<ID>");
        addChild(n1, nextToken);
        getNextToken();
        if(isOpenParen() == 1){
            addChild(root, "(");
            getNextToken();
            flag = 1;
            while(isCloseParen() == 0){
                n2 = addChild(root, "<parameter>");
                parameterSub(n2);
                flag++;
                getNextToken();
                if(isComma() == 1 && flag > 1){
                    addChild(root, ",");
                    getNextToken();
                    continue;
                }
                else if(isCloseParen() == 0){
                    error();
                }
            }
            addChild(root, ")");
            getNextToken();
            if(strcmp(nextToken, "renvoi") == 0){
                addChild(root, nextToken);
                getNextToken();
                if(isDataType() == 1){
                    n2 = addChild(root, "<DataType>");
                    addChild(n2, nextToken);
                    getNextToken();
                    child = addChild(root, "<block>");
                    blockSub(child);
                    return;
                }
            }
        }
    }
    error();
}

//This is used to check if a parameter declaration adheres to the rules of the grammar
//i.e., <Datatype> <ID>
void parameterSub(node* root){
    node* n1, *n2;
    if(isDataType() == 1){
        n1 = addChild(root, "<DataType>");
        addChild(n1, nextToken);
        getNextToken();
        if(isIdentifier() == 1){
            n2 = addChild(root, "<ID>");
            addChild(n2, nextToken);
            return;
        }
    }
    error();
}

//This function checks if the subsequence of the token stream that's used to declare a variable adheres to the rules of the grammar
//It first checks if the declaration is for an integer type variable or a character type variable
//Then it calls upon the corresponding subroutine
void varDec(node* root){
    node* n1;
    if(strcmp(nextToken, "entier") == 0){
        n1 = addChild(root, "<entierDeclaration>");
        addChild(n1, "entier");
        getNextToken();
        entierDec(n1);
    }
    else if(strcmp(nextToken, "car") == 0){
        n1 = addChild(root, "<characterDeclaration>");
        addChild(n1, "car");
        getNextToken();
        carDec(n1);
    }
    else{
        error();
    }
}

//This function checks if the subsequence of the token stream that's used to declare an integer adheres to the rules of the grammar
//It also does a part of type checking by making sure that the integer if initialized is initialized to a num literal
void entierDec(node* root){
    node* n1, *n2;
    if(isIdentifier() == 1){
        n1 = addChild(root, "<ID>");
        addChild(n1, nextToken);
        getNextToken();
        if(isDot() == 1){
            addChild(root, ".");
            return;
        }
        else if(isColon() == 1){
            addChild(root, ":");
            getNextToken();
            if(isNumLiteral() == 1){
                n2 = addChild(root, "<NumLiteral>");
                addChild(n2, nextToken);
            }
            else if(isIdentifier() == 1){
                n2 = addChild(root, "<ID>");
                addChild(n2, nextToken);
            }
            else{
                error();
            }
            getNextToken();
            if(isDot() == 1){
                addChild(root, ".");
                return;
            }
            else if(isComma() == 1){
                goto L;
            }
            else{
                error();
            }
        }
        else if(isComma() == 1){
            L:
                addChild(root, ",");
                getNextToken();
                entierDec(root);
        }
        else{
            error();
        }
    }
    else{
        error();
    }
}

//This function checks if the subsequence of the token stream that's used to declare a character adheres to the rules of the grammar
//It also does a part of type checking by making sure that the character if initialized is initialized to a character literal
void carDec(node* root){
    node* n1, *n2;
    if(isIdentifier() == 1){
        n1 = addChild(root, "<ID>");
        addChild(n1, nextToken);
        getNextToken();
        if(isDot() == 1){
            addChild(root, ".");
            return;
        }
        else if(isColon() == 1){
            addChild(root, ":");
            getNextToken();
            if(isCharacter() == 1){
                n2 = addChild(root, "<CharLiteral>");
                addChild(n2, nextToken);
            }
            else if(isIdentifier() == 1){
                n2 = addChild(root, "<ID>");
                addChild(n2, nextToken);
            }
            else{
                error();
            }
            getNextToken();
            if(isDot() == 1){
                addChild(root, ".");
                return;
            }
            else if(isComma() == 1){
                goto L;
            }
            else{
                error();
            }
        }
        else if(isComma() == 1){
            L:
                addChild(root, ",");
                getNextToken();
                carDec(root);
        }
        else{
            error();
        }
    }
    else{
        error();
    }
}

void blockSub(node* root){
    node* n1, *n2, *n3;
    if(strcmp(nextToken, "debut") == 0){
        addChild(root, "debut");
        getNextToken();
        while(isDataType() == 1){
            n1 = addChild(root, "<VariableDeclaration>");
            varDec(n1);
            getNextToken();
        }
        while(isStatement() == 1){
            n1 = addChild(root, "<Statement>");
            if(strcmp(nextToken, "si") == 0){
                n2 = addChild(n1, "<SelectionStatement>");
                addChild(n2, "si");
                getNextToken();
                condSub(n2);
            }
            else if(strcmp(nextToken, "tantque") == 0){
                n2 = addChild(n1, "<RepetitionStatement>");
                addChild(n2, "tantque");
                getNextToken();
                loopSub(n2);
                getNextToken();
            }
            else if(strcmp(nextToken, "renvoi") == 0){
                n2 = addChild(n1, "<ReturnStatement>");
                addChild(n2, "renvoi");
                getNextToken();
                if(isIdentifier() == 1){
                    n3 = addChild(n2, "<ID>");
                    addChild(n3, nextToken);
                }
                else if(isNumLiteral() == 1){
                    n3 = addChild(n2, "<NumLiteral>");
                    addChild(n3, nextToken);
                }
                else{
                    error();
                }
                getNextToken();
                if(isDot() == 1){
                    addChild(n2, ".");
                    getNextToken();
                    return;
                }
                error();
            }
            else if(strcmp(nextToken, "scanne") == 0){
                n2 = addChild(n1, "<FunctionCall>");
                n3 = addChild(n2, "<PredefinedFunction>");
                addChild(n3, "scanne");
                getNextToken();
                scanSub(n3);
                getNextToken();
            }
            else if(strcmp(nextToken, "affiche") == 0){
                n2 = addChild(n1, "<FunctionCall>");
                n3 = addChild(n2, "<PredefinedFunction>");
                addChild(n3, "affiche");
                getNextToken();
                afficheSub(n3);
                getNextToken();
            }
            else if(isIdentifier() == 1){
                n2 = addChild(n1, "<AssignmentStatement>");
                asgSub(n2);
                getNextToken();
            }
        }
        if(strcmp(nextToken, "fin") == 0){
            addChild(root, "fin");
            return;
        }
    }
    error();
}

int isStatement(){
    if(strcmp(nextToken, "si") == 0 || strcmp(nextToken, "tantque") == 0 || strcmp(nextToken, "renvoi") == 0 || isIdentifier() == 1 || strcmp(nextToken, "scanne") == 0 || strcmp(nextToken, "affiche") == 0){
        return 1;
    }
    return 0;
}

void condSub(node* root){
    node* n1, *n2;
    if(isOpenParen() == 1){
        n1 = addChild(root, "<RelationalExpression>");
        addChild(n1, nextToken);
        getNextToken();
        relExprSub(n1);
        getNextToken();
        if(isCloseParen() == 1)
            addChild(n1, nextToken);
        else
            error();
    }
    else if(isOpenBrack() == 1){
        n1 = addChild(root, "<LogicalExpression>");
        addChild(n1, nextToken);
        getNextToken();
        logExprSub(n1);
        if(isCloseBrack() == 1)
            addChild(n1, "]");
        else
            error();
    }
    getNextToken();
    n2 = addChild(root, "<block>");
    blockSub(n2);
    getNextToken();
    if(strcmp(nextToken, "sinon") == 0){
        addChild(root, "sinon");
        getNextToken();
        if(strcmp(nextToken, "si") == 0){
            addChild(root, "si");
            condSub(root);
        }
        else{
            n2 = addChild(root, "<block>");
            blockSub(n2);
        }
    }
    else{
        return;
    }
}

void logExprSub(node* root){
    node* n1, *n2;
    n2 = addChild(root, "<andFirst>");
    andFirstSub(n2);
    if(isOu() == 1){
        addChild(root, nextToken);
        getNextToken();
        logExprSub(root);
    }
    else if(isCloseBrack() == 0){
        error();
    }
}

void andFirstSub(node* root){
    node* n1;
    if(isPas() == 1){
        addChild(root, "pas");
        getNextToken();
    }
    while(1){
        n1 = addChild(root, "<RelationalExpression>");
        relExprSub(n1);
        getNextToken();
        if(isEt() == 1){
            addChild(root, nextToken);
            getNextToken();
        }
        else{
            return;
        }
    }
}

void relExprSub(node* root){
    node* n1;
    int twice = 1;
    V:
        if(isIdentifier() == 1){
            n1 = addChild(root, "<ID>");
        }
        else if(isNumLiteral() == 1){
            n1 = addChild(root, "<NumLiteral>");
        }
        else if(isCharacter() == 1){
            n1 = addChild(root, "<CharLiteral>");
        }
        else
            error();
        addChild(n1, nextToken);
        if(twice == 2){
            return;
        }
    getNextToken();
    if(isRelOp() == 1){
        addChild(root, nextToken);
        getNextToken();
        twice = 2;
        goto V;
    }
    error();
}


void scanSub(node* root){
    node* n1;
    if(isOpenParen() == 1){
        addChild(root, "(");
        getNextToken();
        if(isIdentifier() == 1){
            n1 = addChild(root, "<ID>");
            addChild(n1, nextToken);
        }
        else{
            error();
        }
        getNextToken();
        if(isCloseParen() == 1){
            addChild(root, ")");
            getNextToken();
            if(isDot() == 1){
                addChild(root, ".");
                return;
            }
        }
    }
}

void afficheSub(node* root){
    node* n1;
    if(isOpenParen() == 1){
        addChild(root, "(");
        getNextToken();
        if(isNumLiteral() == 1){
            n1 = addChild(root, "<NumLiteral>");
        }
        else if(isCharacter() == 1){
            n1 = addChild(root, "<CharLiteral>");
        }
        else if(isIdentifier() == 1){
            n1 = addChild(root, "<ID>");
        }
        else if(isStringLiteral() == 1){
            n1 = addChild(root, "<StrLiteral>");
        }
        else{
            error();
        }
        addChild(n1, nextToken);
        getNextToken();
        if(isCloseParen() == 1){
            addChild(root, ")");
            getNextToken();
            if(isDot() == 1){
                addChild(root, ".");
                return;
            }
        }
    }
}

int isRelOp(){
    if(strcmp(nextToken, "=") == 0 || strcmp(nextToken, ">") == 0 || strcmp(nextToken, "<") == 0 || strcmp(nextToken, "?") == 0){
        return 1;
    }
    return 0;
}

int isArithOp(){
    if(strcmp(nextToken, "+") == 0 || strcmp(nextToken, "-") == 0 || strcmp(nextToken, "/") == 0 || strcmp(nextToken, "*") == 0){
        return 1;
    }
    return 0;
}

void loopSub(node* root){
    node* n1;
    if(isOpenParen() == 1){
        n1 = addChild(root, "<RelationalExpression>");
        addChild(n1, "(");
        getNextToken();
        relExprSub(n1);
        getNextToken();
        if(isCloseParen() == 1)
            addChild(n1, ")");
        else
            error();
    }
    else if(isOpenBrack() == 1){
        n1 = addChild(root, "<LogicalExpression>");
        addChild(n1, "[");
        getNextToken();
        logExprSub(n1);
    }
    else
        error();
    getNextToken();
    blockSub(root);
    return;
}

void asgSub(node* root){
    node* n1, *n2;
    if(isIdentifier() == 1){
        n1 = addChild(root, "<ID>");
        addChild(n1, nextToken);
        getNextToken();
        if(isColon() == 1){
            addChild(root, ":");
            getNextToken();
            if(isCharacter() == 1){
                n2 = addChild(root, "<CharLiteral>");
                addChild(n2, nextToken);
                getNextToken();
                if(isDot() == 1){
                    addChild(root, ".");
                    return;
                }
                error();
            }
            else if(isOpenParen() == 1){
                n2 = addChild(root, "<FunctionCall>");
                funCallSub(n2);
                return;
            }
            else if(isNumLiteral() == 1){
                n2 = addChild(root, "<NumLiteral>");
                addChild(n2, nextToken);
                getNextToken();
                if(isDot() == 1){
                    addChild(root, ".");
                    return;
                }
                error();
            }
            else if(isIdentifier() == 1){
                n2 = addChild(root, "<ID>");
                addChild(n2, nextToken);
                getNextToken();
                if(isDot() == 1){
                    addChild(root, ".");
                    return;
                }
                error();
            }
            else if(isOpenBrack() == 1){
                n2 = addChild(root, "<ArithmeticExpression>");
                addChild(n2, "[");
                arithExprSub(n2);
                return;
            }
        }
    }
    error();
}

void arithExprSub(node* root){
    node* n1, *n2;
    getNextToken();
    n1 = addChild(root, "<term>");
    termSub(n1);
    if(strcmp(nextToken, "+") == 0 || strcmp(nextToken, "-") == 0){
        addChild(root, nextToken);
        arithExprSub(root);
    }
    else if(isCloseBrack() == 1){
        addChild(root, nextToken);
        getNextToken();
        if(isDot() == 1){
            addChild(root, nextToken);
            return;
        }
    }
    else{
        error();
    }
}

void termSub(node* root){
    node* n1, *n2;
    n1 = addChild(root, "<factor>");
    while(isIdentifier() == 1 || isNumLiteral() == 1){
        if(isIdentifier() == 1){
            n2 = addChild(n1, "<ID>");
        }
        else if(isNumLiteral() == 1){
            n2 = addChild(n1, "<NumLiteral>");
        }
        else{
            error();
        }
        addChild(n2, nextToken);
        getNextToken();
        if(strcmp(nextToken, "*") == 0 || strcmp(nextToken, "/") == 0){
            addChild(n1, nextToken);
            getNextToken();
        }
        else{
            return;
        }
    }
}

void funCallSub(node* root){
    node* n1, *n2;
    if(isOpenParen() == 1){
        addChild(root, "(");
        getNextToken();
        if(isIdentifier() == 1){
            n1 = addChild(root, "<ID>");
            addChild(n1, nextToken);
            getNextToken();
            if(isOpenParen() == 1){
                addChild(root, "(");
                getNextToken();
                int flag = 0;
                while(isCloseParen() == 0){
                    if(isIdentifier() == 1){
                        n2 = addChild(root, "<ID>");
                        addChild(n2, nextToken);
                        getNextToken();
                        flag = 1;
                    }
                    if(isComma() && flag == 1){
                        addChild(root, ",");
                        getNextToken();
                        flag = 0;
                    }
                    else if(isCloseParen() == 0){
                        error();
                    }
                }
                addChild(root, ")");
                getNextToken();
                if(isCloseParen() == 1){
                    addChild(root, ")");
                    getNextToken();
                    if(isDot() == 1){
                        addChild(root, ".");
                        return;
                    }
                }
            }
        }
    }
    error();
}



int isPas(){
    if(strcmp(nextToken, "pas") == 0){
        return 1;
    }
    return 0;

}

int isOu(){
    if(strcmp(nextToken, "ou") == 0){
        return 1;
    }
    return 0;

}

int isEt(){
    if(strcmp(nextToken, "et") == 0){
        return 1;
    }
    return 0;

}

int isIdentifier(){
    int i;
    for(i = 0; i < RWC; i++){
        if(strcmp(nextToken, reservedWords[i]) == 0){
            return 0;
        }
    }
    int j = 0;
    if(nextToken[0] == '*'){
        j = 1;
    }
    if(isalpha(nextToken[j]) != 0){
        for(i = j + 1; i < strlen(nextToken); i++){
            if(isalpha(nextToken[i]) == 0 && isdigit(nextToken[i]) == 0){
                return 0;
            }
        }
    }
    else{
        return 0;
    }
    return 1;
}

int isCharacter(){
    if(nextToken[0] == '\'' && isprint(nextToken[1]) != 0 && nextToken[2] == '\''){
        return 1;
    }
    return 0;
}

int isStringLiteral(){
    int idx = strlen(nextToken)-1;
    if(nextToken[0] == '"' && nextToken[idx] == '"'){
        for(int i = 1; i < idx; i++){
            if(isprint(nextToken[i]) == 0){
                return 0;
            }
        }
    }
    else{
        return 0;
    }
    return 1;
}

int isNumLiteral(){
    for(int i = 0; i < strlen(nextToken); i++){
        if(nextToken[0] == '+' || nextToken[0] == '-'){
            i++;
        }
        if(isdigit(nextToken[i]) == 0){
            return 0;
        }
    }
    return 1;
}

int isCloseBrack(){
    if(strcmp(nextToken, "]") == 0){
        return 1;
    }
    return 0;
}

int isOpenBrack(){
    if(strcmp(nextToken, "[") == 0){
        return 1;
    }
    return 0;
}


int isCloseParen(){
    if(strcmp(nextToken, ")") == 0){
        return 1;
    }
    return 0;
}

int isOpenParen(){
    if(strcmp(nextToken, "(") == 0){
        return 1;
    }
    return 0;
}

int isDataType(){
    if(strcmp(nextToken, "entier") == 0 || strcmp(nextToken, "car") == 0){
        return 1;
    }
    return 0;
}

int isDot(){
    if(strcmp(nextToken, ".") == 0){
        return 1;
    }
    return 0;
}

int isColon(){
    if(strcmp(nextToken, ":") == 0){
        return 1;
    }
    return 0;
}

int isComma(){
    if(strcmp(nextToken, ",") == 0){
        return 1;
    }
    return 0;
}

void error(){
    printf("REJECTED!\n");
    printf("Unexpected token: \n\t\t\tToken %s Line %s: %s", d, n, nextToken);
    exit(0);
}


node* newNode(char data[]){
    node* newNode = (node*)malloc(sizeof(node));
    strcpy(newNode->value, data);
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

node* addSibling(node* root, char data[]){
    node* w = root;
    if(w == NULL)
        return NULL;
    while(w->sibling != NULL)
        w = w->sibling;

    return (w->sibling = newNode(data));
}

node* addChild(node* root, char data[]){
    node* w = root;
    if(w == NULL)
        return NULL;
    if(w->child != NULL)
        return addSibling(w->child, data);
    else
        w->child = newNode(data);
        return w->child;
}

void traverseTree(node* root, int level, FILE* out){
    node* w = root;
    if(root == NULL)
        return;
    while(w != NULL){
        fprintf(out, "%c", '\n');
        for(int i = 0; i < level - 1; i++){
            fprintf(out, "%c", '\t');
        }
        fprintf(out, "%d. ", level);
        fprintf(out, "%s ", w->value);
        if(w->child != NULL){
            level++;
            traverseTree(w->child, level, out);
            level--;
        }
        w = w->sibling;
    }
}

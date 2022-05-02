#include <stdio.h>
#include <string.h>
#define lines 200
#define rows 20
#define SPR "+9999999999"
#define HLT "+9000000000"
#define INPUTFILE "AL Simple Arithmetics.txt"

/*
Possible MACRO values for the input file:
Simple code: "AL Simple Arithmetics.txt"
Loop + Conditional: "AL Power.txt"
Nested Loop + Nested Conditionals: "AL Display Pattern.txt"
*/

FILE* inpf, *outf;
char symbolTable[rows][5][5];   //First column stores the symbols; Second column stores their corresponding data memory addresses
char labelTable[rows][5][5];    //First column stores the labels; Second column stores their corresponding code memory addresses
char code[lines][12];           //This array stores the final machine language code
int codeLine = 0, symbolsCount = 0, labelsCount = 0;
int err = 0;

void initData();
void createSymbol(char* name, int address, int idx);
char* getOpcode(char* op);
void readProgram();
char* getSymbol(char* name);
void createLabel(char* label, int address, int idx);
void replaceLabels();
void readInput();

void main(){
    inpf = fopen(INPUTFILE, "r");
    outf = fopen("ML Code.txt", "w");
    if(inpf == NULL){
        printf("File does not exist! Please make sure to change the file name in the macro above to the desired file!");
        return;
    }

    //Read data initialization part
    initData();
    //Read program part
    readProgram();
    //Replace labels by their absolute code memory addresses
    replaceLabels();
    //Read inputs
    readInput();

    //Write the array storing the code to an output file
    for(int i = 0; i < codeLine; i++){
        fprintf(outf, "%s\n", code[i]);
    }

    if(err != 0){
        printf("Unsuccessful Assembly!");
        return;
    }
    printf("Successful Assembly!");
}

//This function is responsible for reading the data initialization part and building the symbol table
void initData(){
    char op[4], name[5], ch;
    int size, address = 0, idx = 0, j;
    fscanf(inpf, "%s", op);
    while(strcmp(op, "SPR") != 0){
        if(strcmp(op,"DEC") == 0){              //If the opcode is a symbol declaration
            fscanf(inpf, "%s %d", name, &size);
            createSymbol(name, address, idx);   //Call the function createSymbol to add the corresponding symbol entry to the symbol table
            address = address + 1;                          //Increment the data memory address location
            idx++;
        }
        else{   //Otherwise (it's not a symbol declaration)
            //Add the code line to the array code while omitting spaces
            j = 0;
            code[codeLine][j++] = op[0];
            code[codeLine][j++] = op[1];
            while((ch = getc(inpf)) != '\n'){
                if(ch != ' '){
                    code[codeLine][j++] = ch;
                }
            }
            codeLine++;
        }
        fscanf(inpf, "%s", op);
    }
    //Add the separator to signify the end of the initialization part
    strcpy(code[codeLine++], SPR);
}

//This function is responsible for adding symbol entries in the symbol table
void createSymbol(char* name, int address, int idx){
    char absAdd[5];
    sprintf(absAdd, "%0*d", 4, address);        //Convert the address from integer to string and add to it preceding zeros
    sprintf(symbolTable[idx][0],"%s", name);    //Write the symbol name into the first column entry of the symbol table
    sprintf(symbolTable[idx][1],"%s", absAdd);  //Write the address into the second column entry of the symbol table
    symbolsCount++;         //Increment the number of symbols
}

//This function is responsible for reading the program part and building the label table
void readProgram(){
    char opcode[4], operand1[5], operand2[5];
    char type;
    int lineNumber = 0, idx = 0;
    fscanf(inpf, "%s %c %s %s", opcode, &type, operand1, operand2); //Read a code line
    while(strcmp(opcode, "SPR")){   //Keep reading the code part till we reach the separator opcode
        strcpy(opcode, getOpcode(opcode));  //Replace the opcode with its machine language sign
        if(opcode == NULL){     //If the opcode does not belong to the list of opcodes allowed by our language
            printf("Error: Invalid Opcode!");
            err++;              //Keep track of the number of errors
        }
        else if(strcmp(opcode, "+9") == 0){  //If the opcode is a halt instruction
            sprintf(code[codeLine++],HLT);   //Add the code line to the array that stores the code
            lineNumber++;       //Increment the number of lines in output program code
        }
        else if(strcmp(opcode, "LBL") != 0){    //If the opcode is not a labeling instruction
            if(getSymbol(operand1) != NULL){
                strcpy(operand1, getSymbol(operand1));
            }
            if(getSymbol(operand2) != NULL){
                strcpy(operand2, getSymbol(operand2));
            }
            sprintf(code[codeLine++],"%s%c%s%s", opcode, type, operand1, operand2);   //Add the code line to the array that stores the code
            lineNumber++;   //Increment the number of lines in out program code
        }
        else{               //If the opcode is a label declaration instruction
            createLabel(operand1, lineNumber, labelsCount);     //Call the function create label to add the corresponding label entry
        }
        fscanf(inpf, "%s ", opcode);    //Scan the next opcode
        if(strcmp(opcode, "HLT") != 0 && strcmp(opcode, "SPR") != 0){   //If the opcode is not a halt nor a separator
            fscanf(inpf, "%c %s %s", &type, operand1, operand2);    //Scan the rest of the instruction
        }
    }
    strcpy(code[codeLine++], SPR);  //Add a separator to the array storing the code to signify the end of the program
}

//This function is responsible for mapping between assembly language opcodes and machine language opcodes
char* getOpcode(char* op){
    char* match[18][2] = {{"ASN","+0"}, {"MOV", "-0"},{"INC", "+1"}, {"DCR", "-1"}, {"ADD", "+2"}, {"SUB", "-2"}, {"MUL", "+3"},
    {"DIV", "-3"}, {"EQL", "+4"}, {"NEQ", "-4"}, {"IGT", "+5"}, {"ILT", "-5"}, {"JMP", "+6"}, {"INP", "+7"},
    {"OUT", "-7"}, {"SPR", "+9"}, {"HLT", "+9"},  {"LBL", "LBL"}};

    for(int i = 0; i < 18; i++){
        if(strcmp(op, match[i][0]) == 0){
            return match[i][1];
        }
    }
    return NULL;
}

//This function retrieves a symbol from the symbol table (if it exists) and returns its corresponding memory address
char* getSymbol(char* name){
    int i;
    for(i = 0; i < symbolsCount ; i++){
        if(strcmp(name, symbolTable[i][0]) == 0){
            return symbolTable[i][1];
        }
    }
    return NULL;
}

//This function is responsible for adding label entries to the label table
void createLabel(char* label, int address, int idx){
    char absAdd[5];
    sprintf(absAdd, "%0*d", 4, address);
    sprintf(labelTable[idx][0],"%s", label);
    sprintf(labelTable[idx][1],"%s", absAdd);
    labelsCount++;
}

//This function is responsible for replacing labels with their absolute code memory addresses using the label table
void replaceLabels(){
    int i,j;
    for(i = 0; i < codeLine; i++){
        char* ptr = &code[i][7];
        for(j = 0; j < labelsCount; j++){
            if(strcmp(ptr, labelTable[j][0]) == 0){
                strcpy(ptr, labelTable[j][1]);
            }
        }
    }
}

//This function is responsible for reading the input part from the assembly code file and writing it to the array storing the code
void readInput(){
    int j = 0;
    char ch;
    while((ch = getc(inpf)) != EOF){
        if(ch != ' '){
            code[codeLine][j++] = ch;
        }
    }
    codeLine++;
}

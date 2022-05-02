#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fillDataMemory(int* dataMemory, FILE* inpf);           //Reads the data initialization part from the ML code to the data memory array
void fillCodeMemory(char codeMemory[][12], FILE* inpf);     //Reads the code part from the ML code to the code memory array
void readInput(int* dataMemory, FILE* inpf, int address);   //Reads an input from the input data part of the ML code and stores it in the specified address of the data memory array
int extractOperand(char* instruction, int beg);    //Extracts an operand from the specified instruction and returns it as an integer

void main(){
    FILE* inpf;
    int ACC, IP = 0;
    char opcode[3];
    char type;      //Describes the type of each operand (address, literal value or accumulator)
    int operand1, operand2; //Operands as written in the instruction
    int value1, value2;     //Values of operands (if the operand is an address, it's value is whatever is stored in the address)
    int dataMemory[10000];
    char codeMemory[10000][12];

    inpf = fopen("ML Code.txt", "r");    //Open the input file
    if(inpf == NULL){
        printf("File not found!");
        return;
    }

    fillDataMemory(dataMemory, inpf);   //Reading data initialization part
    fseek(inpf, 11, SEEK_CUR);          //To skip the separat_or and the newline character because it shouldn't be in the code memory

    fillCodeMemory(codeMemory, inpf);   //Reading the code into the code Memory array
    fseek(inpf, 11, SEEK_CUR);          //To skip the separator and the newline character because it shouldn't be in the code memory


    //Start Decoding the Code Instructions

    strncpy(opcode,codeMemory[IP],2);         //Extract the opcode
    while(strcmp(opcode, "+9") != 0){
        type = codeMemory[IP][2];      //Extract the type of the operands
        operand1 = extractOperand(codeMemory[IP], 3);    //Extract the first operand
        operand2 = extractOperand(codeMemory[IP], 7);    //Extract the second operand

        //The following conditional statements are used to find the values of the operands depending on their type
        if(type == '1' || type == '2' || type == '3'){
            value1 = -operand1;
            if(type == '1'){
                value2 = -operand2;
            }
            else if(type == '2'){
                value2 = operand2;
            }
            else if(type == '3'){
                value2 = dataMemory[operand2];
            }
        }
        else if(type == '4' || type == '5' || type == '6'){
            value1 = operand1;
            if(type == '4'){
                value2 = -operand2;
            }
            else if(type == '5'){
                value2 = operand2;
            }
            else if(type == '6'){
                value2 = dataMemory[operand2];
            }
        }
        else if(type == '7' || type == '8' || type == '9'){
            value1 = dataMemory[operand1];
            if(type == '7'){
                value2 = -operand2;
            }
            else if(type == '8'){
                value2 = operand2;
            }
            else if(type == '9'){
                value2 = dataMemory[operand2];
            }
        }
        else if(type == '0'){
            value1 = operand1;
            value2 = operand2;
        }

        //Start Executing the code instructions
        if(strcmp(opcode, "+0") == 0){
            dataMemory[operand2] = value1;
        }
        else if(strcmp(opcode, "-0") == 0){
            dataMemory[operand2] = ACC;
        }
        else if(strcmp(opcode, "+1") == 0){
            dataMemory[operand2] += value1;
        }
        else if(strcmp(opcode, "-1") == 0){
            dataMemory[operand2] -= value1;
        }
        else if(strcmp(opcode, "+2") == 0){
            ACC = value1 + value2;
        }
        else if(strcmp(opcode, "-2") == 0){
            ACC = value1 - value2;
        }
        else if(strcmp(opcode, "+3") == 0){
            ACC = value1 * value2;
        }
        else if(strcmp(opcode, "-3") == 0){
            ACC = value1 / value2;
        }
        else if(strcmp(opcode, "+4") == 0){
            if(value1 != value2){
                IP += 2;        //This is to skip the jump statement if the if statement is not satisfied
                strncpy(opcode,codeMemory[IP],2);
                continue;
            }
        }
        else if(strcmp(opcode, "-4") == 0){
            if(value1 == value2){
                IP += 2;        //This is to skip the jump statement if the if statement is not satisfied
                strncpy(opcode,codeMemory[IP],2);
                continue;
            }
        }
        else if(strcmp(opcode, "+5") == 0){
            if(value1 <= value2){
                IP += 2;        //This is to skip the jump statement if the if statement is not satisfied
                strncpy(opcode,codeMemory[IP],2);
                continue;
            }
        }
        else if(strcmp(opcode, "-5") == 0){
            if(value1 >= value2){
                IP += 2;        //This is to skip the jump statement if the if statement is not satisfied
                strncpy(opcode,codeMemory[IP],2);
                continue;
            }
        }
        else if(strcmp(opcode, "+6") == 0){
            IP = operand2;          //Jump to the code memory address stored in the second operand
            strncpy(opcode,codeMemory[IP],2);    //Modify the opcode to take the opcode of the next instruction
            continue;
        }
        else if(strcmp(opcode, "+7") == 0){
            readInput(dataMemory, inpf, operand2);
        }
        else if(strcmp(opcode, "-7") == 0){
            printf("%d  ", value1);  //Print the value of the first operand
        }
        IP++;                           //Increment the IP
        strncpy(opcode,codeMemory[IP],2);     //Modify the opcode to take the opcode of the next instruction
        }
        fclose(inpf);
}

void fillDataMemory(int* dataMemory, FILE* inpf){
    /*This loop reads in the data initialization into the data memory array
    It stops once reaching the opcode of the separator which should not be read in.*/
    for(int i = 0; getc(inpf) != '+' , getc(inpf) != '9'; i++){
        fseek(inpf, -2, SEEK_CUR); //The file cursor should go one character back, so that the sign gets scanned with the initialization value
        fscanf(inpf, "%d\n", &dataMemory[i]);
    }
}

void fillCodeMemory(char codeMemory[][12], FILE* inpf){
    int i = 0;
    char ch;
    /*The do-while executes the instructions in the body, then checks for the condition,
    this helps us assure that the halt instruction that has the '|' opcode will be scanned into the code memory array*/
    do{
        fscanf(inpf, "%s\n", codeMemory[i++], ch);//Read an instruction from the input file and store it in the current code memory address (i)
    }while(codeMemory[i-1][1] != '9');      //Stop when reaching a separator
}

void readInput(int* dataMemory, FILE* inpf, int address){
    char str[20];
    fscanf(inpf, "%d\n", &dataMemory[address]); //Read the input value into the specified data memory location
}

int extractOperand(char* instruction, int beg){
    char opStr[5];          //Stores the operand as a string
    int operand;            //Stores the operand as an integer
    //This loop reads an operand from the instruction into the operand string
    for(int c = 0; c < 4; c++, beg++){
        opStr[c] = instruction[beg];
    }
    operand = atoi(opStr);  //Converts the numerical string of the operand into an integer
    return operand;
}

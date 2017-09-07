#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "emulator.h"

Node *memSize;
regs *Registers;
int locationOfText;
int overflowFlag = 0;;
int zeroFlag = 0;
int negativeFlag = 0;
int sizeArr;
int locationOfString = 0;



// loop in .byte one
void printHelp(){
    printf("USAGE:\n");
    printf("./emulator <Y86_filename>\n OR \n");
    printf("./emulator -h to display this message. \n");
}

void get(char *lineIn){
    
    //iterators
    int len=0,i=0,j=0,k=0,l=1;
    
    char *address=malloc(sizeof(char)*12);
    
    // gets address from each line
    
    
    int hexToDec=0;
    char * addressOf = NULL ;
    
    //used for .string
    char *string;
    
    //used for .size
    int size=0;
    
    //used for .long
    int lng=0;
    
    
    //used for .bss
    int decSize=0;
    
    // printf("\nNEW LINE\n");
    //l=1;
    //printf("LINE: %s \n", lineIn);
    if (lineIn[0]!='.') {
        return;
    }
    while(l<10) {
        if (!isalnum(lineIn[l])) {
            break;
        }
        l++;
    }
    // printf("L: %d\n", l);
    for (j=l+1; j<strlen(lineIn); j++) {
        if (!isalnum(lineIn[j])) {
            break;
        }
        address[k]=lineIn[j];
        k++;
        
    }
    if (lineIn[1]=='b') {
        if (lineIn[2]=='s') {                           //.bss
            addressOf = &lineIn[j+1];
            decSize = atoi(addressOf);
            printf("1bss\n");
        }else{                                          //.byte
            // printf("ADDRESS: %s\n", address);
            //  printf("Byte: %d\n", lng);
            addressOf = &lineIn[j+1];
            hexToDec = (int)strtol(address, NULL, 16);
            //  printf("hextodec: %d\n", hexToDec);
            
            sprintf(memSize[hexToDec].data,"%d", (int)strtol(addressOf, NULL, 16));
            // printf("ADDRESS: %s\n", address);
            //printf("BYTE: %s\n", addressOf);
        }
    } else if (lineIn[1]=='s') {
        if (lineIn[2]=='i') {                           //.size
            len=strlen(lineIn)-4;
            // printf("len %lu\n", strlen(lineIn));
            /* for (i=len; i<strlen(lineIn)-1; i++) {
             size+=(lineIn[i]-'0')*pow(10, exp);
             exp--;
             }*/
            size = (int)strtol(address, NULL, 16);
            if(size<0){
                return;
            }
            sizeArr = size;
            memSize = malloc(sizeof(Node)*(size*4));
            //sprintf(Registers->regList[ESP].data, "%d", size);
            // printf("size %d\n", size);
            size=0;
            
        }else{                                          //.string
            string = malloc(sizeof(char)*(strlen(lineIn))-1-j);
            strcpy(string, lineIn);
            for (i = 0; i<strlen(string); i++) {
                if (string[i]=='"') {
                    string[strlen(string)-1] = '\0';
                    addressOf = &string[i+1];
                    //      printf("ADDRESS: %s\n",address);
                    //    printf("STRING: %s\n",addressOf);
                }
            }
            hexToDec=(int)strtol(address, NULL, 16);
            locationOfString = hexToDec;
            if(hexToDec<0){
                
                return;
            }
            /*
             printf("STRING: %s\n",addressOf);
             for (balls=0; balls<strlen(addressOf); balls++) {
             memSize[hexToDec+balls].data[0] = addressOf[balls];
             }
             
             */
            strcpy(memSize[hexToDec].data,addressOf);
        }
    }
    
    else if (lineIn[1]=='t') {                          //.text
        
        addressOf = &lineIn[j+1];
        //printf("ADDRESS: %s\n",address);
        //printf("TEXT: %s\n",addressOf);
        hexToDec = (int)strtol(address, NULL, 16);
        //printf("hex to dec %d\n",hexToDec);
        strcpy(memSize[hexToDec].data,addressOf);
        
        locationOfText = hexToDec;
    }
    
    else if (lineIn[1]=='l') {                          //.long
        
        addressOf = &lineIn[j+1];
        lng=atoi(addressOf);
        //printf("ADDRESS: %s\n", address);
        //printf("LONG: %d\n", lng);
        hexToDec=(int)strtol(address, NULL, 16);
        if(hexToDec<0){
            //  printf("INVALID LONG");
            return;
        }
        strcpy(memSize[hexToDec].data, addressOf);
        
        //printf("LONG IN MEM %s\n",(char*)memSize[hexToDec].data);
        //printf("hextodec: %d\n", hexToDec);
        //printf("%s\n", (char*)memSize[hexToDec].data);
        
    }
    
    else {                                              //ERROR
        printf("LINE: %s (ENDLINE)\n", lineIn);
        
        free(memSize);
        free(Registers);
        exit(EXIT_FAILURE);
    }
}


void decode(char * text){
    int i=0; //i is essentially the program counter
    
    int j=0,k=0,l=0,p=0; //iterators
    int balls;
    int ballsy = 0;
    int ballsier;
    
    long movsbl = 0;
    
    int reg1=0, reg2=0;
    char h;
    int readWrite = 0;
    int numA=0, numB=0, sum = 0;
    char numAB[16];
    char substr[8];
    char temp[8];
    char *null = "";
    for (i=0; i<strlen(text); /* iteration depends on instruction */) {
        j=i+1;
        k=j+1;
        l=k+1;
        p=l+1; //for the 8 characters indicating the 32 bit displacement
        //printf("len %lu\n", strlen(text));
        
        //printf("\nINSTRUCTION: I = %d\n\n%c%c",i, text[i],text[j] );
        switch (text[i]) {
                
            case '0':       //nop
                i+=2;
                break;
                
            case '1':       //halt
                i+=2;
                printf(HALT);
                return;
                break;
                
            case '2':       //rrmovl
                sprintf(Registers->regList[text[l]-'0'].data,"%d", atoi(Registers->regList[text[k]-'0'].data));
                
                i+=4;
                break;
                
            case '3':       //irmovl
                //printf(" %c",text[l]);
                
                for (balls=0; balls<=8; balls++, p++) {
                    // printf("%s\n", substr);
                    if (balls==8) {
                        substr[balls]='\0';
                        break;
                    }
                    substr[balls] = text[p];
                }
                for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                    if (ballsier==8) {
                        temp[ballsier]='\0';
                        break;
                    }
                    // printf("balls : %d\nballsy : %d\nballsier : %d\n", balls, ballsy,ballsier);
                    
                    // printf("Before %s\n",temp);
                    temp[ballsier]=substr[balls];
                    temp[ballsier+1]=substr[ballsy];
                    //printf("After %s\n",temp);
                }
                
                //  printf("substring %s\n", substr);
                //printf(" %s\n", temp);
                strcpy(Registers->regList[text[l]-'0'].data, temp);
                sprintf(Registers->regList[text[l]-'0'].data, "%d", (int)strtol(Registers->regList[text[l]-'0'].data, NULL, 16));
                //printf("Register %d . data: %s\n",text[l]-'0',Registers->regList[text[l]-'0'].data);
                
                //printf("ESP %d\n",atoi(Registers->regList[ESP].data));
                i=p;
                break;
                
            case '4':       //rmmovl
                
                //printf("%c%c",text[k],text[l]);
                
                for (balls=0; balls<=8; balls++, p++) {
                    printf("%s\n", substr);
                    if (balls==8) {
                        substr[balls]='\0';
                        break;
                    }
                    substr[balls] = text[p];
                }
                
                for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                    if (ballsier==8) {
                        temp[ballsier]='\0';
                        break;
                    }
                    //printf("balls : %d\nballsy : %d\nballsier : %d\n", balls, ballsy,ballsier);
                    
                    //printf("Before %s\n",temp);
                    temp[ballsier]=substr[balls];
                    temp[ballsier+1]=substr[ballsy];
                    //printf("After %s\n",temp);
                }
                //printf("substring %s\n", substr);
                
                
                
                // printf("temp %d\n", (int)strtol(temp, NULL, 16));
                // printf("reg %s\n", Registers->regList[text[k]-'0'].data);
                //printf("NUM %d\n",atoi(Registers->regList[text[k]-'0'].data)+(int)strtol(temp, NULL, 16));
                
                strcpy(Registers->regList[text[l]-'0'].data, temp);
                sprintf(memSize[atoi(Registers->regList[text[k]-'0'].data)+(2*((int)strtol(temp, NULL, 16)))].data,"%s", Registers->regList[text[k]-'0'].data);
                //Registers->regList[text[k]-'0'].location = atoi(Registers->regList[text[l]-'0'].data)+(2*(int)strtol(temp, NULL, 16));
                //printf("rmmovl\n");
                strcpy(Registers->regList[text[k]-'0'].data, null);
                i=p;
                break;
                
            case '5':       //mrmovl
                
                //printf("%c%c",text[k],text[l]);
                
                for (balls=0; balls<=8; balls++, p++) {
                    // printf("%s\n", substr);
                    if (balls==8) {
                        substr[balls]='\0';
                        break;
                    }
                    substr[balls] = text[p];
                }
                for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                    if (ballsier==8) {
                        temp[ballsier]='\0';
                        break;
                    }
                    //printf("balls : %d\nballsy : %d\nballsier : %d\n", balls, ballsy,ballsier);
                    //printf("Before %s\n",temp);
                    temp[ballsier]=substr[balls];
                    temp[ballsier+1]=substr[ballsy];
                    //printf("After %s\n",temp);
                }
                
                //printf("substring %s\n", substr);
                //printf(" %s\n", temp);
                sprintf(Registers->regList[text[l]-'0'].data, "%s", memSize[atoi(Registers->regList[text[k]-'0'].data)+(2*((int)strtol(temp, NULL, 16)))].data);
                i=p;
                break;
                
            case '6':       //op1
                //printf("%c%c\n", text[k],text[l]);
                
                switch (text[j]) {
                    case '0':         //add1
                        // printf("addl\n");
                        numA = atoi(Registers->regList[text[k]-'0'].data);
                        numB = atoi(Registers->regList[text[l]-'0'].data);
                        //printf("%d + %d\n",numB,numA);
                        sum = numB+numA;
                        if ((atoi(Registers->regList[text[k]-'0'].data) > 0 && atoi(Registers->regList[text[l]-'0'].data) > 0 && sum < 0) || (atoi(Registers->regList[text[k]-'0'].data) < 0 && atoi(Registers->regList[text[l]-'0'].data) < 0 && sum > 0)) {
                            overflowFlag = 1;
                        }
                        if ((sum)==0) {
                            zeroFlag = 1;
                        }
                        if ((sum)<0) {
                            negativeFlag = 1;
                        }
                        sprintf(numAB, "%d" ,sum);
                        strcpy(Registers->regList[text[l]-'0'].data, numAB);
                        //printf("SUM: %s\n",Registers->regList[text[l]-'0'].data);
                        i+=4;
                        break;
                    case '1':         //subl
                        //printf("subl\n");
                        numA = atoi(Registers->regList[text[k]-'0'].data);
                        numB = atoi(Registers->regList[text[l]-'0'].data);
                        //printf("%d - %d\n",numB,numA);
                        sum = numB-numA;
                        if ((atoi(Registers->regList[text[k]-'0'].data) < 0 && atoi(Registers->regList[text[l]-'0'].data) > 0 && sum < 0) || (atoi(Registers->regList[text[k]-'0'].data) > 0 && atoi(Registers->regList[text[l]-'0'].data) < 0 && sum > 0)) {
                            overflowFlag = 1;
                        }
                        if ((sum)==0) {
                            zeroFlag = 1;
                        }
                        if ((sum)<0) {
                            negativeFlag = 1;
                        }
                        sprintf(numAB, "%d" ,numB);
                        //printf("numAB %s\n", numAB);
                        strcpy(Registers->regList[text[l]-'0'].data, numAB);
                        ///printf("DIFFERENCE: %s\n",Registers->regList[text[l]-'0'].data);
                        i+=4;
                        break;
                    case '2':         //andl
                        //printf("andl\n");
                        numA = atoi(Registers->regList[text[k]-'0'].data);
                        numB = atoi(Registers->regList[text[l]-'0'].data);
                        negativeFlag = 0;
                        zeroFlag = 0;
                        //printf("compare %d and %d\n",numB,numA);
                        if ((numB&numA)==0) {
                            zeroFlag = 1;
                        }
                        if ((numB&numA)<0) {
                            negativeFlag = 1;
                        }
                        sprintf(numAB, "%d" ,numB&numA);
                        
                        strcpy(Registers->regList[text[l]-'0'].data, numAB);
                        //printf("AND: %s\n",Registers->regList[text[l]-'0'].data);
                        i+=4;
                        break;
                    case '3':         //xorl
                        //printf("xorl\n");
                        numA = atoi(Registers->regList[text[k]-'0'].data);
                        numB = atoi(Registers->regList[text[l]-'0'].data);
                        //printf("XORL %d and %d\n",numB,numA);
                        if ((numB^numA)==0) {
                            zeroFlag = 1;
                        }
                        if ((numB^numA)<0) {
                            negativeFlag = 1;
                        }
                        
                        sprintf(numAB, "%d" ,numB^numA);
                        strcpy(Registers->regList[text[l]-'0'].data, numAB);
                        //printf("AND: %s\n",Registers->regList[text[l]-'0'].data);
                        i+=4;
                        break;
                    case '4':         //mull
                        //printf("mull\n");
                        numA = atoi(Registers->regList[text[k]-'0'].data);
                        numB = atoi(Registers->regList[text[l]-'0'].data);
                        //printf("%d * %d\n",numB,numA);
                        
                        if ((atoi(Registers->regList[text[k]-'0'].data) > 0 && atoi(Registers->regList[text[l]-'0'].data) > 0 && sum < 0) || (atoi(Registers->regList[text[k]-'0'].data) < 0 && atoi(Registers->regList[text[l]-'0'].data) < 0 && sum < 0) ||(atoi(Registers->regList[text[k]-'0'].data) < 0 && atoi(Registers->regList[text[l]-'0'].data) > 0 && sum > 0) || (atoi(Registers->regList[text[k]-'0'].data) > 0 && atoi(Registers->regList[text[l]-'0'].data) < 0 && sum > 0)) {
                            overflowFlag = 1;
                        }
                        
                        numB *= numA;
                        sprintf(numAB, "%d" ,numB);
                        strcpy(Registers->regList[text[l]-'0'].data, numAB);
                        //printf("PRODUCT: %s\n",Registers->regList[text[l]-'0'].data);
                        i+=4;
                        break;
                    case '5':         //cmpl
                        //printf("cmpl\n");
                        reg1 = text[k]-'0';
                        reg2 = text[l]-'0';
                        //printf("Register[%d] and Register[%d]\n", reg1,reg2);
                        i+=4;
                        break;
                    default:
                        //printf("def\n");
                        exit(EXIT_FAILURE);
                }
                break;
                
            case '7':       //jXX
                i+=10;
                //printf("%c\n",text[k]);
                //printf("JUMP\n");
                //printf("%d and %d\n", atoi(Registers->regList[reg1].data), atoi(Registers->regList[reg2].data));
                switch (text[j]) {
                        
                    case '0':         //jmp
                        //printf("jmp\n");
                        
                        for (balls=0; balls<=8; balls++, k++) {
                            //printf("%s\n", substr);
                            if (balls==8) {
                                substr[balls]='\0';
                                break;
                            }
                            substr[balls] = text[k];
                        }
                        for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                            if (ballsier==8) {
                                temp[ballsier]='\0';
                                break;
                            }
                            
                            temp[ballsier]=substr[balls];
                            temp[ballsier+1]=substr[ballsy];
                        }
                        
                        //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                        
                        //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                        //printf("mem %s\n", memSize[(int)strtol(temp, NULL, 16)].data);
                        i=2*(int)strtol(temp, NULL, 16);
                        
                        //printf("DEST: %d\n", i);
                        continue;
                    case '1':         //jle
                        
                        //printf("jle\n");
                        if(atoi(Registers->regList[reg1].data)<=atoi(Registers->regList[reg2].data)){
                            for (balls=0; balls<=8; balls++, k++) {
                                //printf("%s\n", substr);
                                if (balls==8) {
                                    substr[balls]='\0';
                                    break;
                                }
                                substr[balls] = text[k];
                            }
                            for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                                if (ballsier==8) {
                                    temp[ballsier]='\0';
                                    break;
                                }
                                
                                temp[ballsier]=substr[balls];
                                temp[ballsier+1]=substr[ballsy];
                            }
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            if ((negativeFlag == 1 || overflowFlag== 1) || zeroFlag == 1){
                                i=2*(int)strtol(temp, NULL, 16);
                                
                            }
                            //printf("DEST: %d\n", i);
                            continue;
                        }
                        break;
                        
                    case '2':         //jl
                        
                        //printf("jl\n");
                        if(atoi(Registers->regList[reg1].data)<atoi(Registers->regList[reg2].data)){
                            for (balls=0; balls<=8; balls++, k++) {
                                //printf("%s\n", substr);
                                if (balls==8) {
                                    substr[balls]='\0';
                                    break;
                                }
                                substr[balls] = text[k];
                            }
                            for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                                if (ballsier==8) {
                                    temp[ballsier]='\0';
                                    break;
                                }
                                
                                temp[ballsier]=substr[balls];
                                temp[ballsier+1]=substr[ballsy];
                            }
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            if ((negativeFlag ==1 || overflowFlag==1)){
                                i=2*(int)strtol(temp, NULL, 16);
                                
                            }
                            //printf("DEST: %d\n", i);
                            continue;
                        }
                        
                        break;
                        
                    case '3':         //je
                        
                        //printf("je\n");
                        if(atoi(Registers->regList[reg1].data)==atoi(Registers->regList[reg2].data)){
                            for (balls=0; balls<=8; balls++, k++) {
                                //printf("%s\n", substr);
                                if (balls==8) {
                                    substr[balls]='\0';
                                    break;
                                }
                                substr[balls] = text[k];
                            }
                            for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                                if (ballsier==8) {
                                    temp[ballsier]='\0';
                                    break;
                                }
                                
                                temp[ballsier]=substr[balls];
                                temp[ballsier+1]=substr[ballsy];
                            }
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            if (zeroFlag==0){
                                i=2*(int)strtol(temp, NULL, 16);
                                
                            }
                            //printf("DEST: %d\n", i);
                            continue;
                        }
                        
                        break;
                        
                    case '4':         //jne
                        
                        //printf("jne\n");
                        if(atoi(Registers->regList[reg1].data)!=atoi(Registers->regList[reg2].data)){
                            for (balls=0; balls<=8; balls++, k++) {
                                //printf("%s\n", substr);
                                if (balls==8) {
                                    substr[balls]='\0';
                                    break;
                                }
                                substr[balls] = text[k];
                            }
                            for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                                if (ballsier==8) {
                                    temp[ballsier]='\0';
                                    break;
                                }
                                
                                temp[ballsier]=substr[balls];
                                temp[ballsier+1]=substr[ballsy];
                            }
                            
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            if (zeroFlag != 1){
                                i=2*(int)strtol(temp, NULL, 16);
                                
                            }
                            //printf("DEST: %d\n", i);
                            continue;
                        }
                        
                        break;
                        
                    case '5':         //jge
                        
                        //printf("jge\n");
                        if(atoi(Registers->regList[reg1].data)>=atoi(Registers->regList[reg2].data)){
                            
                            for (balls=0; balls<=8; balls++, k++) {
                                //printf("%s\n", substr);
                                if (balls==8) {
                                    substr[balls]='\0';
                                    break;
                                }
                                substr[balls] = text[k];
                            }
                            for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                                if (ballsier==8) {
                                    temp[ballsier]='\0';
                                    break;
                                }
                                
                                temp[ballsier]=substr[balls];
                                temp[ballsier+1]=substr[ballsy];
                            }
                            
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            if (!(negativeFlag ==1 || overflowFlag==1)){
                                i=2*(int)strtol(temp, NULL, 16);
                                
                            }
                            //printf("DEST: %d\n", i);
                            continue;
                        }
                        
                        break;
                        
                    case '6':         //jg
                        //printf("jg\n");
                        
                        if(atoi(Registers->regList[reg1].data)>atoi(Registers->regList[reg2].data)){
                            
                            for (balls=0; balls<=8; balls++, k++) {
                                //printf("%s\n", substr);
                                if (balls==8) {
                                    substr[balls]='\0';
                                    break;
                                }
                                substr[balls] = text[k];
                            }
                            for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                                if (ballsier==8) {
                                    temp[ballsier]='\0';
                                    break;
                                }
                                
                                temp[ballsier]=substr[balls];
                                temp[ballsier+1]=substr[ballsy];
                            }
                            
                            
                            //i=atoi(memSize[((int)strtol(temp, NULL, 16))*2].data);
                            if (!(negativeFlag ==1 || overflowFlag==1)||zeroFlag!=0){
                                i=2*(int)strtol(temp, NULL, 16);
                                while (i>=358){
                                    i = i - 357;
                                }
                            }
                            //printf("DEST: %d\n", i);
                            //printf("DEST: %d\n", i);
                            continue;
                        }
                        break;
                        
                    default:
                        //printf("def\n");
                        exit(EXIT_FAILURE);
                }
                break;
                
            case '8':       //call
                
                i+=10;
                
                //printf("%c\n",text[k]);
                //printf("call\n");
                
                sprintf(Registers->regList[ESP].data,"%d",atoi(Registers->regList[ESP].data)-4);
                sprintf(memSize[atoi(Registers->regList[ESP].data)].data, "%d", i);
                //printf("TEMP %s\n",temp);
                for (balls=0; balls<=8; balls++, k++) {
                    //printf("%s\n", substr);
                    if (balls==8) {
                        substr[balls]='\0';
                        break;
                    }
                    substr[balls] = text[k];
                }
                for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                    //printf("%s\n", substr);
                    if (ballsier==8) {
                        temp[ballsier]='\0';
                        break;
                    }
                    
                    temp[ballsier]=substr[balls];
                    temp[ballsier+1]=substr[ballsy];
                }
                //printf("TEMP %s\n",temp);
                
                //printf("ESP is at %d\n", atoi(Registers->regList[ESP].data));
                //printf(" i on stack %s\n", memSize[atoi(Registers->regList[ESP].data)].data);
                //printf("ESP is now at %d\n", atoi(Registers->regList[ESP].data));
                //printf("TEMP %s\n",temp);
                //printf("pushl\n");
                //printf("value in mem: %d\n", atoi(memSize[((int)strtol(temp, NULL, 16))].data));
                
                /*
                 printf("string %d\n", (int)strtol(temp, NULL, 16));
                 balls = 2*atoi(temp);
                 sprintf(temp, "%d", balls);
                 printf("BALLS %d\n", balls);
                 */
                //printf("loc %ld\n", strtol(temp, NULL, 16));
                //printf("data %s\n", memSize[strtol(temp, NULL, 16)].data);
                //printf("I = %d\n", i);
                
                //i=148;
                //i=atoi(memSize[strtol(temp, NULL, 16)].data);
                //printf("I = %d\n", i);
                
                i = ((int)strtol(temp, NULL, 16)/2)-1;
                while (i>=358){
                    i = i - 357;
                }
                //printf("I = %d\n", i);
                //return;
                break;
                
                
            case '9':       //ret
                i+=2;
                
                //printf("\nret\n");
                i = atoi(memSize[atoi(Registers->regList[ESP].data)].data);
                sprintf(Registers->regList[ESP].data,"%d",atoi(Registers->regList[ESP].data)+4);
                //printf("I= %d\n",i);
                break;
                
            case 'a':       //pushl
                
                //printf("%c%c\n",text[k],text[l]); //decrement value in %esp by 1, then copy value of reg into that index in memSize
                //printf("ASS\n");
                //printf("ESP is at %s\n", Registers->regList[ESP].data);
                sprintf(Registers->regList[ESP].data,"%d",atoi(Registers->regList[ESP].data)-4);
                strcpy(memSize[atoi(Registers->regList[ESP].data)].data, Registers->regList[text[k]-'0'].data);
                //printf("ESP is now at %d\n", Registers->regList[ESP].location);
                // printf("pushl\n");
                i+=4;
                break;
                
            case 'b':       //popl
                
                //printf("%c%c\n",text[k],text[l]);  //copy value of memory at that index, save in reg,then increment value %esp by 1
                // printf("ESP is at %s\n", Registers->regList[ESP].data);
                strcpy(Registers->regList[text[k]-'0'].data, memSize[atoi(Registers->regList[ESP].data)].data);
                sprintf(Registers->regList[ESP].data,"%d",atoi(Registers->regList[ESP].data)+4);
                //printf("ESP is now at %d\n", atoi(Registers->regList[ESP].data));
                //printf("popl\n");
                i+=4;
                break;
                
            case 'c':       //readX
                
                //printf("%c%c%c\n",text[k],text[l], text[p]);
                switch (text[j]) {
                    case '0':         //readb
                        //printf("readb\n");
                        
                        for (balls=0; balls<=8; balls++, p++) {
                            // printf("%s\n", substr);
                            if (balls==8) {
                                substr[balls]='\0';
                                break;
                            }
                            substr[balls] = text[p];
                        }
                        for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                            if (ballsier==8) {
                                temp[ballsier]='\0';
                                break;
                            }
                            // printf("balls : %d\nballsy : %d\nballsier : %d\n", balls, ballsy,ballsier);
                            
                            // printf("Before %s\n",temp);
                            temp[ballsier]=substr[balls];
                            temp[ballsier+1]=substr[ballsy];
                            //printf("After %s\n",temp);
                        }
                        // printf("readb\n");
                        if (EOF != scanf("%c", memSize[atoi(Registers->regList[text[k]-'0'].data)+(2*(int)strtol(temp, NULL, 16))].data)) {
                            zeroFlag = 1;
                        }else zeroFlag = 0;
                        sprintf(memSize[atoi(Registers->regList[text[k]-'0'].data)+(2*(int)strtol(temp, NULL, 16))].data,"%d",toascii(readWrite));
                        break;
                    case '1':         //readl
                        //printf("readl\n");
                        for (balls=0; balls<=8; balls++, p++) {
                            // printf("%s\n", substr);
                            if (balls==8) {
                                substr[balls]='\0';
                                break;
                            }
                            substr[balls] = text[p];
                        }
                        for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                            if (ballsier==8) {
                                temp[ballsier]='\0';
                                break;
                            }
                            // printf("balls : %d\nballsy : %d\nballsier : %d\n", balls, ballsy,ballsier);
                            
                            // printf("Before %s\n",temp);
                            temp[ballsier]=substr[balls];
                            temp[ballsier+1]=substr[ballsy];
                            //printf("After %s\n",temp);
                        }
                        if (EOF != scanf("%d", &readWrite)) {
                            zeroFlag = 1;
                        }else zeroFlag = 0;
                        printf("\n%d\n", readWrite);
                        sprintf(memSize[atoi(Registers->regList[text[k]-'0'].data)+(2*(int)strtol(temp, NULL, 16))].data,"%d",toascii(readWrite));
                        //printf("in mem %s\n", memSize[atoi(Registers->regList[text[k]-'0'].data)+(2*(int)strtol(temp, NULL, 16))].data);
                        break;
                    default:
                        //printf("def\n");
                        exit(EXIT_FAILURE);
                }
                p+=7;
                i+=12;
                break;
            case 'd':       //writeX
                
                //printf("%c%c%c\n",text[k],text[l], text[p]);
                switch (text[j]) {
                    case '0':         //writeb
                        //printf("writeb\n");
                        
                        for (balls=0; balls<=8; balls++, p++) {
                            // printf("%s\n", substr);
                            if (balls==8) {
                                substr[balls]='\0';
                                break;
                            }
                            substr[balls] = text[p];
                        }
                        for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                            if (ballsier==8) {
                                temp[ballsier]='\0';
                                break;
                            }
                            // printf("balls : %d\nballsy : %d\nballsier : %d\n", balls, ballsy,ballsier);
                            
                            // printf("Before %s\n",temp);
                            temp[ballsier]=substr[balls];
                            temp[ballsier+1]=substr[ballsy];
                            //printf("After %s\n",temp);
                        }
                        //for (balls=0; balls<strlen(text);balls++) {
                        
                        //printf("temp %s\n", temp);
                        if (locationOfString != 0){
                            printf("%s", memSize[locationOfString].data);
                        }
                        
                        // printf("reg %s\n", Registers->regList[text[k]-'0'].data);
                        //balls=0;
                        ballsy = atoi(memSize[atoi(Registers->regList[text[k]-'0'].data)+((int)strtol(temp, NULL, 16))].data);
                        
                        
                        printf("%c", ballsy);
                        //blarg[o] = ballsy;
                        //o++;
                        
                        
                        
                        //printf("\n");
                        break;
                    case '1':         //writel
                        
                        printf("writel\n");
                        
                        for (balls=0; balls<=8; balls++, p++) {
                            // printf("%s\n", substr);
                            if (balls==8) {
                                substr[balls]='\0';
                                break;
                            }
                            substr[balls] = text[p];
                        }
                        for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                            if (ballsier==8) {
                                temp[ballsier]='\0';
                                break;
                            }
                            // printf("balls : %d\nballsy : %d\nballsier : %d\n", balls, ballsy,ballsier);
                            
                            // printf("Before %s\n",temp);
                            temp[ballsier]=substr[balls];
                            temp[ballsier+1]=substr[ballsy];
                            //printf("After %s\n",temp);
                        }
                        printf("%s\n",memSize[atoi(Registers->regList[text[k]-'0'].data)].data);
                        
                        //printf("%d\n",atoi(memSize[atoi(Registers->regList[text[l]-'0'].data)+(2*(int)strtol(temp, NULL, 16))].data));
                        break;
                    default:
                        // printf("def\n");
                        exit(EXIT_FAILURE);
                }
                p+=7;
                i+=12;
                break;
            case 'e':       //movsbl
                // printf("%c%c%c\n",text[k],text[l], text[p]);
                //printf("movsbl------------------------------\n");
                for (balls=0; balls<=8; balls++, p++) {
                    //printf("%s\n", substr);
                    if (balls==8) {
                        substr[balls]='\0';
                        break;
                    }
                    substr[balls] = text[p];
                }
                for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                    if (ballsier==8) {
                        temp[ballsier]='\0';
                        break;
                    }
                    
                    temp[ballsier]=substr[balls];
                    temp[ballsier+1]=substr[ballsy];
                }
                
                movsbl = atoi(memSize[atoi(Registers->regList[text[l]-'0'].data)].data)+(int)strtol(temp, NULL, 16);
                
                // printf("movsbl %ld\n", movsbl);
                sprintf(numAB, "%ld", movsbl);
                
                sprintf(temp,"%0lx",movsbl);
                movsbl = atoi(temp);
                //printf("before numAb %s\n", numAB);
                for (balls=6, ballsier=0, ballsy = 7; ballsier<=8; balls-=2, ballsier+=2, ballsy-=2) {
                    if (ballsier==8) {
                        numAB[ballsier]='\0';
                        break;
                    }
                    
                    numAB[ballsier]=temp[balls];
                    numAB[ballsier+1]=temp[ballsy];
                }
                
                getBin(movsbl, numAB);
                //printf("after numAb %s\n", numAB);
                if (numAB[strlen(numAB)-2] == '1') {
                    h='f';
                } else { h='0';}
                for (balls = 0; balls<=5; balls++) {
                    numAB[balls] = h;
                }
                
                //printf("temp %s\n", numAB);
                movsbl = (int)strtol(numAB, NULL, 16);
                
                //printf("movsbl %ld\n", movsbl);
                sprintf(Registers->regList[text[k]-'0'].data, "%ld", movsbl);
                //printf("NUmber in %d: %s\n",text[k]-'0', Registers->regList[text[k]-'0'].data);
                //p+=7;
                i+=12;
                //return;
                break;
                
            default:
                //printf("\nINS\n");
                exit(EXIT_FAILURE);
        }
        //printf("\nEND INS text size %lu. I = %d\n",strlen(text), i );
        //printf(OKAY);
    }
    //printf(OKAY);
}

void getBin(int num, char *str)
{
    int o;
    str[7]= '\0';
    for (o=4; o>=0; --o, num>>= 1)
    {
        str[o] = (num&1) + '0';
    }
}


char * enter(char *line1){
    return strtok(line1, "\n");
}

int main(int argc, char * argv[]) {
    FILE *y86;
    char *name;
    char line[2048];
    int c;
    if (argc < 2 || argc > 3 ){
        printf("ERROR with arguments.\n");
        return 0;
    }
    name = argv[1];
    y86 = fopen(name, "r");
    c = strcmp(name, "-h");
    if(c==0){
        printHelp();
        return 0;
    }
    if (y86 == NULL){
        printf("ERROR with arguments, NO FILE.\n");
        return 0;
    }
    Registers = malloc(sizeof(regs)*(8));
    while (fgets(line, sizeof(line), y86)){
        //printf("LINE IN LOOP: %s\n\n",line);
        get(enter(line));
    }printf("\n");
    decode(memSize[locationOfText].data);
    free(Registers);
    free(memSize);
    return 0;
}
#include <stdio.h>
// #include <string.h>
char* endOfString(char* ptr){
    while(*ptr != '\0'){
        ptr++;
    }
    ptr--;
    return(ptr);
}
int main(int argc, char* argv[]){
    // if(argc < 2){
    //     printf("Source file not provided");
    // }
    // char *sourceFile = argv[1];
    // char *lastOccurrence =  strrchr(sourceFile, '.');
    // if(lastOccurrence == NULL){
    //     return(1);
    // }
    // printf("Working on %s\n",sourceFile);
    // printf("Last charcter is %c\n",*lastOccurrence);
    // int stringLength =  strlen(sourceFile);
    // printf("The string length is: %d\n", stringLength);
    return(0);
}
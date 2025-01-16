#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here




int setup_buff(char *buff, char *user_str, int len){
    int i = 0;
    char *p = user_str;
    char last_char = 0;
    char *dst = buff;

    // Process user string and copy into buff
    while (*p && i < len - 1) {
        if (!isspace((unsigned char)*p)) {
            *dst++ = *p;
            last_char = *p;
            i++;
        } else if (last_char != ' ' && last_char != 0) { 
            *dst++ = ' ';
            last_char = ' ';
            i++;
        }
        p++;
    }

    if (*p) { 
        return -1; 
    }

    
    while (i < len) {
        *dst++ = '.';
        i++;
    }

    
    return dst - buff; 
}



void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (!isspace((unsigned char)buff[i]) && buff[i] != '.') {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    return count;
}

void reverse_string(char *buff, int actual_length) {
    int i = 0;
    int j = actual_length - 1;  
    char temp;

    while (i < j) {
        temp = buff[i];
        buff[i] = buff[j];
        buff[j] = temp;
        i++;
        j--;
    }
}



void print_words(char *buff, int str_len) {
    printf("Word Print\n----------\n");
    int word_num = 1;
    char *start = buff;
    char *current = buff;

    while (*current && (current - buff < str_len)) {
        if (!isspace((unsigned char)*current) && *current != '.') {
            start = current;
            while (*current && !isspace((unsigned char)*current) && *current != '.') {
                current++;
            }
            printf("%d. %.*s (%ld)\n", word_num++, (int)(current - start), start, (long)(current - start));
        } else {
            current++;
        }
    }
}




void search_and_replace(char *buff, const char *find, const char *replace, int buff_size) {
    int find_len = 0;
    int replace_len = 0;
    const char *temp;

    // Calculate lengths of find and replace strings
    for (temp = find; *temp; ++temp) ++find_len;
    for (temp = replace; *temp; ++temp) ++replace_len;

    // Temporary buffer to build the new string
    char *new_buff = (char *)malloc(buff_size);
    if (new_buff == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    char *dest = new_buff;
    char *src = buff;
    while (*src) {
        if (strncmp(src, find, find_len) == 0) {  
            memcpy(dest, replace, replace_len);  
            src += find_len;                      
            dest += replace_len;                 
        } else {
            *dest++ = *src++;  // Copy other characters
        }
    }
    *dest = '\0';  // Null-terminate the new string

    
    strncpy(buff, new_buff, buff_size);
    buff[buff_size - 1] = '\0';  

    free(new_buff);
}




//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             
    char *input_string;     
    char opt;               
    int  rc;                
    int  user_str_len;      

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }



    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (!buff) {
        perror("Memory allocation failed");
        exit(99);
    }

    int processed_length = setup_buff(buff, argv[2], BUFFER_SZ);
    if (processed_length < 0) {
        fprintf(stderr, "Error: Provided input string is too long.\n");
        free(buff);
        exit(3);
    }
    


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, processed_length);
            printf("Reversed String: ");
            for (int i = 0; i < processed_length; i++) {
                putchar(buff[i]);
            }
            putchar('\n');
            break;
            
        case 'w':  // Print words and their lengths
            print_words(buff, user_str_len);
            break;
        case 'x':
            if (argc != 5) {
                fprintf(stderr, "Not Implemented!\n");
                usage(argv[0]);
                free(buff);
                exit(1);
            }
            search_and_replace(buff, argv[3], argv[4], BUFFER_SZ);
            printf("Modified String: %s\n", buff);
            break;


        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
// Providing both the pointer and the length to functions can prevent buffer overflows by ensuring operations don't exceed the buffer size.
// It can also make the function interfaces explicit about how much data they are expected to handle, improving code readability and maintainability.


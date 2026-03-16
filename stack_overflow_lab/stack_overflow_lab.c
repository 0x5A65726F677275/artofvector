/**
 * @file stack_overflow_lab.c
 * @brief Demonstrates a classic stack-based buffer overflow vulnerability
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BUF_SIZE
#define BUF_SIZE 500
#endif

#define INPUT_SIZE 600

int bof(char *str) {
    char buffer[BUF_SIZE];
    strcpy(buffer, str);  // VULNERABILITY!
    return 1;
}

int main(int argc, char **argv) {
    char str[INPUT_SIZE];
    FILE *badfile;
    
    char stack_layout_dummy[BUF_SIZE];
    memset(stack_layout_dummy, 0, BUF_SIZE);
    
    badfile = fopen("badfile", "r");
    if (badfile == NULL) {
        perror("Error opening badfile");
        exit(EXIT_FAILURE);
    }
    
    size_t bytes_read = fread(str, sizeof(char), INPUT_SIZE, badfile);
    printf("[*] Read %zu bytes from badfile.\n", bytes_read);
    fclose(badfile);
    
    bof(str);
    
    printf("[*] Returned Properly. Overflow did not alter control flow.\n");
    return EXIT_SUCCESS;
}

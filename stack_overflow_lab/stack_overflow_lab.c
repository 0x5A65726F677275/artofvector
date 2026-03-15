/**
 * @file stack_overflow_lab.c
 * @author Art of Vector Research Example
 * @brief Demonstrates a classic stack-based buffer overflow vulnerability.
 *
 * This program is intentionally vulnerable for educational and research purposes.
 * It reads input from a file named "badfile" into a small stack buffer using
 * strcpy(), which does not perform bounds checking, leading to a buffer overflow.
 *
 * Compilation (disable stack protections for research):
 * gcc -o stack_overflow_lab stack_overflow_lab.c -fno-stack-protector -z execstack -no-pie
 *
 * WARNING: This code contains a critical security vulnerability.
 *          Do NOT run on production systems or with elevated privileges.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Size of the buffer in the vulnerable function. Instructors can modify this value
// to alter the stack layout, preventing the use of fixed-offset exploits from
// previous years. Recommended range: 0-400.
#ifndef BUF_SIZE
#define BUF_SIZE 500
#endif

// Size of the input read from the badfile. Must be larger than BUF_SIZE to trigger overflow.
#define INPUT_SIZE 517

/**
 * @brief A function containing a stack buffer overflow vulnerability.
 *
 * It copies the input string 'str' into a fixed-size local buffer 'buffer'
 * using the unsafe function strcpy().
 *
 * @param str The input string, expected to be read from "badfile".
 * @return Returns 1 upon completion, though overflow may alter execution flow.
 */
int bof(char *str) {
    // Local buffer allocated on the stack. Overflowing this buffer can overwrite
    // the return address and other stack data.
    char buffer[BUF_SIZE];

    /* VULNERABILITY: No bounds checking. If 'str' is longer than BUF_SIZE,
     * it will overflow 'buffer', corrupting the stack. */
    strcpy(buffer, str);

    return 1;
}

/**
 * @brief Main program entry point.
 *
 * Opens the file "badfile", reads its content into a local array, and passes
 * that data to the vulnerable bof() function.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Returns 1.
 */
int main(int argc, char **argv) {
    char str[INPUT_SIZE];
    FILE *badfile;

    /* Stack randomization mitigation: A dummy array is used to alter the stack layout.
     * Its size is tied to BUF_SIZE, forcing a re-analysis of stack offsets when
     * BUF_SIZE is changed. Initialized to zero to prevent compiler optimization. */
    char stack_layout_dummy[BUF_SIZE];
    memset(stack_layout_dummy, 0, BUF_SIZE);

    // Open the file containing the malicious input.
    badfile = fopen("badfile", "r");
    if (badfile == NULL) {
        perror("Error opening badfile");
        exit(EXIT_FAILURE);
    }

    // Read the content of the badfile.
    size_t bytes_read = fread(str, sizeof(char), INPUT_SIZE, badfile);
    if (bytes_read == 0) {
        fprintf(stderr, "Error: Could not read data from badfile or file is empty.\n");
        fclose(badfile);
        exit(EXIT_FAILURE);
    }
    printf("[*] Read %zu bytes from badfile.\n", bytes_read);
    fclose(badfile);

    // Call the vulnerable function.
    bof(str);

    // This line will not be executed if the overflow is successful and redirects execution.
    printf("[*] Returned Properly. Overflow did not alter control flow as expected.\n");
    return EXIT_SUCCESS;
}

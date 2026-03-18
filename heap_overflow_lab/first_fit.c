/**
 * Lab 1: Understanding malloc's First-Fit behavior
 * Compile: gcc -g -o first_fit first_fit.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("=== First-Fit Behavior Demo ===\n\n");
    
    // Allocate three chunks
    char *a = malloc(100);
    char *b = malloc(100);
    char *c = malloc(100);
    
    printf("Initial allocations:\n");
    printf("a = %p\n", a);
    printf("b = %p\n", b);
    printf("c = %p\n", c);
    
    // Fill with data to see them later
    strcpy(a, "AAA");
    strcpy(b, "BBB");
    strcpy(c, "CCC");
    
    // Free the middle chunk
    printf("\n[+] Freeing b...\n");
    free(b);
    
    // Allocate a new chunk of same size
    char *d = malloc(100);
    printf("[+] New allocation d = %p\n", d);
    
    // What happened? d took b's spot!
    printf("d contains: %s\n", d);  // Should see "BBB" if first-fit works
    
    // Check memory layout
    printf("\nMemory layout:\n");
    printf("a at %p: %s\n", a, a);
    printf("d at %p: %s\n", d, d);
    printf("c at %p: %s\n", c, c);
    
    // Free everything
    free(a);
    free(d);
    free(c);
    
    return 0;
}

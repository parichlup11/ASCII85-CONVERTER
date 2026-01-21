#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Helper function to convert a 32-bit number into 5 Ascii85 characters.
 * It uses modulo 85 to find the remainder and adds 33 to map it to ASCII.
 */
void code_characters(uint32_t number)
{
    for (int k = 0; k < 5; k++) {
        // Calculate the character value: remainder + offset '!' (33)
        uint32_t coded = (number % 85) + 33;
        number /= 85;
        printf("%c", coded);
    }
}

/*
 * Encodes standard input to Ascii85.
 * Reads 4 bytes, combines them into a 32-bit integer, and prints 5 characters.
 */
int encode(void)
{
    uint8_t index = 0;
    int ch;
    uint32_t number = 0;

    // Read input byte by byte
    while ((ch = getchar()) != EOF) {
        index += 1;
        // Shift existing bits and add the new byte (Big Endian style)
        number = (number << 8) | ch;

        // Once we have 4 bytes, we process them
        if (index == 4) {
            code_characters(number);
            index = 0;
            number = 0; // Reset for next block
        }
    }

    // Handle padding if the input length wasn't a multiple of 4
    if (index != 0 && index < 4) {
        // Shift remaining bytes to the left as if followed by zeros
        for (int i = index; i < 4; i++) {
            number = (number << 8);
        }
        code_characters(number);
    }
    
    printf("\n");
    return 0;
}

/*
 * Helper math function.
 * Calculates 85^exponent for the decoding process.
 */
uint32_t power_func(uint8_t exponent)
{
    uint32_t result = 1;
    for (int j = 0; j < exponent; j++) {
        int base = 85;
        result *= base;
    }
    return result;
}

/*
 * Decodes Ascii85 input back to binary.
 * Reads 5 characters, reconstructs the 32-bit integer, and prints 4 bytes.
 */
int decode(void)
{
    uint8_t index = 0;
    uint64_t tuple = 0; // 64-bit to safely detect 32-bit overflow
    int ch;

    while ((ch = getchar()) != EOF) {
        // Ignore whitespace (newlines, spaces)
        if (isspace(ch)) {
            continue;
        }

        // Validate character range (must be between '!' and 'u')
        if (ch < '!' || ch > 'u') {
            return -1;
        }

        // Add value to tuple based on its position (base 85)
        tuple += (ch - 33) * power_func(index);

        // Check if the resulting number is valid for 4 bytes
        if (tuple > UINT32_MAX) {
            return 1;
        }

        index++;

        // Once we have 5 characters, we have the full integer
        if (index != 5) {
            continue;
        }

        // Break the 32-bit integer back into 4 bytes and print them
        for (int i = 3; i >= 0; i--) {
            char ascii_char = (tuple >> (i * 8)) & 0xFF;
            printf("%c", ascii_char);
        }

        // Reset for next block
        index = 0;
        tuple = 0;
    }

    // If we have leftovers (not divisible by 5), input is invalid
    if (index > 0) {
        return 1;
    }

    printf("\n");
    return 0;
}

/*
 * Main entry point.
 * Handles arguments to switch between encode (-e) and decode (-d).
 */
int main(int argc, char *argv[])
{
    int retcode = 1;

    // Default to encode if no args or -e
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "-e") == 0)) {
        retcode = encode();
    } else if (argc == 2 && strcmp(argv[1], "-d") == 0) {
        retcode = decode();
    } else {
        fprintf(stderr, "usage: %s [-e|-d]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (retcode != 0) {
        fprintf(stderr, "an error occurred\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
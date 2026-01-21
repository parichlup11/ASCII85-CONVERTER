====================================ASCII85 CONVERTER====================================


A simple CLI tool written in C to encode and decode data using the Ascii85 
algorithm. Created as a homework assignment (HW01).

WHAT IT DOES:
- Encode: Reads 4 bytes from stdin, converts them into a 32-bit integer, and 
  calculates 5 ASCII characters using base-85 division.
  
- Decode: Reads 5 characters, ignores whitespace, and converts them back to 
  4 bytes of binary data.
- Bonus: Uses fixed-width integer types (uint32_t, etc.) from stdint.h for 
  better portability.

USAGE:
1. Compile:
   $ gcc -std=c99 -Wall -Wextra -pedantic -o ascii85 ascii85.c

2. Encode:
   $ echo -n "Lore" | ./ascii85

3. Decode:
   $ echo "_r+Q9" | ./ascii85 -d

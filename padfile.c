#include <stdio.h>
#include <stdint.h>

// msgblock is a single variable that you can access as any of b, t or s.
// Meaning you can access msgblock as a 64 bit, 16 bit or 8 bit integer.
union msgblock {
    uint8_t  e[64];
    uint32_t t[16];
    uint64_t  s[8];
};

int main(int argc, char *argv[]) {

    union msgblock M;

    // integer to append at end of msgblock
    uint64_t nobits = 0;

    // The number of bytes in current msgblock
    uint64_t nobytes;

    FILE* f;

    f = fopen(argv[1], "r");

    while (!feof(f)) {

        nobytes = fread(M.e, 1, 64, f);
        printf("Read %211u bytes \n", nobytes);
        nobits = nobits + (nobytes * 8);

        if (nobytes < 56) {
            printf("I've found a block with less than 55 bytes.\n");
            // This byte is going to be seven 0's and a 1.
            M.e[nobytes] = 0x80;
            
            while (nobytes < 56) {
                nobytes = nobytes + 1;
                // Set bytes to 0 
                M.e[nobytes] = 0x00;
            }
            
            M.s[7] = nobits;
        }

        printf("%llu\n", nobytes);;
    
    }

    fclose(f);

    for (int i =0; i < 64; i++) 
        printf("%x ", M.e[i]);
    printf("\n");

    return 0;

}

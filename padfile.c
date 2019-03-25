#include <stdio.h>
#include <stdint.h>

// msgblock is a single variable that you can access as any of b, t or s.
// Meaning you can access msgblock as a 64 bit, 16 bit or 8 bit integer.
union msgblock {
    uint8_t  b[64];
    uint32_t t[16];
    uint64_t  s[8];
};

int main(int argc, char *argv[]) {

    union msgblock M;

    uint64_t nobytes;

    FILE* f;

    f = fopen(argv[1], "r");

    while (!feof(f)) {

        nobytes = fread(M.b, 1, 64, f);
        printf("%llu\n", nobytes);;
    
    }

    fclose(f);

    return 0;

}

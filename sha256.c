// Hugh Brady - G00338260
// The Secure Hash Algorithm, 256 bit version
// https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

// For input/ output header file.
#include <stdio.h>
// For using fixed bit length integers.
#include <stdint.h>

// msgblock is a single variable that you can access as any of b, t or s.
// Meaning you can access msgblock as a 64 bit, 16 bit or 8 bit integer.
union msgblock {
    uint8_t  e[64];
    uint32_t t[16];
    uint64_t  s[8];
};

// For tracking where the program is when padding the message.
enum status {READ, PAD0, PAD1, FINISH};

// Calculates SHA256 hash of a file
void sha256(FILE *f);

// Retrieves the next message block.
int nextmsgblock(FILE *file, union msgblock *M, enum status *S, uint64_t *nobits);

// See Section 4.1.2.
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

// Section 3.2
uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

// Section 4.1.2
uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);

/*
// Macro Functions - Section 3.2

// See Section 4.1.2.
#define rotr(x, n)   (x >> n) | (x << (32 - n))
#define shr(x, n)    (x >> n)

// Section 3.2
#define sig0(x)      (rotr(7, x)  ^ rotr(18, x) ^ shr(3, x))
#define sig1(x)      (rotr(17, x) ^ rotr(19, x) ^ shr(10, x))

// Section 4.1.2
#define SIG0(x)      (rotr(2, x)  ^ rotr(13, x) ^ rotr(22, x))
#define SIG1(x)      (rotr(6, x)  ^ rotr(11, x) ^ rotr(25, x))
#define Ch(x, y, z)  (x & y) ^ ((!x) & z)
#define Maj(x, y, z) (x & y) ^ (x & z) ^ (y & z)

*/

int main(int argc, char *argv[]){
    
    // Open the file.
    FILE* file;
    file = fopen(argv[1], "r");

    // Run hash algorithm on the file.
    sha256(file);

    // Close the file.
    fclose(file);

    return 0;

}

void sha256(FILE *file) {
   
    // The current message block.
    union msgblock M;
    
    // The number of bits read from the file.
    // This integer is to be appended at the end of the message block.
    uint64_t nobits = 0;
    
    // The padding status of the message block
    enum status S = READ;

    // Message schedule (Section 6.2).
    uint32_t W[64];

    // Working variables (Section 6.).)
    uint32_t a, b, c, d, e, f, g, h;
    
    // Two temporary variables (Section 6.2).
    uint32_t T1, T2;
    
    // The Hash value (Section 6.2).
    // These values come from Section 5.3.3.
    uint32_t H[8] = {
          0x6a09e667
        , 0xbb67ae85
        , 0x3c6ef372
        , 0xa54ff53a
        , 0x510e527f
        , 0x9b05688c
        , 0x1f83d9ab
        , 0x5be0cd19
    };

    // The K constants (Section 4.2.2).
    uint32_t K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
   
    // Loop variables.
    int i, j;

    // Loop through message blocks as per step 2, page 22.
    while (nextmsgblock(file, &M, &S, &nobits)) {

        // W[j] = M[j] for 0 <= t <=15 (Page 22).
        for (j = 0; j < 16; j++) 
             W[j] = M.t[j];
    
        // W[j] = ... (Page 22).
        for (j = 16; j < 64; j++)
            W[j] = sig1(W[j-2]) + W[j-7] + sig0(W[j-15]) + W[j-16];
        
        // Initialise a, b, c ... variables as per step 2, page 22.
        a = H[0]; b = H[1]; c = H[2]; d = H[3];
        e = H[4]; f = H[5]; g = H[6]; h = H[7];

        // Step 3.
        for (j = 0; j < 64; j++) {
            T1 = h + SIG1(e) + Ch(e, f, g) + K[j] + W[j];
            T2 = SIG0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b; 
            b = a;
            a = T1 + T2;
        }

        // Step 4.
        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];
    
    }

    printf("%x %x %x %x %x %x %x %x %x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
   
}

int nextmsgblock(FILE *file, union msgblock *M, enum status *S, uint64_t *nobits) {  

    // The number of bytes in current message block.
    uint64_t nobytes;
    
    // Loop variables.
    int i;

    // If we have finished all the message blocks, then S should be FINISH.
    if (*S == FINISH)
        return 0;

    // Otherwise, check if we need another block full of padding.
    if (*S == PAD0 || *S == PAD1) { 
        
        // Set the first 56 bytes to all zero bits.
        for (i = 0; i < 56; i++) 
            M->e[i] = 0x00;
      
        // Set the last 64 bits to the number of bits in the file
        // Should be big-endian.
        M->s[7] = *nobits;
        *S = FINISH;

        // If S was PAD1, then set the first bit of M to one.
        if (*S == PAD1)
        M->e[0] = 0x80;
    
        // Keep the loop in SHA256 going for one more iteration.
        return 1;
    } 

    // If we get down this far we haven't finished reading the lines.
    nobytes = fread(M->e, 1, 64, file);
    
    // Keep track of the number of bytes we've read.
    *nobits = *nobits + (nobytes * 8);

    // If we read < 56 bytes, we can put all padding in this message block.
    if (nobytes < 56) {
        printf("I've found a block with less than 55 bytes.\n");
        // Append 1 to message.
        M->e[nobytes] = 0x80;
        
        // Add 0 bits up to last 64 bits.
        while (nobytes < 56) {
            nobytes = nobytes + 1;
            // Set bytes to 0 
            M->e[nobytes] = 0x00;
        }
        
        // Append the file size in bits.
        M->s[7] = *nobits;

        // Set status to FINISH.
        *S = FINISH;
     // Otherwise, check if we can pad this message block.
     } else if (nobytes < 64) { 
        // Set status
        *S = PAD0;
        // Append 1 to message.
        M->e[nobytes] = 0x80;

        while (nobytes < 64) {
            nobytes = nobytes + 1;
            // Fill the rest with 0's.
            M->e[nobytes] = 0x00;
        }
    // Otherwise, check if we're just at the end of the file.
    } else if (feof(file)) {
        // Set status to pad1, meaning we need another message block with all the padding.
        *S = PAD1;
    }

    // If we get this far, then return 1 so the function is called again
    return 1;

}

uint32_t rotr(uint32_t n, uint32_t x){
    return (x >> n) | (x << (32 - n));
}
    
uint32_t shr(uint32_t n, uint32_t x){
    return (x >> n);
}
        
uint32_t sig0(uint32_t x){
    // Sections 3.2 & 4.1.2 
    return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));
}
                
uint32_t sig1(uint32_t x){
    // Section 3.2 & 4.1.2
    return(rotr(17, x) ^ rotr(19, x) ^ shr(10, x));
}
                        
uint32_t SIG0(uint32_t x) {
    return (rotr(2, x) ^ rotr(13, x) ^ rotr(22, x));
}
                            
uint32_t SIG1(uint32_t x){ 
    return (rotr(6, x) ^ rotr(11, x) ^ rotr(25, x));
}
                                 
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) ^ ((!x) & z));
}
                                     
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) ^ (x & z) ^ (y & z));
 }



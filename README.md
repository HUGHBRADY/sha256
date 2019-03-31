# SHA256 Algorithm - Theory of Algorithms project
#### Hugh Brady - G00338260
An implementation of the Secure Hash Algorithm - 256 bit version in C. This project was developed using the vi text editor 
on a Linux virtual machine. 

The SHA256 algorithm is a set of cryptographic hash functions that takes input data and 
converts it into an output of fixed length, which represents the "fingerprint" of the data. 

The standard for the algorithm can be viewed [here]( https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
).

## How To Run
The C compiler is needed to compile this script. 

Compile the file using this command:
> gcc -o sha256.c sha256

Then run the executable using this command: 
> ./sha256 input.txt

input.txt can be replaced with any file of your choice. 

## Implementation
This project was completed alongside lecture videos created by Ian McLoughlin. The script consists of 
a number of functions, the largest of which being the sha256() and nextmsgblock() functions.
The sha256 method encrypts the inputted data. Then, nextmsgblock() deals with padding the message, meaning 
data is appended to the end of the message to ensure that the message is of the expected length.

Macro functions were included for converting bits in the message block to big endian in both 64 and 32 bit unsigned integers. 

Aside from these functions are smaller functions that are taken from the documentation that are called by the sha256() function.
I tried to change these functions into macros, but they inadvertantly affected the output so I left them as they were. 

This implementation is unfortunately incorrect and does not give the correct output when tested against online SHA256 hash generators. 
The reason for this, I think, is because I was converting to big endian incorrectly, be that in the implementation of the conversions
or in where the conversions were taking place.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/rsa.h"

#include "encoding.h"

int main(int argc, char **argv) {
    rsa_private_key privkey;
    FILE *fmessage;

    if (argc < 3) {
        fprintf(stdout, "Use: %s <d> <n>\n",argv[0]);
        fprintf(stdout, "d\t decryption exponent\n");
        fprintf(stdout, "n\t keypair modulus\n");
        exit(-1);
    }
    
    sscanf(argv[1],"%lu",&privkey.d);
    sscanf(argv[2],"%lu",&privkey.n);

    //printf("Decrypted content: ");


    if ((fmessage = fopen(FILE_NAME, "r"))) {

        // Calculate file dimensions
        /* fseek(fmessage, 0, SEEK_END);
        int filesize = ftell(fmessage);
        fseek(fmessage, 0, SEEK_SET); */

        // Read file content
        uint64_t M;
        unsigned i = 0;
        unsigned char message[MSG_LEN];
        memset(message,0,MSG_LEN);

        fseek(fmessage, 0, SEEK_SET);
        while (fread(&M, sizeof(uint64_t),1,fmessage)) {
            
            // Decryption
            M = rsa_decrypt(M, privkey);
            //printf("%08lx ", M);
            
            // Decoding
            unsigned j = 0;
            j = decode_kgram(M, message+i);
            i+=j;
            
        }
        printf("Message: %s\n",message);

        fclose(fmessage);
        exit(0);
    }
    else {
        printf("No messages found.\n");
        exit(0);
    }


}
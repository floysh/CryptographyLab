#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "modular_ops.h"
#include "ciphers/rsa.h"

#include "encoding.h"

int main(int argc, char **argv) {
    rsa_public_key pubkey;
    FILE *fmessage;

    if (argc < 3) {
        fprintf(stdout, "Use: %s <e> <n>\n",argv[0]);
        fprintf(stdout, "d\t encryption exponent\n");
        fprintf(stdout, "n\t keypair modulus\n");
        exit(-1);
    }

    sscanf(argv[1],"%lu",&pubkey.e);
    sscanf(argv[2],"%lu",&pubkey.n);

    // Ask for message
    unsigned char *msg = malloc(MSG_LEN*sizeof(char));
    memset(msg,0,MSG_LEN);
    printf("Type your message: ");
    fgets((char*)msg, MSG_LEN, stdin);

    // Properly terminate the input string
    /* unsigned padding_required = (strlen(msg) + 1) % K;
    unsigned block_count = (strlen(msg)+1)/K + (padding_required > 0); */
    for (int i=0; i < MSG_LEN; i++) {
        if (msg[i] == '\n') {msg[i] = '\0'; break;}
    }

    
    unsigned block_count = count_blocks(msg);
    
    int returncode = 0;
    
    // Write message to an encrypted binary file
    if ((fmessage = fopen(FILE_NAME, "w+"))) {  
        fseek(fmessage, 0, SEEK_SET);      
        for (unsigned i=0; i < block_count; i++) {
            uint64_t M = 0;

            // Encoding
            M = encode_kgram(msg+i*K);

            //printf("%08lx ",M);
            
            // Encryption
            M = rsa_encrypt(M, pubkey);
            fwrite(&M, sizeof(uint64_t),1,fmessage);
        }
        fclose(fmessage);
        printf("[INFO] Encrypted message saved to '%s'\n",FILE_NAME);

    }
    else {
        fprintf(stderr,"[ERR] Cannot create the encrypted file.\n");
    }

    free(msg);
    exit(returncode);

}
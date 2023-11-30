#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/rsa.h"

#define MSG_LEN 1024


int main(int argc, char **argv) {
    rsa_private_key privkey = (rsa_private_key) {.n = 1457, .d = 203};
    rsa_public_key pubkey = (rsa_public_key){.n = 1457, .e = 17};

    // RSA's encryption/decryption function
    // multiplicativity can be exploited to 
    // decrypt ciphertexts without having the private key!

    uint64_t M1,M2,M12;
    uint64_t C1,C2,C12;

    //M1 = 7;
    //C1 = encrypt(M1,pubkey);
    C1 = 1134;


    /* printf("Alice signed\n(M1,S1) = (%lu,%lu) %s\n",M1,S1, verify(M1,S1,pubkey) ? "✔" : "❌" );
    printf("We want to forge a valid signature for M2 = %lu\n",M2); */

    printf("We want to decrypt C1 = %lu but we don't have the private key\n",C1);


    // We use the public key to encrypt an arbitrary message
    M2 = 5;
    C2 = rsa_encrypt(M2,pubkey);

    // Since powermod is a multiplicative function,
    // C1*C2 = M1^2 * M2^e = (M1*M2)^e (mod n)
    // so if we trick Alice into decrypting the evil ciphertext C1C2
    // we get
    // (C1*C2)^d = (M1*M2)^(e*d) = M1*M2 (mod n)
    C12 = modmul(C1,C2, pubkey.n);
    printf("C12 = %lu\n",C12);

    // Now we ask Alice to decrypt the evil ciphertext
    // she provides us M1*M2
    M12 = rsa_decrypt(C12,privkey);
    printf("M12 = %lu\n",M12);

    // We got M1*M2!
    // Now we can extract M1 with the aid of the 
    // modular multiplicative inverse of the message we chose:
    // M1 = M1*M2*M2^(-1) (mod n)

    //M12 * mod_inverse(M2,pubkey.n);
    M1 = modmul(M12, mod_inverse(M2,pubkey.n), pubkey.n);
    printf("M2inv = %lu\n",mod_inverse(M2,pubkey.n));


    printf("The decrypted message is: %lu\n",M1);
    printf("Proof: %lu^%lu = %lu (mod %lu), which the same ciphertext we wanted to decrypt.\n",M1,pubkey.e, rsa_encrypt(M1,pubkey), pubkey.n);
    

    printf("\n-------------------------------------\n");
    printf("Let's try something less trivial: decrypting a real text message\n");

    unsigned char message[MSG_LEN];
    uint64_t encrypted_buffer[MSG_LEN];

    memset(message, 0, MSG_LEN);
    sprintf((char*)message,"Le soluzioni dei compiti sono nel secondo cassetto. P.C. 👴");

    printf("CIPHERTEXT: ");
    for (int i=0; i<MSG_LEN; i++) {
        encrypted_buffer[i] = rsa_encrypt(message[i],pubkey);
        printf("%c", (unsigned char)encrypted_buffer[i]);
    }
    printf("\n");

    M2 = 5;
    C2 = rsa_encrypt(M2, pubkey);

    for (int i=0; i<MSG_LEN; i++) {
        encrypted_buffer[i] = modmul(encrypted_buffer[i], C2, pubkey.n);
    }

    // Ask alice to decrypt M1*M2
    for (int i=0; i<MSG_LEN; i++) {
        encrypted_buffer[i] = rsa_decrypt(encrypted_buffer[i],privkey);
    }

    // Now we extract the original message
    printf("DECRYPTED MESSAGE: ");
    uint64_t M2inv = mod_inverse(M2,pubkey.n);
    for (int i=0; i<MSG_LEN; i++) {
        encrypted_buffer[i] = modmul(encrypted_buffer[i], M2inv, pubkey.n);
        printf("%c", (unsigned char)encrypted_buffer[i]);
    }

    printf("\n");


    printf("\n-------------------------------------\n");
    printf("A similair attack can be performed on the RSA signature scheme as well, tricking Alice into signing arbitrary data!\n");

    printf("Say we have a message signed by Alice:\n");

    rsa_signature S1,S2,S12;
    S1 = rsa_sign(M1,privkey);

    printf("\t(M1,S1) = (%lu,%lu) %s\n",M1,S1,rsa_verify(M1,S1,pubkey) ? "✔" : "❌");

    printf("Let's try to get a valid signature for M2=%lu.\n",M2);
    printf("If Alice signs M12 = M1 * M2 = %lu*%lu, we can obtain a valid signature for another message M2. Even if Alice never intended to signed it\n",M1,M2);

    M12 = M1*M2;

    S12 = rsa_sign(M12,privkey);

    S2 = modmul(S12, mod_inverse(S1,pubkey.n), pubkey.n);

    printf("\t(M2,S2) = (%lu,%lu) %s\n",M2,S2,rsa_verify(M2,S2,pubkey) ? "✔" : "❌");


}
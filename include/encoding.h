// Choose N and K carefully
// if MAX_MESSAGE = (N^K-1) > 64/2 = 32
// modexp() will fail to encrypt/decrypt
// because of integer overflow errors
// as the mul step (partial*partial) may require double the size of the operands
// to safely store the result
// but we only have up to 64bit integers :/

// To safely encode the whole standard C unsigned charset
// N = 0x100 //256
// K = 4
// as 256^2 = 2^8^4 = 2^32 which is 2^64

#ifndef LIBENCODING
#define LIBENCODING

#define N 0x100 // alphabet set cardinality
#define K 4    // block size
#define MSG_LEN K*1024

#define FILE_NAME "message.enc"

// Returns the number of blocks required to encode
// a string
unsigned count_blocks(unsigned char *msg); 

// Encoding for a single k-gram
uint64_t encode_kgram(unsigned char *buffer);
unsigned decode_kgram(uint64_t M, unsigned char* buffer);

// Encoding for an entire string
// return the number of k-grams written
unsigned encode(unsigned char *msg, uint64_t *encoded_buffer);
unsigned decode(uint64_t *encoded_buffer, unsigned char *msg, unsigned buff_size);

#endif
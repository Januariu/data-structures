#include "blockchain.h"

#define BLOCK_SIZE 31

typedef struct Block {
    struct Block *next;      
    unsigned char size;      
    char data[BLOCK_SIZE];           
} Block;

typedef struct BlockChain {
    Block *head;
    Block *tail;
    size_t length;
} BlockChain;


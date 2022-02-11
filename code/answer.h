#ifndef ANSWER_H
#define ANSWER_H

#define BYTE 1

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>  

struct answer{
    uint16_t name;
    uint16_t type;
    uint16_t class;
    uint16_t *ttl;
    uint16_t rdlength;
    char *rdata;
};

int parse_answer(struct dns_message *message, int index);
void free_answer(struct dns_message *message);

#endif
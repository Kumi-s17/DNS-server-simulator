#ifndef QUESTION_H
#define QUESTION_H

#define QUESTION_START 14
#define BYTE 1
#include <stdio.h>
#include <stdlib.h>

struct question{
    char *qname;
    uint16_t qtype;
    uint16_t qclass;
};

int parse_question(struct dns_message *message);
void free_question(struct dns_message *message);


#endif
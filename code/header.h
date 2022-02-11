#ifndef HEADER_H
#define HEADER_H

#define ID 2
#define FLAG 4
#define QDCOUNT 6
#define ANCOUNT 8
#define NSCOUNT 10
#define ARCOUNT 12

#include <stdio.h>
#include <stdlib.h>

struct header{
    uint16_t id;
    struct flag *flag;
    uint16_t qdcount;
	uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

struct flag{
    uint8_t qr;
    uint8_t opcode;
    uint8_t aa;
    uint8_t tc;
    uint8_t rd;
    uint8_t ra;
    uint8_t z;
    uint8_t rcode;
};

struct dns_message * parse_header(struct dns_message *message);
struct flag *parse_flag(uint16_t buffptr);
void free_header(struct dns_message *message);

#endif

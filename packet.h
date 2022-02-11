#ifndef PACKET_H
#define PACKET_H

#define BUFFER_LEN_BYTES 2
#define HEADER_LEN_BYTE1 0
#define HEADER_LEN_BYTE2 12
#define HEADER_LEN 14

#define ZERO 0
#define FLAG_BYTE1 128
#define FLAG_BYTE2 4

#define QDCOUNT_INDEX_1 6
#define QDCOUNT_INDEX_2 7

#define ANCOUNT_INDEX_1 8
#define ANCOUNT_INDEX_2 9

#define NSCOUNT_INDEX_1 10
#define NSCOUNT_INDEX_2 11

#define ARCOUNT_INDEX_1 12
#define ARCOUNT_INDEX_2 13

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct dns_message{
    struct header *header;
    struct question *question;
    struct answer *answer;
    int buffer_length;
    uint8_t *buffer;
};

struct error_packet{
    uint8_t *header;
    int buffer_length;
};

struct dns_message *parse_message(struct dns_message *message);
struct error_packet *create_response(struct dns_message * message);
void free_packet(struct dns_message *message);
void free_error_packet(struct error_packet *error_response);

#endif
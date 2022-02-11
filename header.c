#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "packet.h"
#include "header.h"
#include "utils.h"

//parses the header section of the dns message
struct dns_message * parse_header(struct dns_message *message){
    message -> header = malloc(sizeof(*(message -> header)));
    assert(message -> header);

    message -> header -> id = toTwoByte(message -> buffer[ID], message -> buffer[ID + 1]);
    message -> header -> flag = parse_flag(toTwoByte(message -> buffer[FLAG], message -> buffer[FLAG + 1]));
    
    message -> header -> qdcount = toTwoByte(message -> buffer[QDCOUNT], message -> buffer[QDCOUNT + 1]);
    message -> header -> ancount = toTwoByte(message -> buffer[ANCOUNT], message -> buffer[ANCOUNT + 1]);
    message -> header -> nscount = toTwoByte(message -> buffer[NSCOUNT], message -> buffer[NSCOUNT + 1]);
    message -> header -> arcount = toTwoByte(message -> buffer[ARCOUNT], message -> buffer[ARCOUNT + 1]);

    return message;
}

//parsing individual components of the header flag
struct flag *parse_flag(uint16_t buffptr){
    struct flag *flag = malloc(sizeof(*flag));
    assert(flag);

    flag -> qr = buffptr >> 15 & 1;
    flag -> opcode = buffptr >> 11 & 15;
    flag -> aa = buffptr >> 10 & 1;
    flag -> tc = buffptr >> 9 & 1;
    flag -> rd = buffptr >> 8 & 1;
    flag -> ra = buffptr >> 7 & 1;
    flag -> z = buffptr >> 4 & 7;
    flag -> rcode = buffptr & 15;

    return flag;
}

//frees the header section of the DNS message
void free_header(struct dns_message *message){
    free(message -> header -> flag);
    free(message -> header);
}
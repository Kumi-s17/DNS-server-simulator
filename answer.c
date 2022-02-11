#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "packet.h"
#include "answer.h"
#include "utils.h"

//parsing the answer section of the DNS message
int parse_answer(struct dns_message *message, int index){
    int i;
    //allocate space for the answer
    message -> answer = malloc(sizeof(*(message -> answer)));
    assert(message -> answer);

    message -> answer -> name = toTwoByte(message -> buffer[index], message -> buffer[index + 1]);
    index += (2 * BYTE);

    message -> answer -> type = toTwoByte(message -> buffer[index], message -> buffer[index + 1]);
    index += (2 * BYTE);
    message -> answer -> class = toTwoByte(message -> buffer[index], message -> buffer[index + 1]);
    index += (2 * BYTE);
    
    //ttl 4bytes
    index += (4 * BYTE);
    message -> answer -> rdlength = toTwoByte(message -> buffer[index], message -> buffer[index + 1]);
    index += (2 * BYTE);
    message -> answer -> rdata = malloc(sizeof(char) * INET6_ADDRSTRLEN);
    uint8_t rdata[message -> answer -> rdlength];
    for(i = 0; i < message -> answer -> rdlength; i++){
        rdata[i] = message -> buffer[index];
        index ++;
    }
    inet_ntop(AF_INET6, rdata, message -> answer -> rdata, INET6_ADDRSTRLEN);
    
    return index;
}

//frees the answer section of the DNS message
void free_answer(struct dns_message *message){
    free(message -> answer -> rdata);
    free(message -> answer);
}
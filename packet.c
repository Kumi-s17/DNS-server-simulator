#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "packet.h"
#include "header.h"
#include "question.h"
#include "answer.h"

//Parses the DNS message 
struct dns_message *parse_message(struct dns_message *message){
    int index = 0;

    //parse in headers
    message = parse_header(message);
     
    //parse the question field for both the request and response
    index = parse_question(message);
    

    //if QR is 1, the message is a response so parse in the answer field
    if(message -> header -> flag -> qr == 1){
        index = parse_answer(message, index);
    }

    return message;
}


//creates an response with Rcode 4 for requests other than AAAA
struct error_packet *create_response(struct dns_message * message){
    struct error_packet *errorPacket = malloc(sizeof(*errorPacket));
    errorPacket -> header = malloc(sizeof(*errorPacket -> header) * HEADER_LEN);

    errorPacket -> header[0] = HEADER_LEN_BYTE1;
    errorPacket -> header[1] = HEADER_LEN_BYTE2;

    //ID
    errorPacket -> header[2] = message -> buffer[2];
    errorPacket -> header[3] = message -> buffer[3];

    //setting the flags
    //sets QR to 1
    errorPacket -> header[4] = FLAG_BYTE1;
    //sets RCODE to 4
    errorPacket -> header[5] = FLAG_BYTE2;
    //QDCOUNT (number of entries in the question section)
    errorPacket -> header[QDCOUNT_INDEX_1] = ZERO;
    errorPacket -> header[QDCOUNT_INDEX_2] = ZERO;
    //ANCOUNT
    errorPacket -> header[ANCOUNT_INDEX_1] = ZERO;
    errorPacket -> header[ANCOUNT_INDEX_2] = ZERO;
    //NSCOUNT
    errorPacket -> header[NSCOUNT_INDEX_1] = ZERO;
    errorPacket -> header[NSCOUNT_INDEX_2] = ZERO;
    //ARCOUNT
    errorPacket -> header[ARCOUNT_INDEX_1] = ZERO;
    errorPacket -> header[ARCOUNT_INDEX_2] = ZERO;

    errorPacket -> buffer_length = HEADER_LEN;

    return errorPacket;

}

void free_packet(struct dns_message *message){
    free(message -> buffer);
    free_header(message);
    free_question(message);
    free_answer(message);
}


void free_error_packet(struct error_packet *error_response){
    free(error_response -> header);
    free(error_response);
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "packet.h"
#include "question.h"
#include "utils.h"

//Parse the question section of the dns message
int parse_question(struct dns_message *message){
    //allocate space for question 
    message -> question = malloc(sizeof(*(message -> question)));
    assert(message -> question);

    int index = QUESTION_START;
    int urlLength = 0;

   //getting the length of the url and the null byte at the end
    while(message -> buffer[index] != 0){
        urlLength += message -> buffer[index] + 1;
        index += message -> buffer[index] + 1;
    }

    message -> question -> qname = malloc(sizeof(*message -> question -> qname) * urlLength);
    assert(message -> question -> qname);

    index = QUESTION_START;
    int qname_index = 0;
    int length = 0;
    while(message -> buffer[index] != 0){
        length = message -> buffer[index];
        index ++;
        for(int i = 0; i < length; i++){
            message -> question -> qname[qname_index] = message -> buffer[index];
            qname_index ++;
            index ++;
        }

        //add a period if it is not the end of the url
        if(qname_index < urlLength - 1){
            message -> question -> qname[qname_index] = '.';
            qname_index ++;
        }
        message -> question -> qname[qname_index] = '\0';

    }

    index ++;
    message -> question -> qtype = toTwoByte(message -> buffer[index], message -> buffer[index + 1]);
    index += (2 * BYTE);
    message -> question -> qclass = toTwoByte(message -> buffer[index], message -> buffer[index + 1]);
    index += (2 * BYTE);
   
    return index;
}

//frees the question section of the DNS message
void free_question(struct dns_message *message){

    free(message->question->qname);
    free(message -> question);
}
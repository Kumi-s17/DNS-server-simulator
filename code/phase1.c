#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <unistd.h> 
#include <time.h>
#include <stdint.h>   

#include "phase1.h"
#include "packet.h"
#include "header.h"
#include "question.h"
#include "answer.h"
#include "utils.h"


//Parsing the dns message 
struct dns_message *parse(int newsockfd) {

    //reading in the first 2 bytes
    uint8_t *len = malloc(sizeof(*len) * BUFFER_LEN_BYTES);

    int i = 0, num_read = 0;
   
   //continue reading until the length buffer arrives
    while(i < BUFFER_LEN_BYTES){
       num_read = read(newsockfd, &len[i], 1);
       while(num_read != 1){
           num_read = read(newsockfd, &len[i], 1);
       } 
       i++;      
   }

    //get the length of the buffer by combining the 2 bytes
    int length = (len[0] << 8) | len[1];

    i = 0, num_read = 0;

    //reading in the buffer 
    uint8_t *contents = malloc(sizeof(*contents) * length);

    //continue reading until all the bytes have arrived 
    while(i < length){
        num_read = read(newsockfd, &contents[i], 1);
        while(num_read != 1){
            num_read = read(newsockfd, &contents[i], 1);
        } 
        i++;
    }

    
    struct dns_message *message = malloc(sizeof(*message));
    assert(message);

    message -> buffer = malloc(sizeof(*(message -> buffer)) * (length + BUFFER_LEN_BYTES));
    
    //combine the buffers 
    memcpy(message -> buffer, len, BUFFER_LEN_BYTES);
    memcpy(message -> buffer + BUFFER_LEN_BYTES, contents, length);
    //store the length of the buffer
    message -> buffer_length = length + BUFFER_LEN_BYTES;


    //parsing the dns message (packet)
    message = parse_message(message);

    
    //Logging
    FILE *fp;
    fp = fopen("dns_svr.log", "a");
    assert(fp);

    //adapted from stack overflow https://stackoverflow.com/questions/25030055/add-date-and-time-to-a-file-name-in-c
    char buffer[BUFFER_LEN];
    time_t rawTime;
    struct tm *timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(buffer, BUFFER_LEN, "%FT%T%z", timeInfo);

    //if the dns message is a query
    if(message -> header -> flag -> qr == 0){
        fprintf(fp, "%s requested %s\n", buffer, message -> question -> qname);
        if(message -> question -> qtype != AAAA){
            fprintf(fp, "%s unimplemented request\n", buffer);
        }
    }

    //if the dns message is a response and there is at least one answer
    else if(message -> header -> flag -> qr == 1 && message -> header -> ancount > 0){
        //check if the response is of type AAAA
        if(message -> answer -> type == AAAA){
             fprintf(fp, "%s %s is at %s\n", buffer, message -> question -> qname, message -> answer -> rdata);
        }
    }


    fclose(fp);

    //free memory
    free(len);
    free(contents);
    
    return message;
}



#ifndef PHASE1_H
#define PHASE1_H

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LEN_BYTES 2
#define BUFFER_LEN 100
#define AAAA 28

struct dns_message *parse(int newsockfd);


#endif
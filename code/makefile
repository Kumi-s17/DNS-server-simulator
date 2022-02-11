CC = gcc
CFLAGS = -Wall -o -g
LIB = -lm
//HDR = phase1.h packet.h header.h question.h answer.h utils.h

SRC1 = dns_svr.c phase1.c packet.c header.c question.c answer.c utils.c
OBJ1 = $(SRC1:.c=.o)
EXE1 = dns_svr

all: $(EXE1) 
$(EXE1): $(HDR) $(OBJ1)
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ1) $(LIB)


clean:
	rm -f $(EXE1) *.o dns_svr.log

$(OBJ1): $(HDR)
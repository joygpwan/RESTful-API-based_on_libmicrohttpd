#s3:contain.o request_analysis.o object_response_header.o bucket_response_header.o delete_bucket.o xml.o
#source file
SOURCE=contain.c contain.h request_analysis.c request_analysis.h object_response_header.c object_response_header.h bucket_response_header.c bucket_response_header.h delete_bucket.c delete_bucket.h xml.c xml.h 

#which compiler
CC=gcc

CFLAGS=-w -g
INCLUDEX=/usr/local/include/libxml2
INCLUDEH=/usr/local/include
LIB=-lxml2 -lmicrohttpd
OBJECTS=container.o object_response_header.o request_analysis.o  bucket_response_header.o delete_bucket.o xml.o

#s3:contain.o request_analysis.o object_response_header.o bucket_response_header.o delete_bucket.o xml.o
all:s3
s3:$(OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDEX) -I$(INCLUDEH)  $(OBJECTS) -o s3 $(LIB)
#	$(CC) $(CFLAGS) -I$(INCLUDEX) -I$(INCLUDEH) $(LIB) -o s3 $^
%.o:%.c
#	%.o
	$(CC) $(CFLAGS) -I$(INCLUDEX) -I$(INCLUDEH) $(LIB) -c $<

.PHONY:clean
clean:
	-rm $(OBJECTS) s3






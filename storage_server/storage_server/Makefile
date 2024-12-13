

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = SS

SRCS = additionalinfo.c commonfunctionsstorageserver.c copy.c paste.c create.c delete.c read.c strogeservers.c write.c 
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

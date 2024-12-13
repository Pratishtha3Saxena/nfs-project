

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = NMS

SRCS = main.c client_data.c client_tcp.c LRU.c Path_DS.c S_centre.c Storage_data.c storage_tcp.c redundancy.c duplicacy.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

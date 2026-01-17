CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude
SRCS = src/main.c src/graph.c src/sort.c src/dijkstra.c
OBJS = $(SRCS:.c=.o)
TARGET = metro

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) data/metro.txt

clean:
	rm -f $(OBJS) $(TARGET)

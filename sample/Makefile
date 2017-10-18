TARGET	= local-yasuna
CC	= cc
RM	= rm
CFLAGS	= -O2
LIBS	= -lbenly_proc -lcheapbot -lncipher
SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LIBS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) -f $(OBJS) $(TARGET)

.PHONY: all clean

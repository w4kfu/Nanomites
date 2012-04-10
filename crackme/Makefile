include config.mk

TARGET  = 	crackme

OBJS 	=	test.o \
		son_work.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(TARGET)
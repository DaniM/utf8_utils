
.PHONY: clean

default: utf8_utils.o

dev: CFLAGS+=-g -D_DEBUG 
dev: utf8_utils.o

test_utf8: CFLAGS+=-g -D_DEBUG 
test_utf8: utf8_utils.o

clean:
	rm -f *.o

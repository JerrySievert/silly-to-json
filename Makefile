SRCS := export.c mobile.c object.c room.c
CFLAGS := -I/opt/homebrew/include
LDFLAGS := -L/opt/homebrew/lib -ljansson

OBJS = $(subst .c,.o, ${SRCS})

all: $(OBJS)
	clang $(OBJS) $(LDFLAGS) -o export

clean:
	rm *.o

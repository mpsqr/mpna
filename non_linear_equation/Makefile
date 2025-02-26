CC = mpicc
CFLAGS = -I../../td/hypre-master/src/hypre/include/ -O2
LDFLAGS = -L../../td/hypre-master/src/hypre/lib/ -lHYPRE -lm

SRC = src/non_linear_diffusion.c
OBJ = $(SRC:.c=.o)
BIN = bin/non_linear_diffusion

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)


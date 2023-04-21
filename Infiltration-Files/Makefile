CC = gcc
CFLAGS = -ansi -pedantic -Wall
LDFLAGS = -lMLV -lm

SRC=./src/
BIN=./bin/
INC=./include/

EXEC: $(BIN)stealth
	./bin/stealth

$(BIN)stealth: $(BIN)terrain.o $(BIN)graphic.o $(BIN)player.o $(BIN)guardians.o $(BIN)collisions.o $(BIN)score.o $(BIN)main.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN)main.o: $(SRC)main.c $(INC)terrain.h $(INC)graphic.h $(INC)player.h $(INC)guardians.h
	$(CC) -c -o $@ $(CFLAGS) $(SRC)main.c

$(BIN)terrain.o: $(SRC)terrain.c $(INC)terrain.h
	$(CC) -c -o $@ $(CFLAGS) $(SRC)terrain.c

$(BIN)player.o: $(SRC)player.c $(INC)player.h $(INC)movement.h $(INC)collisions.h
	$(CC) -c -o $@ $(CFLAGS) $(SRC)player.c

$(BIN)guardians.o: $(SRC)guardians.c $(INC)guardians.h $(INC)movement.h $(INC)collisions.h
	$(CC) -c -o $@ $(CFLAGS) $(SRC)guardians.c

$(BIN)collisions.o: $(SRC)collisions.c $(INC)collisions.h $(INC)terrain.h
	$(CC) -c -o $@ $(CFLAGS) $(SRC)collisions.c

$(BIN)graphic.o: $(SRC)graphic.c $(INC)graphic.h $(INC)guardians.h $(INC)player.h $(INC)terrain.h
	$(CC) -c -o $@ $(CFLAGS) $(SRC)graphic.c

$(BIN)score.o: $(SRC)score.c $(INC)score.h
	$(CC) -c -o $@ $(CFLAGS) $(SRC)score.c

clean:
	rm -f $(BIN)*.o
	rm $(BIN)stealth
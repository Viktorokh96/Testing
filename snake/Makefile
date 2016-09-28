CC = g++
CFLAG = -c -Wall -g
CLFLAG = -Wall -lncurses

ALLOBJ = snake.o

EXE = snake

all: $(EXE)

$(EXE) : $(ALLOBJ)
	$(CC) $^ $(CLFLAG) -o $@

$(ALLOBJ) : %.o : %.cpp
	$(CC) $(CFLAG) $< -o $@

.PHONY: clean 
clean:
	rm -rf *.o

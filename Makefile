CC=g++
CXXFLAGS = -std=c++20 -g
DEPS = Model.h DataPoint.h Agent.h Cell.h AbnormalCell.h
OBJ = Model.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cancerABM: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY : clean
clean :
	-rm *.o $(objects) cancerABM
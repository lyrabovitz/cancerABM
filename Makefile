CC=g++
DEPS = Model.h DataPoint.h Agent.h Cell.h AbnormalCell.h
OBJ = Model.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cancerABM: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
CC = g++
CFLAGS = -g -Wall -O2
TARGET = Flow

run: Flow
	./$(TARGET)

Make: Flow

Flow: Driver.o FlowGraphics.o FlowGrid.o FlowTile.o FlowTypes.o
	$(CC) $(CFLAGS) -o $(TARGET) Driver.o FlowGraphics.o FlowGrid.o FlowTile.o FlowTypes.o

Driver.o: Driver.cpp FlowGrid.h FlowTypes.h FlowGraphics.h
	$(CC) $(CFLAGS) -c Driver.cpp

FlowGraphics.o: FlowGraphics.cpp FlowGraphics.h FlowGrid.o
	$(CC) $(CFLAGS) -c FlowGraphics.cpp

FlowGrid.o: FlowGrid.cpp FlowGrid.h FlowTile.h FlowTypes.h
	$(CC) $(CFLAGS) -c FlowGrid.cpp 

FlowTile.o: FlowTile.cpp FlowTile.h FlowTypes.h
	$(CC) $(CFLAGS) -c FlowTile.cpp 

FlowTypes.o: FlowTypes.cpp FlowTypes.h
	$(CC) $(CFLAGS) -c FlowTypes.cpp

clean:
	rm *.o
	rm *.tmp
	rm $(TARGET)
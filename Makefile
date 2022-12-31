CC = g++
CFLAGS = -g -Wall

Make: Flow

Flow: Driver.o FlowGrid.o FlowTile.o FlowTypes.o
	$(CC) $(CFLAGS) -o Flow Driver.o FlowGrid.o FlowTile.o FlowTypes.o
#$(CC) $(CFLAGS) -o Flow Driver.cpp FlowGrid.cpp FlowTile.cpp FlowTypes.cpp

Driver.o: Driver.cpp FlowGrid.h FlowTypes.h
	$(CC) $(CFLAGS) -c Driver.cpp

#FlowGraphics.o: FlowGraphics.cpp FlowGraphics.h FlowGrid.o FlowTile.o FlowTypes.o
#	$(CC) $(CFLAGS) -c FlowGraphics.cpp FlowGrid.h FlowTile.h FlowTypes.h

FlowGrid.o: FlowGrid.cpp FlowGrid.h FlowTile.h FlowTypes.h
	$(CC) $(CFLAGS) -c FlowGrid.cpp 

FlowTile.o: FlowTile.cpp FlowTile.h FlowTypes.h
	$(CC) $(CFLAGS) -c FlowTile.cpp 

FlowTypes.o: FlowTypes.cpp FlowTypes.h
	$(CC) $(CFLAGS) -c FlowTypes.cpp

clean:
	rm *.o
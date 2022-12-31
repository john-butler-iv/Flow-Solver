#include <memory>

#include "FlowTypes.h"

#ifndef FLOW_TILE
#define FLOW_TILE

class FlowTile {
private:
    static int counter;

    int uniqId;
    bool isSource;
    FlowColor color;
    std::shared_ptr<FlowTile> connections[NUM_DIRECTIONS];
    std::shared_ptr<FlowTile> sharedThis;
    std::shared_ptr<FlowTile> tail;


    /**
     * Computes the total number of connections this FlowTile is connected to.
     */
    int totalConnections();

    /**
     * Merges the color of this FlowTile and otherTile.
     * If both paths have two different colors assigned, an error is thrown
     */
    void mergeColors(std::shared_ptr<FlowTile> otherTile);

    /**
     * Assigns a color to this sequence of FlowTiles. The current tile must be uncolored
     * @param color - Color to assign this FlowTile and all of the connected FlowTiles
     */
    void assignColor(FlowColor color);

    /**
     * This source will be assigned to otherTile and vice versa if possible.
     * There must be an available source on each tile.
     */
    void physicallyConnect(std::shared_ptr<FlowTile> otherTile, Direction toOtherTile);

public:
    /**
     * Creates an empty tile. If you want this tile to be a source, make sure to call setAsSource
     * before performing any other action
     */
    FlowTile();

    /**
     * As part of initialization, this tile will be marked as a source.
     * If any action has already occured, this will throw an error.
     */
    void setAsSource(FlowColor color);

    /**
     * Connects this to other tile in the toOtherTile direction and vice verse
     * This also merges the colors of the two tiles and their whole chains
     * 
     * Many safety asserts occur in this function
     * 
     * @param otherTile FlowTile to connect to.
     * @param toOtherTile Direction that otherTile lies from the perspecive of this tile.
     */
    void connect(std::shared_ptr<FlowTile> otherTile, Direction toOtherTile);
    /**
     * Determines if this FlowTile is in a solved state based on the number of conncetions and whether it has an assigned color
     */
    bool isInSolvedState();

    /**
     * If you want to use a pointer to this FlowTile, this will return the shared pointer created.
     */
    std::shared_ptr<FlowTile> getSharedPointer();

    /**
     * Getter method for the connections this Tile has. Indexed by Directions enum
     */
    const std::shared_ptr<FlowTile> getConnection(Direction dir);

    /**
     * Simple getter for the color of this FlowTile.
     */
    const FlowColor getColor();
    
    bool getIsSource();

    /**
     * Calculates the number of connections remaining until this tile is full. This will return
     * a negative number if there are more connections than are allowed
     */
    int remainingConnections();
};

#endif
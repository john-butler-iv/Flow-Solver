#include <memory>

#include "FlowTypes.h"

#ifndef FLOW_TILE
#define FLOW_TILE

class FlowTile {
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
};

#endif
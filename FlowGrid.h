#include <memory>

#include "FlowTypes.h"
#include "FlowTile.h"

#ifndef FLOW_GRID
#define FLOW_GRID

class FlowGrid {
private:
    int height;
    int width;
    std::unique_ptr<FlowTile[]> grid;

    int index(int r, int c);
    int indexOffset(int r, int c, Direction dir);
public:
    /**
     * Creates a blank flow grid for you to populate
     */
    FlowGrid(int height, int width);

    /**
     * Marks a particular tile as a source of the given color
     */
    void addSource(int x, int y, FlowColor color);


    void solve();

    /**
     * Creates a string representation of the grid and all of its tiles.
     */
    const std::string toString();
};

#endif
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
    int offsetRawIndex(int i, Direction dir);
    int indexOffset(int r, int c, Direction dir);

    bool wouldCreateLoop(std::shared_ptr<FlowTile> currentTile, std::shared_ptr<FlowTile> neighborTile);
    bool validDirection(std::shared_ptr<FlowTile> currentTile, std::shared_ptr<FlowTile> neighborTile, Direction dir);

    bool doForcedMove(int r, int c);
    bool joinAdjacentColors(int r, int c);
public:
    /**
     * Creates a blank flow grid for you to populate
     */
    FlowGrid(int height, int width);

    /**
     * Marks a particular tile as a source of the given color
     */
    void addSource(int r, int c, FlowColor color);

    std::shared_ptr<FlowTile> getPointer(int r, int c);

    std::shared_ptr<FlowTile> getPointerRelative(int r, int c, Direction dir);
    

    void solve();

    /**
     * Creates a string representation of the grid and all of its tiles.
     */
    const std::string toString();
};

#endif
#include <string>

#ifndef FLOW_TYPES
#define FLOW_TYPES

/**
 * Total number of elements in the Direction enum. 
 */
static const int NUM_DIRECTIONS = 4;
/**
 * Named constants for relative positions of Tiles. Intented to be used as indexes into the FlowTile connections array
 * If you alter this (such as if I indend to extend this to higher dimensions), remember to update NUM_DIRECTIONS
 */
enum Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};
/**
 * Given a single Direction, returns the oposite direction.
 */
Direction OppositeDirection(Direction dir);

/**
 * Valid colors for a flow tile.
 */
enum FlowColor {
    Uncolored,
    Red,
    Green,
    Blue,
    Yellow,
    Orange,
    Cyan,
    Pink,
    Maroon,
    Purple,
    White,
    Grey,
    Lime,
    Tan,
    DarkBlue
};
/**
 * Given a FlowColor, a human-friendly name is returned.
 */
std::string FlowColorName(FlowColor color);

#endif
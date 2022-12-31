#include <iostream>
#include <memory>
#include <string>

#include "FlowGrid.h"
#include "FlowTile.h"
#include "FlowTypes.h"

/// @brief given a FlowColor, returns a one-letter string containing a unique initial
/// @param color 
/// @return 
std::string colorInitial(FlowColor color) {
    switch (color) {
    case Red:
        return "R";
    case Green:
        return "G";
    case Blue:
        return "B";
    case Yellow:
        return "Y";
    case Orange:
        return "O";
    case Cyan:
        return "C";
    case Pink:
        return "P";
    case Maroon:
        return "M";
    case Purple:
        return "P";
    case White:
        return "W";
    case Grey:
        return "G";
    case Lime:
        return "L";
    case Tan:
        return "T";
    case DarkBlue:
        return "D";
    case Uncolored:
    default:
        return " ";
    }
}

FlowGrid::FlowGrid(int height, int width) {
    this->height = height;
    this->width = width;
    grid = std::unique_ptr<FlowTile[]>(new FlowTile[height * width]);
}

void FlowGrid::addSource(int x, int y, FlowColor color){
    grid[y * height + x].setAsSource(color);
}


int FlowGrid::index(int r, int c){
    return r * height + c;
}

int FlowGrid::indexOffset(int r, int c, Direction dir){
    switch(dir){
        case Left:
            return index(r, c-1);
        case Right:
            return index(r, c+1);
        case Up:
            return index(r-1, c);
        case Down:
            return index(r+1, c);
    }
}

bool validDirection(FlowTile currentTile, FlowTile neighborTile, Direction dir){
    return currentTile.getConnection(dir) == nullptr
            && neighborTile.getConnection(OppositeDirection(dir)) == nullptr
            && (currentTile.getColor() == neighborTile.getColor()
            || currentTile.getColor() == Uncolored
            || neighborTile.getColor() == Uncolored);
}

void FlowGrid::solve(){
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            FlowTile currentTile = grid[index(r,c)];
            assert(currentTile.remainingConnections() >= 0);
            if(currentTile.remainingConnections() == 0){
                continue;
            }
            int totalValidMoves = 0;
            Direction validMoves[4];

            // Left
            if(r != 0 && validDirection(currentTile, grid[indexOffset(r,c,Left)], Left)){
                validMoves[totalValidMoves++] = Left;
            }
        }
    }
}


const std::string FlowGrid::toString() {
    std::string outStr = "";
    for (int r = 0; r < height; r++) {
        std::string lineStr1 = "";
        std::string lineStr2 = "";
        std::string lineStr3 = "";
        for (int c = 0; c < width; c++) {
            FlowTile tile = grid[r * height + c];
            lineStr1 += (tile.getConnection(Up) == nullptr ? "   " : " | ");
            lineStr2 += (tile.getConnection(Left) == nullptr ? " " : "-") +
                        (colorInitial(tile.getColor())) +
                        (tile.getConnection(Right) == nullptr ? " " : "-");
            lineStr3 += (tile.getConnection(Down) == nullptr ? "   " : " | ");
        }
        outStr += lineStr1 + "\n";
        outStr += lineStr2 + "\n";
        outStr += lineStr3 + "\n";
    }
    return outStr;
}

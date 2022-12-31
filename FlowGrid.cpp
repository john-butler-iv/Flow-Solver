#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "FlowGrid.h"
#include "FlowTile.h"
#include "FlowTypes.h"

using namespace std;

/// @brief given a FlowColor, returns a one-letter string containing a unique initial
/// @param color 
/// @return 
string colorInitial(FlowColor color) {
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
    grid = unique_ptr<FlowTile[]>(new FlowTile[height * width]);
}

void FlowGrid::addSource(int r, int c, FlowColor color){
    grid[index(r,c)].setAsSource(color);
}

int FlowGrid::index(int r, int c){
    return r * height + c;
}

shared_ptr<FlowTile> FlowGrid::getPointer(int r, int c){
    if(r >= 0 && r < height && c >= 0 && c < width)
        return grid[index(r,c)].getSharedPointer();
    return nullptr;
}

int FlowGrid::offsetRawIndex(int i, Direction dir){
    switch(dir){
        case Left:
            if(i % width == 0) return -1;
            return i - 1;
        case Right:
            if(i % width == width - 1) return -1;
            return i + 1;
        case Up:
            if(i < width) return -1;
            return i - width;
        case Down:
            if(i >= (height - 1) * width) return -1;
            return i + width;
        return -1;
    }
}

int FlowGrid::indexOffset(int r, int c, Direction dir){
    switch(dir){
        case Left:
            if(c == 0) return -1;
            return index(r, c-1);
        case Right:
            if(c == width - 1) return -1;
            return index(r, c+1);
        case Up:
            if(r == 0) return -1;
            return index(r-1, c);
        case Down:
            if(r == height - 1) return -1;
            return index(r+1, c);
        default:
            return -1;
    }
}

shared_ptr<FlowTile> FlowGrid::getPointerRelative(int r, int c, Direction dir) {
    if(r < 0 || r > height - 1 || c < 0 || c > width - 1)
        return nullptr;
    switch(dir){
        case Left:
            if(c == 0) return nullptr;
            break;
        case Right:
            if(c == width - 1) return nullptr;
            break;
        case Up:
            if(r == 0) return nullptr;
            break;
        case Down:
            if(r == height - 1) return nullptr;
            break;
    }
    
    return grid[indexOffset(r, c, dir)].getSharedPointer();
}

bool areNeighbors(int id1, int id2, int height, int width){
    int idDiff = id1 - id2;
    if(idDiff == 1)
        return id1 % width != 0;
    if(idDiff == -1)
        return id2 % width != 0;
    if(idDiff == -height || idDiff == height)
        return true;
    return false;
}

bool FlowGrid::wouldCreateLoop(shared_ptr<FlowTile> currentTile, shared_ptr<FlowTile> neighborTile){
    shared_ptr<FlowTile> currentTail = currentTile->getTailPointer();
    shared_ptr<FlowTile> neighborTail = neighborTile->getTailPointer();

    if(currentTail == currentTile && neighborTail == neighborTile) return false;

    vector<shared_ptr<FlowTile> > currentChain = currentTile->getCurrentChain();

    bool loopingNeighborPositions[height * width];
    for(int i = 0; i < height * width; i++){
        loopingNeighborPositions[i] = false;
    }
    for(int i = 0; i < currentChain.size(); i++){
        for(int dir = 0; dir < NUM_DIRECTIONS; dir++){
            int index = offsetRawIndex(currentChain[i]->getUniqId(), (Direction)dir);
            if(index == -1) continue;

            assert(index >= 0 && index < height * width);

            loopingNeighborPositions[index] = true;
        }
    }

    vector<shared_ptr<FlowTile> > neighborChain = neighborTile->getCurrentChain();
    for(int i = 1; i < neighborChain.size(); i++){
        assert(neighborChain[i]->getUniqId() >= 0 && neighborChain[i]->getUniqId() < height * width);
        if(loopingNeighborPositions[neighborChain[i]->getUniqId()]){
            return true;
        }
    }

    return false;
}

bool FlowGrid::validDirection(shared_ptr<FlowTile> currentTile, shared_ptr<FlowTile> neighborTile, Direction dir){
    return neighborTile != nullptr
            // wouldn't be invalid
            && neighborTile->remainingConnections() > 0
            // Tiles aren't already connected
            && currentTile->getConnection(dir) == nullptr
            && neighborTile->getConnection(OppositeDirection(dir)) == nullptr
            // color is compatible
            && (currentTile->getColor() == neighborTile->getColor()
            || currentTile->getColor() == Uncolored
            || neighborTile->getColor() == Uncolored)
            // would create a loop
            && !wouldCreateLoop(currentTile, neighborTile);
}

bool FlowGrid::doForcedMove(int r, int c){
    shared_ptr<FlowTile> currentTile = grid[index(r,c)].getSharedPointer();
    assert(currentTile->remainingConnections() >= 0);
    if(currentTile->remainingConnections() == 0){
        return false;
    }

    bool madeConnection = false;

    shared_ptr<FlowTile> neighborPtr;
    int totalValidDirections = 0;
    shared_ptr<FlowTile> validDirections[NUM_DIRECTIONS] = {
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };


    for(int dir = 0; dir < NUM_DIRECTIONS; dir++){
        neighborPtr = getPointerRelative(r,c, (Direction)dir);
        if(validDirection(currentTile, neighborPtr, (Direction) dir)){
            totalValidDirections++;
            validDirections[dir] = neighborPtr;
        }
    }

    if(totalValidDirections < currentTile->remainingConnections()){
        cout << toString() << endl;
        cout << currentTile -> getUniqId() << endl;
    }
    assert(totalValidDirections >= currentTile->remainingConnections());
    if(totalValidDirections == currentTile->remainingConnections()){
        for(int dir = 0; dir < NUM_DIRECTIONS; dir++){
            if(validDirections[dir] != nullptr){
                currentTile->connect(validDirections[dir], (Direction) dir);
                madeConnection = true;
            }
        }
    }
    return madeConnection;
}

bool FlowGrid::joinAdjacentColors(int r, int c){
    shared_ptr<FlowTile> currentTile = grid[index(r,c)].getSharedPointer();
    assert(currentTile->remainingConnections() >= 0);
    if(currentTile->remainingConnections() == 0)
        return false;
    if(currentTile-> getColor() == Uncolored)
        return false;

    shared_ptr<FlowTile> neighborPointer;

    for(int dir = 0 ; dir < NUM_DIRECTIONS; dir++){
        neighborPointer = getPointerRelative(r, c, (Direction) dir);
        if(neighborPointer != nullptr
                && neighborPointer->getColor() == currentTile->getColor()
                && currentTile->getConnection((Direction) dir) == nullptr){
            currentTile->connect(neighborPointer, (Direction) dir);
            return true;
        }
    }

    return false;
}


void FlowGrid::solve(){
    bool madeConnection;
    do {
        madeConnection = false;
        for(int r = 0; r < height; r++){
            for(int c = 0; c < width; c++){
                if(doForcedMove(r,c)){
                    madeConnection = true;
                }
                if(joinAdjacentColors(r, c)){
                    madeConnection = true;
                }
            }
        }
    } while(madeConnection);
}


const string FlowGrid::toString() {
    string outStr = "";

    outStr += "╔";
    for(int c = 0; c < width; c++){
        outStr += "=====";
    }
    outStr += "╗\n";
    for (int r = 0; r < height; r++) {
        string lineStr1 = "║";
        string lineStr2 = "║";
        string lineStr3 = "║";
        for (int c = 0; c < width; c++) {
            FlowTile tile = grid[r * height + c];
            lineStr1 += (tile.getConnection(Up) == nullptr ? "     " : "  |  ");
            lineStr2 += (tile.getConnection(Left) == nullptr ? "  " : "--") +
                        (colorInitial(tile.getColor())) +
                        (tile.getConnection(Right) == nullptr ? "  " : "--");
            lineStr3 += (tile.getConnection(Down) == nullptr ? "     " : "  |  ");
        }
        outStr += lineStr1 + "║\n";
        outStr += lineStr2 + "║\n";
        outStr += lineStr3 + "║\n";
    }
    outStr += "╚";
    for(int c = 0; c < width; c++){
        outStr += "=====";
    }
    outStr += "╝\n";
    return outStr;
}

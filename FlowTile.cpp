#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>

#include "FlowTypes.h"
#include "FlowTile.h"

using namespace std;


////////////////////
// Public Methods //
////////////////////

FlowTile::FlowTile() {
    sharedThis = shared_ptr<FlowTile>(this);
    uniqId = counter++;

    color = Uncolored;
    isSource = false;

    tail = sharedThis;

    for(int i = 0; i < NUM_DIRECTIONS; i++){
        connections[i] = nullptr;
    }
}

void FlowTile::setAsSource(FlowColor color){
    assert(this-> color == Uncolored);
    assert(color != Uncolored);
    assert(tail == sharedThis);
    assert(totalConnections() == 0);

    isSource = true;
    this-> color = color;
}

void FlowTile::connect(shared_ptr<FlowTile> otherTile, Direction toOtherTile) {
    assert(otherTile != nullptr);

    // if already connected, we don't have to worry about anything
    if(this -> connections[toOtherTile] == otherTile) {
        assert(otherTile->connections[OppositeDirection(toOtherTile)] == sharedThis);
        return;
    } 

    mergeColors(otherTile);
    physicallyConnect(otherTile, toOtherTile);
}

bool FlowTile::isInSolvedState(){
    return remainingConnections() == 0 
            && color != Uncolored
            && tail != sharedThis
            && tail-> isSource;
}

shared_ptr<FlowTile> FlowTile::getSharedPointer(){
    return this-> sharedThis;
}

const shared_ptr<FlowTile> FlowTile::getConnection(Direction dir){
    return this-> connections[dir];
}

const FlowColor FlowTile::getColor(){
    return this-> color;
}


/////////////////////
// Private Methods //
/////////////////////

int FlowTile::totalConnections() {
    int numConnections = 0;
    for(int i = 0; i < NUM_DIRECTIONS; i++) {
        if(connections[i] != nullptr){
            numConnections ++;
        }
    }

    return numConnections;
}

int FlowTile::remainingConnections() {
    if(isSource){
        return 1 - totalConnections();
    }
    return 2 - totalConnections();
}

/**
 * Merges the color of this FlowTile and otherTile.
 * If both paths have two different colors assigned, an error is thrown
 */
void FlowTile::mergeColors(shared_ptr<FlowTile> otherTile) {
    // no merging needs to take place
    if(this->color == otherTile->color)
        return;

    assert(otherTile->color == Uncolored
            || this->color == Uncolored);

    if (this->color != Uncolored) {
        otherTile->assignColor(this->color);
    } else { // otherTile-> color != Uncolored
        this->assignColor(otherTile->color);
    }
}

/**
 * Assigns a color to this sequence of FlowTiles. The current tile must be uncolored
 * @param color - Color to assign this FlowTile and all of the connected FlowTiles
 */
void FlowTile::assignColor(FlowColor color) {
    assert(this->color == Uncolored);
    this->color = color;
    // TODO if stack overflow becomes an issue, I can try to implement an iterative solution
    for(int dir = 0; dir < NUM_DIRECTIONS; dir++) {
        if(connections[dir] != nullptr && connections[dir]-> color != color) {
            connections[dir]-> assignColor(color);
        }
    }
}

/**
 * This source will be assigned to otherTile and vice versa if possible.
 * There must be an available source on each tile.
 */
void FlowTile::physicallyConnect(shared_ptr<FlowTile> otherTile, Direction toOtherTile) {
    // update direct connections
    Direction toThisTile = OppositeDirection(toOtherTile);
    assert(this -> connections[toOtherTile] == nullptr
            && otherTile -> connections[toThisTile] == nullptr);
    
    assert(this->remainingConnections() != 0
            && otherTile->remainingConnections() != 0);
    this->connections[toOtherTile] = otherTile;
    otherTile->connections[toThisTile] = sharedThis;


    // update tail pointers
    // we must be at the end of the chain to update tails
    assert(this-> tail != nullptr);
    assert(this-> tail-> tail == sharedThis);
    assert(otherTile-> tail != nullptr);
    assert(otherTile-> tail-> tail.get() == otherTile.get());

    // we should cannot create loops
    assert(this-> tail != otherTile);
    assert(otherTile-> tail != sharedThis);

    if (this->tail == sharedThis){
        this-> tail-> tail = otherTile-> tail;
        otherTile-> tail -> tail = sharedThis;
    } else {
        this-> tail-> tail = otherTile-> tail;
        otherTile-> tail -> tail = this-> tail;
    }
    // tail pointers in the middle of the chain don't have any meaning, so we don't have to update them.
}

vector<shared_ptr<FlowTile> > FlowTile::getCurrentChain(){
    assert(totalConnections() < 2);

    vector<shared_ptr<FlowTile> > currentChain;

    const int MAX_TILES = counter;
    bool visitedTiles[MAX_TILES];
    for(int i = 0; i < MAX_TILES; i++){
        visitedTiles[i] = false;
    }

    shared_ptr<FlowTile> tracer = nullptr;
    shared_ptr<FlowTile> nextTile = sharedThis;
    do {
        tracer = nextTile;
        currentChain.push_back(tracer);
        visitedTiles[tracer->getUniqId()] = true;
        nextTile = nullptr;


        for(int dir = 0; dir < NUM_DIRECTIONS; dir++){
            shared_ptr<FlowTile> connectedTile = tracer-> getConnection((Direction) dir);
            if(connectedTile != nullptr && !visitedTiles[connectedTile->getUniqId()]){
                assert(nextTile == nullptr);
                nextTile = connectedTile;
            }
        }
    } while (nextTile != nullptr);
    
    return currentChain;
}

bool FlowTile::getIsSource(){
    return isSource;
} 

int FlowTile::getUniqId(){
    return uniqId;
}

shared_ptr<FlowTile> FlowTile::getTailPointer(){
    return tail;
}

int FlowTile::counter = 0;
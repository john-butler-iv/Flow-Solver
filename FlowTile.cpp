#include <assert.h>
#include <memory>

#include "FlowTypes.h"

using namespace std;

class FlowTile {
    private:
        static int counter;

        int uniqId;
        bool isSource;
        FlowColor color;
        shared_ptr<FlowTile> connections[NUM_DIRECTIONS];
        shared_ptr<FlowTile> sharedThis;
        shared_ptr<FlowTile> tail;


        /**
         * Computes the total number of connections this FlowTile is connected to.
         */
        int totalConnections(){
            int numConnections = 0;
            for(int i = 0; i < NUM_DIRECTIONS; i++){
                if(connections[i] != nullptr){
                    numConnections ++;
                }
            }

            return numConnections;
        }

        /**
         * Determines if this FlowTile is allowed to have any more connections
         */
        bool areConnectionsFull(){
            return (isSource && totalConnections() == 1) ||
                    (!isSource && totalConnections() == 2);
        }

        /**
         * Merges the color of this FlowTile and otherTile.
         * If both paths have two different colors assigned, an error is thrown
         */
        void mergeColors(shared_ptr<FlowTile> otherTile){
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
        void assignColor(FlowColor color){
            assert(this->color != Uncolored);
            this->color = color;
            // TODO if stack overflow becomes an issue, I can try to implement an iterative solution
            for(int dir = 0; dir < NUM_DIRECTIONS; dir++){
                if(connections[dir] != nullptr && connections[dir]->color != color){
                    connections[dir]->assignColor(color);
                }
            }
        }

        /**
         * This source will be assigned to otherTile and vice versa if possible.
         * There must be an available source on each tile.
         */
        void physicallyConnect(shared_ptr<FlowTile> otherTile, Direction toOtherTile){
            // update direct connections
            Direction toThisTile = OppositeDirection(toOtherTile);
            assert(this -> connections[toOtherTile] == nullptr
                    && otherTile -> connections[toThisTile] == nullptr);
            assert(!this->areConnectionsFull() 
                    && !otherTile -> areConnectionsFull());
            this->connections[toOtherTile] = otherTile;
            otherTile->connections[toThisTile] = sharedThis;


            // update tail pointers
            // we must be at the end of the chain to update tails
            assert(this-> tail != nullptr);
            assert(this-> tail-> tail == sharedThis);
            assert(otherTile-> tail != nullptr);
            assert(otherTile-> tail-> tail == otherTile);

            // we should cannot create loops
            assert(this-> tail != otherTile);
            assert(otherTile-> tail != sharedThis);

            this-> tail-> tail = otherTile-> tail;
            otherTile-> tail -> tail = this-> tail;
            // tail pointers in the middle of the chain don't have any meaning, so we don't have to update them.
        }


    public:
        FlowTile(){
            sharedThis = shared_ptr<FlowTile>(this);
            uniqId = counter++;

            color = Uncolored;
            isSource = false;

            tail = sharedThis;

            for(int i = 0; i < NUM_DIRECTIONS; i++){
                connections[i] = nullptr;
            }
        }

        void setAsSource(FlowColor color){
            assert(this-> color == Uncolored);
            assert(color != Uncolored);
            assert(tail == sharedThis);
            assert(totalConnections() == 0);

            isSource = true;
            this-> color = color;
        }


        void connect(shared_ptr<FlowTile> otherTile, Direction toOtherTile) {
            assert(otherTile != nullptr);

            // if already connected, we don't have to worry about anything
            if(this -> connections[toOtherTile] == otherTile) {
                assert(otherTile->connections[OppositeDirection(toOtherTile)] == sharedThis);
                return;
            } 

            mergeColors(otherTile);
            physicallyConnect(otherTile, toOtherTile);
        }

        bool isInSolvedState(){
            return areConnectionsFull() 
                    && color != Uncolored
                    && tail != sharedThis
                    && tail-> isSource;
        }

        shared_ptr<FlowTile> getSharedPointer(){
            return this-> sharedThis;
        }

        const shared_ptr<FlowTile> getConnection(Direction dir){
            return this-> connections[dir];
        }

        const FlowColor getColor(){
            return this-> color;
        }
};
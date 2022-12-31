#include "FlowTypes.h"

#ifndef FLOW_GRID
#define FLOW_GRID

class FlowGrid {
    public:
        /**
         * Creates a blank flow grid for you to populate
         */
        FlowGrid(int height, int width);

        /**
         * Marks a particular tile as a source of the given color
         */
        void addSource(int x, int y, FlowColor color);
};

#endif
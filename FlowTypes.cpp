#include <string>

#include "FlowTypes.h"

Direction OppositeDirection(Direction dir) {
    return (Direction) (((int) dir + 2) % NUM_DIRECTIONS);
}

std::string FlowColorName(FlowColor color){
    switch(color){
        case Uncolored:
            return "uncolored";
        case Red:
            return "red";
        case Green:
            return "green";
        case Blue:
            return "blue";
        case Yellow:
            return "yellow";
        case Orange:
            return "orange";
        case Cyan:
            return "cyan";
        case Pink:
            return "pink";
        case Maroon:
            return "maroon";
        case Purple:
            return "purple";
        case White:
            return "white";
        case Grey:
            return "grey";
        case Lime:
            return "lime";
        case Tan:
            return "tan";
        case DarkBlue:
            return "dark blue";
        default:
            return "";
            }
}
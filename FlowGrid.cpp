#include <memory>
#include <string>
#include <vector>

#include "FlowTile.h"
#include "FlowTypes.h"

using namespace std;

class FlowGrid {
    private:
        int height;
        int width;
        vector<FlowTile> grid;
    
        string colorInitial(FlowColor color){
            switch(color){
                case Red: return "R";
                case Green: return "G";
                case Blue: return "B";
                case Yellow: return "Y";
                case Orange: return "O";
                case Cyan: return "C";
                case Pink: return "P";
                case Maroon: return "M";
                case Purple: return "P";
                case White: return "W";
                case Grey: return "G";
                case Lime: return "L";
                case Tan: return "T";
                case DarkBlue: return "D";
                case Uncolored:
                default:
                    return " ";
            }
        }
    public:
        FlowGrid(int height, int width) {
            this-> height = height;
            this-> width = width;
            grid = vector<FlowTile>(height * width);
            for(int r = 0; r < height; r++){
                for(int c = 0; c < width; c++){
                    grid[r * height + c] = FlowTile();
                }
            }
        }

        void addSource(int x, int y, FlowColor color){
            grid[y * height + x].setAsSource(color);
        }

        string toString() {
            string outStr = "";
            for(int r = 0; r < height; r++){
                string lineStr1 = "";
                string lineStr2 = "";
                string lineStr3 = "";
                for(int c = 0; c < width; c++){
                    FlowTile tile = grid[r * height + c];
                   lineStr1 += (tile.getConnection(Up) == nullptr? "   " : " | ");
                   lineStr2 += (tile.getConnection(Left) == nullptr? " " : "-") + 
                               (colorInitial(tile.getColor())) + 
                               (tile.getConnection(Right) == nullptr? " " : "-");
                   lineStr3 += (tile.getConnection(Down) == nullptr? "   " : " | ") ; 
                }
                outStr += lineStr1 + "\n";
                outStr += lineStr2 + "\n";
                outStr += lineStr3 + "\n";
            }
            return outStr;
        }
};
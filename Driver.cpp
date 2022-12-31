#include <algorithm>
#include <iostream>
#include <locale>
#include <string>

#include "FlowGrid.h"
#include "FlowGraphics.h"

using namespace std;


bool getIntsCore(int &value1, int &value2, string query){
    cout << query << endl
            << " - ";

    string value1Raw,value2Raw;

    cin >> value1Raw >> value2Raw;

    value1 = stoi("0" + value1Raw);
    value2 = stoi("0" + value2Raw);
    return true;
}

void getInts(int &value1, int &value2, string query, string errMsg) {
    if(errMsg == "") errMsg = "That was an invalid pair of numbers.";
    while(!getIntsCore(value1, value2, query)) {
        cout << errMsg << endl;
    }
}

bool getBoolCore(bool &value, string query) {
    cout << query << endl
            << " - ";

    string valueRaw;

    cin >> valueRaw;

    transform(valueRaw.begin(), valueRaw.end(), valueRaw.begin(), ::toupper);
    
    if(valueRaw == "Y" || valueRaw == "YES"){
        value = true;
        return true;
    }
    if(valueRaw == "N" || valueRaw == "NO"){
        value = false;
        return true;
    }
    return false;
}

void getBool(bool &value, string query, string errMsg){
    if(errMsg == "") errMsg = "That was an invalid response.";
    while(!getBoolCore(value, query)){
        cout << errMsg << endl;
    }
}

void getHeightWidth(int &height, int &width){
    string query = "Please enter the height and width of the board as \"{height} {width}\":";
    string errMsg = "You did not enter a valid height and/or width.";
    getInts(height, width, query, errMsg);
}

struct Point {
    int x;
    int y;
};

bool getSourcePts(struct Point &sourceA, struct Point &sourceB){
    string confirmQuery = "Would you like to add a pair of sources?";
    bool shouldQueryPts;
    getBool(shouldQueryPts, confirmQuery, "");
    if(!shouldQueryPts) return false;

    string errorMsg = "You didn't enter valid coordinates. Are you sure you want to add a pair of sources?";

    string firstPointQuery = "Enter the x and y coordinates of the first point as \"{x} {y}\":";
    while (!getIntsCore(sourceA.x, sourceA.y, firstPointQuery)){
        getBool(shouldQueryPts, firstPointQuery, "");
        if(!shouldQueryPts) return false;
    }

    string secondPointQuery = "Enter the x and y coordinates of the second point as \"{x} {y}\":";
    while (!getIntsCore(sourceB.x, sourceB.y, secondPointQuery)){
        getBool(shouldQueryPts, secondPointQuery, "");
        if(!shouldQueryPts) return false;
    }

    return true;
}



void board1(FlowGrid &flowGrid){
    flowGrid.addSource(0,0, Red);
    flowGrid.addSource(4,1, Red);
    flowGrid.addSource(0,2, Green);
    flowGrid.addSource(3,1, Green);
    flowGrid.addSource(1,2, Blue);
    flowGrid.addSource(4,2, Blue);
    flowGrid.addSource(0,4, Yellow);
    flowGrid.addSource(3,3, Yellow);
    flowGrid.addSource(1,4, Orange);
    flowGrid.addSource(4,3, Orange);
}

void board2(FlowGrid &flowGrid){
    flowGrid.addSource(0,0, Green);
    flowGrid.addSource(0,1, Yellow);
    flowGrid.addSource(0,2, Cyan);
    flowGrid.addSource(0,4, Red);
    flowGrid.addSource(0,5, Blue);
    flowGrid.addSource(1,4, Orange);
    flowGrid.addSource(2,2, Cyan);
    flowGrid.addSource(3,2, Red);
    flowGrid.addSource(4,0, Green);
    flowGrid.addSource(4,2, Orange);
    flowGrid.addSource(5,0, Yellow);
    flowGrid.addSource(5,2, Blue);
}

void board3(FlowGrid &flowGrid){
    flowGrid.addSource(0,6, Blue);
    flowGrid.addSource(1,5, Orange);
    flowGrid.addSource(1,6, Red);
    flowGrid.addSource(2,1, Orange);
    flowGrid.addSource(3,3, Green);
    flowGrid.addSource(3,4, Cyan);
    flowGrid.addSource(4,2, Green);
    flowGrid.addSource(4,4, Yellow);
    flowGrid.addSource(5,4, Red);
    flowGrid.addSource(5,5, Yellow);
    flowGrid.addSource(6,5, Blue);
    flowGrid.addSource(6,6, Cyan);
}

void board4(FlowGrid &flowGrid){
    flowGrid.addSource(0,4, Red);
    flowGrid.addSource(1,4, Cyan);
    flowGrid.addSource(1,6, Green);
    flowGrid.addSource(1,7, Yellow);
    flowGrid.addSource(2,4, Green);
    flowGrid.addSource(2,5, Yellow);
    flowGrid.addSource(2,7, Cyan);
    flowGrid.addSource(3,3, Orange);
    flowGrid.addSource(3,4, Blue);
    flowGrid.addSource(4,4, Orange);
    flowGrid.addSource(5,4, Blue);
    flowGrid.addSource(7,1, Red);
}

void board5(FlowGrid &flowGrid){
    flowGrid.addSource(1,1, Red);
    flowGrid.addSource(1,2, Cyan);
    flowGrid.addSource(1,3, Blue);
    flowGrid.addSource(2,3, Cyan);
    flowGrid.addSource(2,4, Orange);
    flowGrid.addSource(2,6, Orange);
    flowGrid.addSource(2,7, Blue);
    flowGrid.addSource(3,7, Pink);
    flowGrid.addSource(4,1, Green);
    flowGrid.addSource(4,3, Green);
    flowGrid.addSource(4,4, Red);
    flowGrid.addSource(5,0, Yellow);
    flowGrid.addSource(5,1, Pink);
    flowGrid.addSource(5,7, Maroon);
    flowGrid.addSource(5,8, Purple);
    flowGrid.addSource(6,1, Purple);
    flowGrid.addSource(6,2, Yellow);
    flowGrid.addSource(7,1, Maroon);
}

int main(int argc, char* argv[]){
    /*
    int height, width;
    getHeightWidth(height, width);
    FlowGrid flowGrid(height, width);

    struct Point ptA, ptB;
    int currentColor = ((int) Uncolored) + 1;
    while(currentColor <= (int) DarkBlue && getSourcePts(ptA, ptB)) {
        flowGrid.addSource(ptA.x, ptA.y, (FlowColor) currentColor);
        flowGrid.addSource(ptB.x, ptB.y, (FlowColor) currentColor);
    }
    */

    FlowGrid flowGrid(9,9);
    board5(flowGrid);

    displayGrid(flowGrid);

    flowGrid.solve();

    displayGrid(flowGrid);
}
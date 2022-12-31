#include <algorithm>
#include <iostream>
#include <locale>
#include <string>

#include "FlowGrid.h"
//#include "FlowGraphics.h"

using namespace std;


bool getIntsCore(int &value1, int &value2, string query){
    cout << query << endl
            << " - ";

    string value1Raw,value2Raw;

    cin >> value1Raw >> value2Raw;

    value1 = stoi(value1Raw);
    value2 = stoi(value2Raw);
    return true;
}

void getInts(int &value1, int &value2, string query, string errMsg){
    if(errMsg == "") errMsg = "That was an invalid pair of numbers.";
    while(!getIntsCore(value1, value2, query)){
        cout << errMsg << endl;
    }
}

bool getBoolCore(bool &value, string query){
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



int main(int argc, char* argv[]){
    int height, width;
    getHeightWidth(height, width);
    FlowGrid flowGrid(height, width);

    struct Point ptA, ptB;
    int currentColor = ((int) Uncolored) + 1;
    while(currentColor <= (int) DarkBlue && getSourcePts(ptA, ptB)){
        flowGrid.addSource(ptA.x, ptA.y, (FlowColor) currentColor);
        flowGrid.addSource(ptB.x, ptB.y, (FlowColor) currentColor);
    }
}
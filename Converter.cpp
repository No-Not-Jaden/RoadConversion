#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <map>
using namespace std;

class Road {
    public:
        bool along;
        int length;
        bool plow;
};

void addRoad(string roadName, Road road);

int keySize = 0;
string roadKeys[200];
Road roadMap[200];


int main(int argc, char* argv[]) {
    string fileName = "roads.csv";
    if (argc > 1) {
        fileName = argv[1];
    }
    ifstream file;
    file.open(fileName);
    if (!file) {
        cout << "Error opening " << fileName << " file" << endl;
        cout << "Specify a file path of a .csv as an argument, or have a roads.csv file in the local directory." << endl;
        return 0;
    }

    string entry;
    string tmpstr;
    while (file >> entry) {
        entry.erase(std::remove(entry.begin(), entry.end(), '\"'), entry.end()); // remove any quote strings
        bool traverseType;
        if (entry.compare("PLOW") == 0) {
            traverseType = true;
        } else if (entry.compare("DIRECTIONAL") == 0) {
            file >> tmpstr;
            traverseType = false;
        } else {
            continue;
        }
        string feet;
        file >> feet;
        int amount = stoi(feet);
        file >> tmpstr; // FEET
        string relative; // ALONG, ACROSS
        file >> relative;
        bool along = relative.compare("ALONG") == 0;
        string roadName;
        file >> roadName;
        // add to record
        Road readRoad;
        readRoad.along = along;
        readRoad.length = amount;
        readRoad.plow = traverseType;
        addRoad(roadName, readRoad);
    }
    file.close();

    // sort roads
    for (int j = 0; j < 100; j++)
    {
        bool fullySorted = true;
        for (int i = 0; i < keySize-1; i++)
        {
            if (roadKeys[i].compare(roadKeys[i+1]) > 0 || (roadKeys[i].compare(roadKeys[i+1]) == 0 && roadMap[i].length < roadMap[i+1].length)) {
                // switch values
                string oldValue = roadKeys[i];
                Road oldRoad = roadMap[i];
                roadKeys[i] = roadKeys[i+1];
                roadMap[i] = roadMap[i+1];
                roadKeys[i+1] = oldValue;
                roadMap[i+1] = oldRoad;
                fullySorted = false;
            } 
        }
        if (fullySorted) {
            break;
        }
        
    }
    
    int plowTotal = 0;
    int boreTotal = 0;
    int boreAcrossTotal = 0;
    for (int i = 0; i < keySize; i++)
    {
        if (roadMap[i].plow) {
            cout << "Plow ";
            plowTotal+= roadMap[i].length;
        } else {
            cout << "Directional bore ";
            if (roadMap[i].along) {
                boreTotal+= roadMap[i].length;
            } else {
                boreAcrossTotal+= roadMap[i].length;
            }
        }
        if (roadMap[i].along) {
            cout << "along ";
        } else {
            cout << "across ";
        }
        cout << roadKeys[i] << " a total of " << roadMap[i].length << " feet of fiber cable in 1-2\" SDR 13.5 HDPE Type 3 conduit" << endl;
    }

    cout << "Plow total: " << plowTotal << " feet." << endl;
    cout << "Directional Bore Along Total: " << boreTotal << " feet." << endl;
    cout << "Directional Bore Across Total: " << boreAcrossTotal << " feet." << endl;
    cout << "Total Plow + Bore: " << boreTotal + plowTotal << " feet." << endl;

    if (keySize < 1) {
        return 0;
    }
    cout << "Roads: " << roadKeys[0];
    for (int i = 1; i < keySize; i++)
    {
        if (roadKeys[i-1].compare(roadKeys[i]) == 0) {
            continue;
        }
        cout << ", " << roadKeys[i];
    }
    cout << endl;
    
}


void addRoad(string roadName, Road road) {
    // look for existing road
    for (int i = 0; i < keySize; i++)
    {
        if (roadKeys[i].compare(roadName) == 0) {
            // found road -> check if the plow and along values are the same
            if (roadMap[i].along == road.along && roadMap[i].plow == road.plow) {
                // add to road
                roadMap[i].length+= road.length;
                return;
            }
        }
    }
    // no road found -> create new entry
    roadKeys[keySize] = roadName;
    roadMap[keySize] = road;
    keySize++;
}




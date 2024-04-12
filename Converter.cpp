#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <filesystem>
#include <algorithm>
using namespace std;

class Road
{
public:
    bool along;
    int length;
    bool plow;
};

void addRoad(string roadName, Road road);
void countStage3(string fileName);

int keySize = 0;
string roadKeys[200];
Road roadMap[200];

string inputFiles = "input_files";
string outputFiles = "output_files";
string cabelType = "fiber cable in 1-2\" SDR 13.5 HDPE Type 3 conduit";

int main(int argc, char *argv[])
{
    bool newFile = !filesystem::exists("cabel_type.txt");

    if (newFile)
    {
        ofstream suffix;
        suffix.open("cabel_type.txt");
        if (suffix.is_open())
        {
            suffix << "fiber cable in 1-2\" SDR 13.5 HDPE Type 3 conduit" << endl;
            suffix.close();
        }
    }
    else
    {
        ifstream suffix;
        suffix.open("cabel_type.txt");
        getline(suffix, cabelType);
        suffix.close();
    }

    // make directory in case it wasn't there
    filesystem::create_directory(inputFiles);
    filesystem::create_directory(outputFiles);
    // create iterator for that directory
    filesystem::directory_iterator it(inputFiles);
    // go through every file in that directory
    for (const auto &entry : it)
    {
        if (entry.is_regular_file())
        {
            string fileName = entry.path().string();
            fileName.erase(0, inputFiles.length() + 1);
            countStage3(fileName);
        }
    }
}

void countStage3(string fileName)
{
    ifstream file;
    string relativeName = inputFiles;
    relativeName.append("/").append(fileName);

    file.open(relativeName);
    if (!file)
    {
        cout << "Error opening " << relativeName << " file" << endl;
        return;
    }

    string entry;
    string tmpstr;
    while (file >> entry)
    {
        entry.erase(std::remove(entry.begin(), entry.end(), '\"'), entry.end()); // remove any quote strings
        bool traverseType;
        if (entry.compare("PLOW") == 0)
        {
            traverseType = true;
        }
        else if (entry.compare("DIRECTIONAL") == 0)
        {
            file >> tmpstr;
            traverseType = false;
        }
        else
        {
            continue;
        }
        string feet;
        file >> feet;
        int amount = stoi(feet);
        file >> tmpstr;  // FEET
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
        for (int i = 0; i < keySize - 1; i++)
        {
            // Sorts by alphabetical / Numerical order for the road
            // then bore along, bore across, plow
            if (roadKeys[i].compare(roadKeys[i + 1]) > 0 || (roadKeys[i].compare(roadKeys[i + 1]) == 0 && roadMap[i].plow && !roadMap[i + 1].plow) || (roadKeys[i].compare(roadKeys[i + 1]) == 0 && !roadMap[i].plow && !roadMap[i + 1].plow && !roadMap[i].along && roadMap[i + 1].along))
            {
                // switch values
                string oldValue = roadKeys[i];
                Road oldRoad = roadMap[i];
                roadKeys[i] = roadKeys[i + 1];
                roadMap[i] = roadMap[i + 1];
                roadKeys[i + 1] = oldValue;
                roadMap[i + 1] = oldRoad;
                fullySorted = false;
            }
        }
        if (fullySorted)
        {
            break;
        }
    }

    // get output file name
    string outputFile = outputFiles;
    outputFile.append("/").append(fileName);
    outputFile.erase(outputFile.find('.'));
    outputFile.append(".txt");
    // open stream
    ofstream output;
    output.open(outputFile);

    cout << endl
         << "-------------- " << fileName << " --------------" << endl;

    int plowTotal = 0;
    int boreTotal = 0;
    int boreAcrossTotal = 0;
    string lastRoadName = roadKeys[0];
    for (int i = 0; i < keySize; i++)
    {
        if (lastRoadName.compare(roadKeys[i]) != 0)
        {
            // add space between different roads
            output << endl;
            cout << endl;
            lastRoadName = roadKeys[i];
        }
        if (roadMap[i].plow)
        {
            output << "Plow ";
            cout << "Plow ";
            plowTotal += roadMap[i].length;
        }
        else
        {
            output << "Directional bore ";
            cout << "Directional bore ";
            if (roadMap[i].along)
            {
                boreTotal += roadMap[i].length;
            }
            else
            {
                boreAcrossTotal += roadMap[i].length;
            }
        }
        if (roadMap[i].along)
        {
            output << "along ";
            cout << "along ";
        }
        else
        {
            output << "across ";
            cout << "across ";
        }
        output << roadKeys[i] << " a total of " << roadMap[i].length << " feet of " << cabelType << endl;
        cout << roadKeys[i] << " a total of " << roadMap[i].length << " feet of " << cabelType << endl;
    }

    output << endl;
    output << "Plow total: " << plowTotal << " feet." << endl;
    output << "Directional Bore Along Total: " << boreTotal << " feet." << endl;
    output << "Directional Bore Across Total: " << boreAcrossTotal << " feet." << endl;
    output << "Total Plow + Bore: " << boreTotal + boreAcrossTotal + plowTotal << " feet." << endl;

    cout << endl;
    cout << "Plow total: " << plowTotal << " feet." << endl;
    cout << "Directional Bore Along Total: " << boreTotal << " feet." << endl;
    cout << "Directional Bore Across Total: " << boreAcrossTotal << " feet." << endl;
    cout << "Total Plow + Bore: " << boreTotal + boreAcrossTotal + plowTotal << " feet." << endl;

    if (keySize < 1)
    {
        return;
    }
    // gather unique roads in a seperate list
    int numRoads = 1;
    string uniqueRoads[200];
    uniqueRoads[0] = roadKeys[0];
    for (int i = 1; i < keySize; i++)
    {
        if (roadKeys[i - 1].compare(roadKeys[i]) == 0)
        {
            continue;
        }
        uniqueRoads[numRoads] = roadKeys[i];
        numRoads++;
    }

    output << endl
           << numRoads << " different roads in this segment." << endl;
    cout << endl
         << numRoads << " different roads in this segment." << endl;
    if (numRoads == 1)
    {
        output << uniqueRoads[0];
        cout << uniqueRoads[0];
    }
    else if (numRoads == 2)
    {
        output << uniqueRoads[0] << " and " << uniqueRoads[1];
        cout << uniqueRoads[0] << " and " << uniqueRoads[1];
    }
    else
    {
        for (int i = 0; i < numRoads - 1; i++)
        {
            output << uniqueRoads[i] << ", ";
            cout << uniqueRoads[i] << ", ";
        }
        output << "and " << uniqueRoads[numRoads - 1];
        cout << "and " << uniqueRoads[numRoads - 1];
    }

    output << endl;
    cout << endl
         << "---------------------------------------" << endl
         << endl;

    output.close();
}

void addRoad(string roadName, Road road)
{
    // look for existing road
    for (int i = 0; i < keySize; i++)
    {
        if (roadKeys[i].compare(roadName) == 0)
        {
            // found road -> check if the plow and along values are the same
            if (roadMap[i].along == road.along && roadMap[i].plow == road.plow)
            {
                // add to road
                roadMap[i].length += road.length;
                return;
            }
        }
    }
    // no road found -> create new entry
    roadKeys[keySize] = roadName;
    roadMap[keySize] = road;
    keySize++;
}

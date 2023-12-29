#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "parser.h"

using namespace std;

vector<Configuration> Parser(string nameFile) {
    ifstream jsonFile;
    jsonFile.open(nameFile);
    if (!jsonFile.is_open()) {
        cout << "The file was not open!" << '\n';
        exit(1);
    }

    vector<Configuration> configs;
    string line;
    Configuration currentConfig;

    json data = json::parse(jsonFile);
    int jobs_number = data["jobs_number"];

    for (int i = 1; i <= jobs_number; ++i) {
        string ID = "job_" + to_string(i);
        currentConfig.id = data[ID]["id"];
        currentConfig.parents.push_back(data[ID]["parent"]);
        currentConfig.path = data[ID]["path"];
        configs.push_back(currentConfig);
        currentConfig.parents.clear();
    }
    
    jsonFile.close();
    return configs;
}
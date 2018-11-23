#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include "mygraphlib.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <map>
#include "cppalgorithm.h"

using namespace std;

typedef struct structParams
{
    string inputFile;
    //string    outputFile;
} Params;

void readCheckParams(Params &params, int argc, char *argv[]);
void readInstance(Digraph &g, DNodePosMap &posx, DNodePosMap &posy, map<int, DNode> &nameMap, ArcValueMap &distance, int &n, int &m, string filename);
#endif // MAIN_H

#ifndef CPPALGORITHM_H
#define CPPALGORITHM_H
#include <vector>
#include <float.h>
#include <math.h>
#include <set>
#include <lemon/list_graph.h>
#include <lemon/dijkstra.h>
#include "mygraphlib.h"
#include <lemon/matching.h>
#include <lemon/core.h>
#include <lemon/adaptors.h>
#include <lemon/connectivity.h>
#include <lemon/euler.h>
#include <chrono>
#include <utility>
#include <list>

typedef lemon::Dijkstra<Digraph, ArcValueMap> SptSolver;

class CPPAlgorithm
{
    private:
        static void getEulerianPath(Digraph &g, vector<Arc> &solution);
        static void addPath(Digraph &g, list<int> &path);
        static void preprocess(Digraph &g);
    public:
        static void run(Digraph &g, ArcValueMap &distance, vector<Arc> &solution, DNodeIntMap &components);
};

#endif // CPPALGORITHM_H

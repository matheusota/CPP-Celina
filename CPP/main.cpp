#include "main.h"

using namespace std;

int main(int argc, char *argv[])
{
    Digraph g;
    DNodePosMap posx(g);
    DNodePosMap posy(g);
    map<int, DNode> nameMap;
    ArcValueMap distance(g);
    vector<Arc> solution;
    DNodeIntMap components(g);

    int n, m;
    int time;

    // read parameters from command line
    Params params;
    readCheckParams(params, argc, argv);

    // read instance from file
    readInstance(g, posx, posy, nameMap, distance, n, m, "in/" + params.inputFile);

    // get eulerian tour
    CPPAlgorithm::run(g, distance, solution, components);

    // print the read graph
    CreateCPPGraph(g, posx, posy, solution, components);

    return 0;
}

// read argv params
void readCheckParams(Params &params, int argc, char *argv[])
{
    params.inputFile  = "";

    // Read
    for(int i = 1; i < argc; i++){
        const string arg(argv[i]);
        string next;

        if((i+1) < argc){
            next = string(argv[i+1]);
        }
        else{
            next = string("");
        }

        if(arg.find("-i") == 0 && next.size() > 0){
            params.inputFile = next;
            i++;
            continue;
        }

        cerr << "Invalid parameter." << endl;
        exit(1);
    }

    // Check
    if(params.inputFile == ""){
        cerr << "Input file should be specified" << endl;
        exit(1);
    }
}

// read file and create corresponding graph on the instance variable
void readInstance(Digraph &g, DNodePosMap &posx, DNodePosMap &posy, map<int, DNode> &nameMap, ArcValueMap &distance, int &n, int &m, string filename)
{
    ifstream ifile;
    int nameu, namev;
    double posx_aux, posy_aux;
    DNode u, v;
    string STR;

    // open file
    ifile.open(filename.c_str());
    if (!ifile) {
        cerr << "File '" << filename << "' does not exist.\n"; exit(0);
    }

    // read n and m
    ifile >> n >> m >> ws;

    // read nodes: <node_id> <node_weight> <posx> <posy>
    getline(ifile,STR);
    for(int i = 0; i < n; i++){
        ifile >> nameu >> posx_aux >> posy_aux >> ws;

        u = g.addNode();
        posx[u] = posx_aux;
        posy[u] = posy_aux;
        nameMap[nameu] = u;
    }

    // read edges: <from> <to>
    getline(ifile,STR);
    for(int i = 0; i < m; i++){
        ifile >> nameu >> namev >> ws;
        u = nameMap[nameu];
        v = nameMap[namev];
        Arc a = g.addArc(u, v);
        //Arc a2 = g.addArc(v, u);
        distance[a] = hypot((posx[v] - posx[u]), (posy[v] - posy[u]));
        //distance[a2] = hypot((posx[v] - posx[u]), (posy[v] - posy[u]));
    }

    ifile.close();
}

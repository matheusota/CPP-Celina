#include "cppalgorithm.h"

void CPPAlgorithm::getEulerianPath(Digraph &g, vector<Arc> &solution){
    for(DiEulerIt<Digraph> e(g); e != INVALID; ++e)
        solution.push_back(e);
}

void CPPAlgorithm::addPath(Digraph &g, list<int> &path){
    for(auto it = path.begin(); next(it) != path.end(); ++it){
        int id1 = *it;
        int id2 = *(next(it));

        DNode u = g.nodeFromId(id1);
        DNode v = g.nodeFromId(id2);

        g.addArc(u, v);
    }
}

void CPPAlgorithm::preprocess(Digraph &g){
    // remove one degree nodes
    while(true){
        bool deleted = false;
        vector<int> toDelete;

        for(DNodeIt v(g); v != INVALID; ++v){
            int in = 0;
            int out = 0;

            for(InArcIt a(g, v); a != INVALID; ++a)
                in++;

            for(OutArcIt a(g, v); a != INVALID; ++a)
                out++;

            if(in + out == 1){
                toDelete.push_back(g.id(v));
                deleted = true;
            }
        }

        if(!deleted)
            return;

        for(auto v_id : toDelete){
            DNode v = g.nodeFromId(v_id);
            g.erase(v);
        }
    }
}

void CPPAlgorithm::run(Digraph &g, ArcValueMap &distance, vector<Arc> &solution, DNodeIntMap &components){
    // remove one degree nodes
    CPPAlgorithm::preprocess(g);

    // first check if the graph is strongly connected
    if(!stronglyConnected(g)){
        cout << "Graph is not strongly connected!" << endl;

        // set the components in the map
        int k = countStronglyConnectedComponents(g);
        cout << "Number of Strongly Connected Components: " << k << endl;
        stronglyConnectedComponents(g, components);
        return;
    }

    // first we count the in and out degrees and save the wrong degree vertices
    vector<int> D_plus;
    vector<int> D_minus;

    for(DNodeIt v(g); v != INVALID; ++v){
        int in = 0;
        int out = 0;

        for(InArcIt a(g, v); a != INVALID; ++a)
            in++;

        for(OutArcIt a(g, v); a != INVALID; ++a)
            out++;

        int delta = out - in;

        //cout << "delta(" << g.id(v) << ") : " << delta << endl;
        if(delta > 0)
            D_plus.push_back(g.id(v));
        else if(delta < 0)
            D_minus.push_back(g.id(v));
    }

    /*
    cout << "D+ : ";
    for(auto d : D_plus)
        cout << d << ", ";
    cout << endl;

    cout << "D- : ";
    for(auto d : D_minus)
        cout << d << ", ";
    cout << endl;
    */

    // we need to add edges to make the graph eulerian
    if(D_plus.size() > 0){
        map<DNode, map<DNode, pair<double, list<int>>>> minPaths;
        SptSolver spt(g, distance);

        // run dijkstra algorithm for each vertex of D-
        for(auto u_id: D_minus){
            DNode u = g.nodeFromId(u_id);
            spt.run(u);

            // now we get the minimum path for each vertex of D+
            for(auto v_id: D_plus){
                DNode v = g.nodeFromId(v_id);
                list<int> path;
                double pathCost = spt.dist(v);

                //cout << "path from " << u_id <<  " to " << v_id << endl;
                for (DNode curr = v; curr != u; curr = spt.predNode(curr)){
                    if (curr != INVALID && spt.reached(curr)){
                        path.push_front(g.id(curr));
                        //cout << g.id(curr) << ", ";
                    }
                }
                path.push_front(u_id);
                //cout << u_id << ", ";
                //cout << endl;

                if(u_id < v_id)
                    minPaths[u][v] = make_pair(pathCost, path);
                else
                    minPaths[v][u] = make_pair(pathCost, path);
            }
        }

        // create a bipartite graph for D+ and D-
        ListGraph bpGraph;
        NodeIntMap nodeMap(bpGraph);
        map<int, Node> reverseNodeMap;

        // add D- as red nodes and D+ as blue nodes
        for(auto v_id: D_minus){
            Node v = bpGraph.addNode();
            nodeMap[v] = v_id;
            reverseNodeMap[v_id] = v;
        }

        for(auto v_id: D_plus){
            Node v = bpGraph.addNode();
            nodeMap[v] = v_id;
            reverseNodeMap[v_id] = v;
        }

        // add edges in the bipartite graph, the cost of the edges is given by the minPath
        EdgeValueMap bpWeights(bpGraph);

        for(auto u_id: D_minus){
            Node u = reverseNodeMap[u_id];

            for(auto v_id: D_plus){
                Node v = reverseNodeMap[v_id];

                Edge e = bpGraph.addEdge(u, v);
                bpWeights[e] = -minPaths[g.nodeFromId(u_id)][g.nodeFromId(v_id)].first;
            }
        }

        // run max matching algorithm
        MaxWeightedPerfectMatching<ListGraph, EdgeValueMap> algorithm(bpGraph, bpWeights);
        algorithm.run();

        // for each matching edge, add the corresponding path in the graph
        for(EdgeIt e(bpGraph); e != INVALID; ++e){
            if(algorithm.matching(e)){
                Node u = bpGraph.u(e);
                Node v = bpGraph.v(e);
                int u_id = nodeMap[u];
                int v_id = nodeMap[v];

                if(u_id < v_id)
                    CPPAlgorithm::addPath(g, minPaths[g.nodeFromId(u_id)][g.nodeFromId(v_id)].second);
                else
                    CPPAlgorithm::addPath(g, minPaths[g.nodeFromId(v_id)][g.nodeFromId(u_id)].second);
            }
        }
    }

    // at this point, the graph should be eulerian
    CPPAlgorithm::getEulerianPath(g, solution);
}

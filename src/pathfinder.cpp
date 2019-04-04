/*
 * CSE100 Project 3 - pathfinder.cpp
 * Author: Zhaopu Wang and Yanxun Li
 * Date: 03/04/2019
 * This is the main function of finding path.
 */ 

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <chrono>
#include "Graph.hpp"

using namespace std;

/*
 * print error message
 */
void usage(char* program_name) {
    cerr << program_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << program_name
         << " friendship_pairs_file test_pairs_file output_file"
         << endl;
    exit(-1);
}

/*
 * The main function loads the graph from a graph input file, 
 * find the shortest paths for the pairs in a pair input file
 * and output the paths to an output file.
 * Param: argc - 4 if valid, argv - the array of args
 * Return: 0 if successful, 1 if error
 */
int main(int argc, char* argv[]) {
    if (argc != 4) {
      usage(argv[0]);
    }

    char* graph_filename = argv[1];
    char* pairs_filename = argv[2];
    char* output_filename = argv[3];

    ifstream infile;
    ofstream outfile;
    infile.open(pairs_filename);
    outfile.open(output_filename);
   
    auto startTime = chrono::high_resolution_clock::now();

    Graph g;
    cout << "Reading graph ...\n";
    g.loadFromFile(graph_filename);

    while (infile) {
        // read a pair of IDs from the input file:
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector<int> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(stoi(s));
        }

        if (record.size() != 2) {
            continue;
        }

        // find the shortest path:
        Node* from = g.getNode(record[0]);
        Node* to = g.getNode(record[1]);

        cout << "Computing shortest path between id " << record[0]
            << " and id " << record[1] << "\n";

        if (!g.pathfinder(from,to)) {
            outfile << "\n";
            continue;
        }

        Node* curr = to;
        stack<int> stk;
        stk.push(curr->index);
        while (curr->prev != -1) {
            curr = g.getNode(curr->prev);
            stk.push(curr->index);
        }

        // retrieve the path and write it to the output file:
        while (true) {
            outfile << stk.top();
            stk.pop();
            if (stk.empty())
                break;
            outfile << " ";
        }

        if (infile)
            outfile << "\n";
    }
    
    auto endTime = chrono::high_resolution_clock::now();
    cout << "Elapsed Time (nanoseconds): " 
       << chrono::duration_cast<chrono::nanoseconds>(endTime-startTime).count()
       << "\n";

    if (!infile.eof()) {
        return 1;
    }

    infile.close();
    outfile.close();
    return 0;
}  

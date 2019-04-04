#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Graph.hpp"

using namespace std;

/*
 * print error message
 */
void usage(char* program_name) {
    cerr << program_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << program_name
         << " friendship_pairs_file k_value output_file"
         << endl;
    exit(-1);
}

/*
 * The main function loads the graph from a graph input file,
 * find a list of invitees who will have at least k friends in a party
 * and output the list to an output file.
 * Param: argc - 4 if valid, argv - the array of args
 * Return: 0 if successful, 1 if error
 */
int main(int argc, char* argv[]) {
    if (argc != 4) {
      usage(argv[0]);
    }
    char* graph_filename = argv[1];
    char* output_filename = argv[3];
    istringstream ss(argv[2]);

    ifstream infile;
    ofstream outfile;
    infile.open(graph_filename);
    outfile.open(output_filename);
    int k;
    ss >> k;

    cout << "Reading " << graph_filename << " ...\n";
    Graph g;
    g.loadFromFile(graph_filename);

    cout << "Preparing Invitees List" << "\n";   
    vector<int> invitees;
    g.socialgathering(invitees, k);
    sort(invitees.begin(), invitees.end());
    for (int i = 0; i < invitees.size(); i++) {
        outfile << invitees[i] << "\n";
    }

    if (!infile.eof()) {
        return 1;
    }

    infile.close();
    outfile.close();
    return 0;
}

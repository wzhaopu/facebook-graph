/*
 * CSE100 Project 3 - Graph.cpp
 * Author: Zhaopu Wang and Yanxun Li
 * Date: 03/04/2019
 * This is the definition of Graph class
 */

#include "Graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

/*
 * Graph class constructor
 */
Graph::Graph(void)
    : nodes(0) {}

/*
 * Graph class destructor
 */
Graph::~Graph(void) {
    for (auto itr : nodes) {
        delete itr.second;
    }
}

/*
 * Add a node to the graph representing person with idNumber and add
 * a connection between two nodes in the graph.
 * Param: id of the person to be added, id of one friend
 * Return: -
 */ 
void Graph::addNode(int id, int friendId) {
    auto itr = nodes.find(id);
    
    if (itr == nodes.end()) {
        Node* newFriend = new Node(id);
        newFriend->addAdj(friendId);
        pair<int, Node*> newNode(id, newFriend);
        nodes.insert (newNode);
    }
    else {
        itr->second->addAdj(friendId);
    }
}  

/*
 * Get the node with given id
 * Param: id of the node
 * Return: pointer to the node
 */
Node* Graph::getNode(int index) {
    if (nodes.find(index) == nodes.end())
        return nullptr;
    return nodes[index];
}

/*
 * Read in relationships from an inputfile to create a graph
 * Param: the input filename
 * Return: true if successed, false if failed
 */
bool Graph::loadFromFile(const char* in_filename) {
    ifstream infile(in_filename);

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ' ')) break;
            record.push_back(s);
        }

        if (record.size() != 2) {
            continue;
        }

        addNode(stoi(record[0]), stoi(record[1]));
        addNode(stoi(record[1]), stoi(record[0]));
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }

    infile.close();
    return true;
}

/*
 * Find the shortest path between to nodes using BFS.
 * Param: the starting node and ending node
 * Return: true if successed, false if invalid input or failed to find
 */
bool Graph::pathfinder(Node* from, Node* to) {
    if (!from || !to)
        return false;

    queue<int> q;
    for (auto itr : nodes) {
        itr.second->dist = INF_DIST;
        itr.second->prev = -1;
    }
    
    q.push(from->index);
    from->dist = 0;
    
    Node* curr = from;
    while (!q.empty()) {
        curr = nodes[q.front()];
        q.pop();

        // the shortest path found
        if (curr->index == to->index)
            return true;
        
        for (auto neighbor : curr->adj) {
            if (nodes[neighbor]->dist == INF_DIST) {
                nodes[neighbor]->dist = curr->dist + 1;
                nodes[neighbor]->prev = curr->index;
                q.push(neighbor);
            }   
        }   
    }

    return false;
}

/*
 * Find a list of invitees who will have at least k friends in a party.
 * We create a unordered_map where key is degree and value is a list
 * of nodes (represented by id) of the degree. For every neighbor of nodes
 * of degree less than k, decrease their degree by 1 and update lists.
 * After we traverse all nodes in less-than-k lists, the nodes in
 * greater-than-k list are guaranteed in core k.
 * Param: a vector of invitees
 *        and number of friends needed to have at least in a party
 * Return: -
 */
void Graph::socialgathering(vector<int>& invitees, const int& k) {
    unordered_map<int, vector<int>> cores;

    // initialize
    for (auto nodePair : nodes) {
        nodePair.second->degree = (nodePair.second->adj).size();
        if (cores.find(nodePair.second->degree) == cores.end()) {
            vector<int> v;
            pair<int,vector<int>> newList(nodePair.second->degree, v);
            cores.insert(newList);
        }
        cores[nodePair.second->degree].push_back(nodePair.first);
    }

    // find nodes in core k (method is described above)
    for (int degree = 0; degree < k; degree++) {
        if (cores.find(degree) == cores.end()) {
            continue;
        }
        for (int i = 0; i < cores[degree].size(); i++) {
            Node* node = getNode(cores[degree][i]);
            for (auto neighborId : node->adj) {
                Node* neighbor = getNode(neighborId);
                if (neighbor->degree > node->degree) {
                    for (auto itr = cores[neighbor->degree].begin();
                         itr != cores[neighbor->degree].end();
                         itr++) {
                        if (*itr == neighborId) {
                            cores[neighbor->degree].erase(itr);
                            break;
                        }
                    }
                    neighbor->degree--;
                    if (cores.find(neighbor->degree) == cores.end()) {
                        vector<int> v;
                        pair<int,vector<int>> newList(neighbor->degree, v);
                        cores.insert(newList);
                    }
                    cores[neighbor->degree].push_back(neighborId);
                }
            }
        }
    }

    // fill in the invitee list
    for (auto nodePair : nodes) {
        if (nodePair.second->degree >= k) {
            invitees.push_back(nodePair.second->index);
        }
    }
}



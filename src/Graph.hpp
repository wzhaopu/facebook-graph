/*
 * CSE100 Project 3 - Graph.hpp
 * Author: Zhaopu Wang and Yanxun Li
 * Date: 03/04/2019
 * This file is the header file of Graph class, including struct Node
 */

#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <unordered_map>

#define INF_DIST INT_MAX // infinity distance

using namespace std;
struct Node{
    int index;
    vector<int> adj;    // adjacency list
    int prev;
    int dist;
    int degree;

    /*
     * Node class constuctor
     */
    Node(int id, int prev = -1, int dist = INF_DIST)
       : index(id), prev(prev), dist(prev),degree(0) {}

    /*
     * Add one id to the adjacent list of the node
     * Param: friend id to be add
     * Return: -
     */
    void addAdj(int friendId) {
        adj.push_back(friendId);  
    }
};

class Graph {
protected:
    unordered_map<int, Node*> nodes;

public:
    /*
     * Graph class constructor
     */
    Graph(void);

    /*
     * Graph class destructor
     */
    ~Graph(void);
	
    /*
     * Add a node to the graph representing person with idNumber and add
     * a connection between two nodes in the graph.
     * Param: id of the person to be added, id of one friend
     * Return: -
     */
    void addNode(int id, int friendId);

    /*
     * Get the node with given id
     * Param: id of the node
     * Return: pointer to the node
     */
    Node* getNode(int index);

    /*
     * Read in relationships from an inputfile to create a graph.
     * Param: the input filename
     * Return: true if successed, false if failed
     */ 
    bool loadFromFile(const char* in_filename);
    
    /*
     * Find the shortest path between to nodes.
     * Param: the starting node and ending node
     * Return: true if successed, false if invalid input or failed to find
     */
    bool pathfinder(Node* from, Node* to);

    /*
     * Find a list of invitees who will have at least k friends in a party.
     * Param: a vector of invitees
     *        and number of friends needed to have at least in a party
     * Return: -
     */
    void socialgathering(vector<int>& invitees, const int& k);
};

#endif  // GRAPH_HPP

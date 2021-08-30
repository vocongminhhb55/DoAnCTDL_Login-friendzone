#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#if defined(_WIN32)
#define clearscreen "cls"
#elif defined(__MACH__)
#define clearscreen "clear"
#endif

// This class represents a directed graph using
// adjacency list representation
class Graph
{
    int V; // No. of vertices

    // Pointer to an array containing adjacency
    // lists
    list<int> *adj;
public:
    Graph(int V); // Constructor

    // function to add an edge to graph
    void addEdge(int v, int w);

    // prints BFS traversal from a given source s
    void BFS(int s);
    
    void displayAdjList();
    
    list<int> friendBFS(int s);
    
    void recommendFriend(int s);
    
    void saveFile(string name);
    
    bool loadFile(string name);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
    adj[w].push_back(v);
}

void Graph::BFS(int s)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
        visited[i] = false;

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    // 'i' will be used to get all adjacent
    // vertices of a vertex
    list<int>::iterator i;

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        cout << s << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }
}

void Graph::saveFile(string name)
{
    ofstream ofs(name);
    
    ofs << V << endl;
    
    for (int i = 0; i < V; i++) {
        ofs << i;
        for (auto x : adj[i])
            ofs << " --> " << x;
        ofs << endl;
    }
    
    ofs.close();
}

bool Graph::loadFile(string name)
{
    ifstream ifs(name);
    if(ifs.is_open())
    {
        if(adj != NULL)
            delete [] adj;
        
        ifs >> V;
        ifs.ignore();
        
        adj = new list<int>[V];
        
        string s;
        int idx;
        int des;
        while (!ifs.eof()) {
            getline(ifs, s);
            stringstream ss(s);
            ss >> idx;
            while (!ss.eof()) {
                ss.ignore(5);
                ss >> des;
                adj[idx].push_back(des);
            }
        }
        
        ifs.close();
        return true;
    }
    else
        return false;
}

void Graph::recommendFriend(int s)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
        visited[i] = false;

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    // 'i' will be used to get all adjacent
    // vertices of a vertex
    list<int>::iterator i;
    list<int> friends;
    int oneTimeUse = 1;
    
    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
        
        if (oneTimeUse) {
            oneTimeUse--;
            friends = queue;
        }
        
        friends.remove(s);
        if (friends.empty()) {
            //print out the recommend friends.
            for (auto x : queue) {
                cout << x << ' ';
            }
            return;
        }
    }
}

void Graph::displayAdjList()
{
    for (int i = 0; i < V; i++) {
        cout << i;
        for (auto x : adj[i])
            cout << " --> " << x;
        cout << endl;
    }
}
// Driver program to test methods of graph class

void printMenu() {
    cout << "Welcome to find friends program!" << endl;
    cout << "1. Find friends" << endl;
    cout << "2. Load friends relationship" << endl;
    cout << "3. Save friend relationship" << endl;
    cout << "4. Print friends relationship" << endl;
    cout << "5. Add relationship" << endl;
    cout << "6. Clear screen" << endl;
    cout << "0. Exit" << endl;
    cout << "-----------------------------" << endl;
    cout << "Your choice:";
}

void menu()
{
    // Create a graph given in the above diagram
    int vNumb = 9;
    Graph g(vNumb);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 7);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 6);
    g.addEdge(5, 6);
    g.addEdge(6, 8);
    string fileNameIn = "input";
    string fileNameOut = "output";
    
    int find;
    int fr1, fr2;
    while (true)
    {
        int opt;
        printMenu();
        cin >> opt;
        cin.ignore();

        switch (opt)
        {
        case 1:
                cout << "Kimi no Na wa (enter number smaller than " << vNumb << " only): ";
                cin >> find;
                cin.ignore();
                g.recommendFriend(find);
                break;
        case 2:
                g.loadFile(fileNameIn);
                break;
        case 3:
                g.saveFile(fileNameOut);
                break;
        case 4:
                g.displayAdjList();
                break;
        case 5:
                cout << "Enter 2 node: ";
                cin >> fr1 >> fr2;
                cin.ignore(2);
                g.addEdge(fr1, fr2);
            break;
        case 6:
                //clrscr
                break;
        case 0:
                exit(0);
                break;
        default:
                break;
        }
        cout << "Press enter to continue!";
        while ((getchar()) != '\n');
        if(opt == 6)
            system(clearscreen);
    }
}

int main(int argc, const char * argv[])
{
    menu();
    return 0;
}

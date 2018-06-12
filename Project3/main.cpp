/******************************
 * Author: Alperen Kantarcı   *
 * Id : 150140140             *
 *                            *
 *****************************/

 /*****************************************************
  *  IMPORTANT COMPILING NOTE:                        *
  * In order to compile the program use this command: *
  * g++ -std=c++11 main.cpp -o thomassShops           *
  *****************************************************/
  
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <climits>
#include <stack>

using namespace std;
const int shopLimit = 1000; //Input size
int adjMatrix[shopLimit][shopLimit]={0}; //Original graph
int transformedMat[shopLimit*2][shopLimit*2]={0}; // Splitted graph with directed
int path[shopLimit]; // stores parent of each node
int maxShop=0,numberOfRoad;
int visitedGlobal[2*shopLimit];

/*
 * Depth First Search is used for path finding algorithm.
 * This function also manipulates path array for giving the
 * path that it found.
 */

int DFS(int source,int dest){
    int visited[shopLimit] = {0};
    stack<int> s;

    path[source] = -1;
    visited[source] = 1;
    s.push(source);

    while(!s.empty()){
        int e = s.top();
        s.pop();

        for (int i = 0; i < 2*maxShop; i++) {
            if(visited[i]!=1 && transformedMat[e][i]>0){
                path[i] =e;
                visited[i] = 1;
                s.push(i);
            }
        }
    }

    for (int i = 0; i < 2*maxShop; ++i) { // Save the visited nodes in global array
        visitedGlobal[i] = visited[i];
    }

    return visited[dest]; // Return 1 if visited. Return 0 if not visited (no path)
}

/*
 * Inputs (s,t) should be doubled since i use two nodes (i_input,i_output)
 * so for example node 3 in original graph has two nodes 6 and 7. 6 for input 7 for output.
 * These two nodes has exactly one edge between them. If this edge reversed than that means that node should removed.
 * This algorithm based on the course slides. I reverse the edges if flow goes from that edge.
 */
int fordFulkerson(int s, int t)
{
    int i=0, j=0;
    s*=2;
    t*=2;

    int max_flow = 0;
    while (DFS(s, t))
    {
        int disjoint_flow = INT_MAX;
        for (j=t; j!=s; j=path[j]) {
            i = path[j];
            //path_flow = min(path_flow, transformedMat[u][v]);
            if(transformedMat[i][j] < disjoint_flow){
                disjoint_flow = transformedMat[i][j];
            }
        }

        for (j=t; j != s; j=path[j]) {
            i = path[j];
            transformedMat[i][j] -= disjoint_flow; // Reverse the edge
            transformedMat[j][i] += disjoint_flow; // Add flow to the edge
        }
        max_flow += disjoint_flow;
    }

    return max_flow;
}

/*
 * This function splits the nodes that are not sink or source to
 * two nodes which are i_input and i_output . i_input nodes are accepting
 * only coming edges and there is only one edge that directs to out which is
 * internal arc to the i_output. All internal edges has 1 capacity.
 * Other edges have capacity of number of nodes. For example if there are 5 nodes
 * then edge capacities will be 5.
 * Sink and sources are not splitted so their sink_output fields will remain 0.
 */

void transformAdjMatrix(int source,int sink){
    //First clean all global arrays
    for (int i = 0; i < shopLimit; i++) {
        for (int j = 0; j < shopLimit; j++) {
            transformedMat[i][j] = 0;
        }
        path[i] =0;
    }


    for(int i=0 ; i<maxShop ;i++){
        for(int j=0; j < maxShop;j++){
            if( i== source || i == sink ){
                if(i==j)     //There is no self edges
                    continue;

                if(adjMatrix[i][j] == 1)
                    transformedMat[2*i][2*j] = maxShop;

            }else{
                if(i==j)
                    transformedMat[2*i][2*i+1] = 1;

                if(adjMatrix[i][j] == 1 ){
                    transformedMat[2*i+1][2*j] =maxShop;
                }
            }
        }
    }
    return;
}

int main(int argc, char const *argv[]) {
    if(argc > 2 ){
        cerr << "You have given more parameters than expected.";
        return 0;
    }

    string textfile = argv[1];
    ifstream input(textfile);


    input >> maxShop >> numberOfRoad ;

    //Read the graphs
    for(int i=0 ; i < numberOfRoad ;i++){
        int x,y;
        input >> x >> y ;
        x-=1; y-=1;
        adjMatrix[x][y] = 1;
        adjMatrix[y][x] = 1;
    }

    int MINIMUM = 0;
    // Calculate for every pair possible
    for(int i=0 ; i< maxShop;i++){
        for (int j = 0; j < maxShop ; j++) {
            if(i==j)
                continue;

            transformAdjMatrix(i,j); //Transform original graph into my implementation
            int flow = fordFulkerson(i,j); //Get max flow

            for (int k = 0; k <shopLimit ; ++k) { //Clean graphs
                path[k] = 0;
                visitedGlobal[k] =0;
            }
            DFS(i,j); //Latest residual graph will be tested for connectivity

            int count =1;
            for(int k=0 ; k< 2*maxShop;k++){ //Count the nodes of two sides of the disconnected graphs
                if(k==i)
                    continue;
                if(visitedGlobal[k] == 1)
                    count++;
            }
            count /= 2;
            //Calculate minimum number of nodes to remove among all pairs
            if(count >=2 && (maxShop-count-flow) >=2){
                if( MINIMUM == 0 )
                    MINIMUM = flow;
                else if(flow < MINIMUM)
                    MINIMUM = flow;
            }
        }
    }
    cout << MINIMUM;

    return 0;
}
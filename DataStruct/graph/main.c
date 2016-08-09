#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[])
{
    Graph* graph = Graph_Create(5);
    
    Graph_AddEdge(graph, 0, 1, 1);
    Graph_AddEdge(graph, 1, 0, 1);
    
    Graph_Destroy(graph);
    
	return 0;
}

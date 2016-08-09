
#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef void Graph;
typedef void Vertex;

/* 创建并返回有n个顶点的图 */
Graph* Graph_Create(int n);

/* 销毁graph所指向的图 */
void Graph_Destroy(Graph* graph);

/* 将graph所指图的边集合清空 */
void Graph_Clear(Graph* graph);

/* 在graph所指图中的v1和v2之间加上边，且边的权为w */
int Graph_AddEdge(Graph* graph, int v1, int v2, int w);

/* 将graph所指图中v1和v2之间的边删除，返回权值 */
int Graph_RemoveEdge(Graph* graph, int v1, int v2);

/* 将graph所指图中v1和v2之间的边的权值返回 */
int Graph_GetEdge(Graph* graph, int v1, int v2);

/* 将graph所指图中v顶点的度数 */
int Graph_TD(Graph* graph, int v);

/* 将graph所指图中的顶点数返回 */
int Graph_VertexCount(Graph* graph);

/* 将graph所指图中的边数返回 */
int Graph_EdgeCount(Graph* graph);

#endif

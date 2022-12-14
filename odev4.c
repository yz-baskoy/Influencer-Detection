#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Person {
	int numNodes;
	bool **edges;
} graph;

/*Function prototypes*/
graph *createGraph(int numNodes);
void destroyGraph(graph* g);
void printGraph(graph *g);
bool addEdge(graph *g, int fromNode, int toNode);
bool hasEdge(graph *g, int fromNode, int toNode);

int main() {
	return 0;
}
graph *createGraph(int numNodes) {
	graph *g = malloc(sizeof(*g));
	if (g == NULL) {
		return NULL;	
	}
	g->numNodes = numNodes;
	// allocte matrix
	g->edges = calloc(sizeof(bool*), g->numNodes);
	if (g->edges == NULL) {
		free(g);
		return NULL;
	}

	for(int i=0;i < g->numNodes;i++) {
		g->edges[i] = calloc(sizeof(bool), g->numNodes);
		if (g->edges[i] == NULL) {
			destroyGraph(g);
			return NULL;
		}
	}
	return g;
}

void destroyGraph(graph* g){
	int i;
	if (g->edges == NULL) {
		return;
	}

	for (i=0; i < g->numNodes; i++) {
		if (g->edges[i] != NULL) {
			free(g->edges[i]);	
		}
	}
	free(g->edges);
	free(g);
}

void printGraph(graph *g) {
	int from, to;
	printf("graph {\n");

	for (from=0 ; from < g->numNodes; from++) {
		for (to=0; to < g->numNodes; to++) {
			if (g->edges[from][to]) {
				printf("%d --> %d;\n", from, to);
			}
		}
	}
	printf("*}\n");
}
bool addEdge(graph *g, int fromNode, int toNode) {
	assert(g != NULL);
	assert(fromNode < g->numNodes);
    assert(toNode < g->numNodes);
	if (hasEdge(g, fromNode, toNode)) {
		return false;
	}
	
	g->edges[fromNode][toNode] = true;
	return true;
}
bool hasEdge(graph *g, int fromNode, int toNode) {
	assert(g != NULL);
	assert(fromNode < g->numNodes);
	assert(toNode < g->numNodes);
		
	return g->edges[fromNode][toNode];
}
















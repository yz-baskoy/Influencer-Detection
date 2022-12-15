#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define MAX 100

typedef struct Graph {
	int numNodes;
	bool **edges;
} graph;

typedef struct Person {
	char* name;
	char* surname;
} person;

/*Function prototypes*/
graph *createGraph(int numNodes);
void destroyGraph(graph* g);
void printGraph(graph *g);
bool addEdge(graph *g, int fromNode, int toNode);
bool hasEdge(graph *g, int fromNode, int toNode);
void removeNodesByinDegree(graph *g, int m); 
int* inDegrees(graph* G);
int inDegree(graph* G, int u);
int totalConnections(graph * g, int i);
void printInfluencers(graph *g, int x, int y);

int main() {
	graph *g1 = createGraph(13);
	addEdge(g1, 1, 2);
	addEdge(g1, 1, 3);
	addEdge(g1, 1, 5);

	addEdge(g1, 2, 1);
	addEdge(g1, 2, 3);
	
	addEdge(g1, 3, 1);
	addEdge(g1, 3, 2);

	addEdge(g1, 4, 6);
	
	addEdge(g1, 5, 6);
	addEdge(g1, 5, 7);
	
	addEdge(g1, 6, 8);
	
	addEdge(g1, 7, 8);
	
	addEdge(g1, 8, 11);
	addEdge(g1, 8, 10);
	addEdge(g1, 8, 9);
	
	addEdge(g1, 9, 4);
	
	addEdge(g1, 10, 8);
	addEdge(g1, 10, 12);
	
	addEdge(g1, 11, 12);
	
	addEdge(g1, 12, 8);
	addEdge(g1, 12, 10);
	addEdge(g1, 12, 11);

	printGraph(g1);
	
	int* degrees = inDegrees(g1);

    // print the in-degree of each vertex
    for (int i = 1; i < g1->numNodes; i++) {
        printf("Vertex %d has in-degree %d\n", i, degrees[i]);
    }
	
	removeNodesByinDegree(g1, 2);

	printf("***************\n");

	printGraph(g1);
	
	for (int i = 1; i < g1->numNodes; i++) {
		if (totalConnections(g1, i) == 0)
			printf("Vertex %d has %d total connections (DELETED)\n", i, totalConnections(g1, i));
		else
			printf("Vertex %d has %d total connections\n", i, totalConnections(g1, i));
	}

	printInfluencers(g1, 2, 3);

	destroyGraph(g1);
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

// return the in-degree of a given vertex
int inDegree(graph* G, int u) {
    int degree = 0;
    for (int i = 0; i < G->numNodes; i++) {
        if (G->edges[i][u] == 1) {
            degree++;
        }
    }
    return degree;
}

// return an array of the in-degree of each vertex
int* inDegrees(graph* g) {
    int* degrees = malloc(sizeof(int)*g->numNodes);
    for (int i = 0; i < g->numNodes; i++) {
        degrees[i] = inDegree(g, i);
    }
    return degrees;
}


// Function to remove a node from a graph
void remove_node(graph *g, int u) {
	// Set all elements in the row and column of the adjacency matrix for vertex u to 0
	int i;
	for (i = 0; i < g->numNodes; i++) {
		g->edges[u][i] = false;
		g->edges[i][u] = false;
	}
}

// Function to remove nodes from a graph based on their in-degree
void removeNodesByinDegree(graph *g, int m) {
	int i;
	for (int u = 0; u < g->numNodes; u++) {
		int in_degree = 0;

		for (i = 0; i < g->numNodes; i++) {
		if (g->edges[i][u] == true) {
			in_degree++;
		}
		}
		if (in_degree < m) {
		remove_node(g, u);
		}
	}
}

/*breadth first search to calculate the total number of direct and indirect connections*/
int totalConnections(graph * g, int i) {
	int j, connections = 0;
	int queue[MAX], front = 0, rear = 0;
	int visited[MAX];

	if (i >= g -> numNodes) {
		printf("Error: NODE does not exist\n");
		return -1;
	}

	for (j = 0; j < g -> numNodes; j++) {
		visited[j] = 0;
	}

	queue[rear] = i;
	rear++;
	visited[i] = 1;

	while (front != rear) {
		int k = queue[front];
		front++;

		for (j = 0; j < g -> numNodes; j++) {
		if (g -> edges[k][j] == 1 && visited[j] == 0) {
			queue[rear] = j;
			rear++;
			visited[j] = 1;
			connections++;
		}
		}
	}

	return connections;
}

void printInfluencers(graph * g, int x, int y) {
    int i;

	printf("Influencer nodes:\n");

	for (i = 0; i < g -> numNodes; i++) {
		if (inDegree(g, i) >= x && totalConnections(g, i) >= y) {
		printf("Node %d: in-degree %d, total connections %d\n", i, inDegree(g, i), totalConnections(g, i));
		}
	}
}


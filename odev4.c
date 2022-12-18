#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX 100
#define SIZE 4;
#define MAX_LINE_LEN 1024
#define MAX_NAME_LEN 128
#define MAX_NODES 100

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    int* connections;
    int num_connections;
} Person;

typedef struct Graph {
	Person* persons;
	int numNodes;
	bool **edges;
} graph;

/*Function prototypes*/
graph *createGraph(int numNodes);
void destroyGraph(graph* g);
void printGraph(graph *g);
bool addEdge(graph *g, int fromNode, int toNode);
bool hasEdge(graph *g, int fromNode, int toNode);
void removeNodesByinDegree(graph *g, int m); 
int* inDegrees(graph* G);
int inDegree(graph* G, int u);
void printInfluencers(graph *g, int x, int y);
graph* read_data(const char* filename);
int calculate_indegree_connections(graph* graph, int start, int end);
int calculate_indegree_connectionsBFS(graph* graph, int node);

int main() {
	//graph *g1 = createGraph(13);
	
	FILE *file = fopen("socialNET.txt", "r");
	const char* filename = "socialNET.txt";

	//graph* g1 = read_data(filename);
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
        printf("Node %d has in-degree %d\n", i, degrees[i]);
    }
	
	removeNodesByinDegree(g1, 1);

	printf("***************\n");

	printGraph(g1);
	
	for (int i = 1; i < g1->numNodes; i++) {
		if (calculate_indegree_connectionsBFS(g1, i) == 0)
			printf("Node %d has %d total connections (DELETED)\n", i, calculate_indegree_connectionsBFS(g1, i));
		else
			printf("Node %d has %d total connections\n", i, calculate_indegree_connectionsBFS(g1, i));
	}
	printInfluencers(g1, 2, 4);
	destroyGraph(g1);
	fclose(file);
	return 0;
}

graph *createGraph(int numNodes) {
	graph *g = malloc(sizeof(*g));
    g->persons = malloc(numNodes * sizeof(Person));

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


void printInfluencers(graph * g, int x, int y) {
    int i;

	printf("Influencer nodes:\n");

	for (i = 0; i < g -> numNodes; i++) {
		if (inDegree(g, i) >= x && calculate_indegree_connectionsBFS(g, i) >= y) {
		printf("Node %d: in-degree %d, total connections %d\n", i, inDegree(g, i), calculate_indegree_connectionsBFS(g, i));
		}
	}
}

Person* parse_data(const char* data, int* num_people) {
    // Split the data into lines
    char* lines[MAX_LINE_LEN];
    int num_lines = 0;
    char* line = strtok((char*)data, "\n");
    while (line) {
        lines[num_lines++] = line;
        line = strtok(NULL, "\n");
    }

    // Allocate an array for the Person objects
    Person* people = malloc(sizeof(Person) * num_lines);
    *num_people = num_lines;

    // Parse each line and create a Person object
    for (int i = 0; i < num_lines; i++) {
        char* parts[MAX_NAME_LEN];
        int num_parts = 0;
        char* part = strtok(lines[i], ",");
        while (part) {
            parts[num_parts++] = part;
            part = strtok(NULL, ",");
        }
        people[i].id = atoi(parts[0]);
        strcpy(people[i].name, parts[1]);
        strcpy(people[i].last_name, parts[2]);
        people[i].num_connections = num_parts - 3;
        people[i].connections = malloc(sizeof(int) * people[i].num_connections);
        for (int j = 0; j < people[i].num_connections; j++) {
            people[i].connections[j] = atoi(parts[j + 3]);
        }
    }

    return people;
}

void free_people(Person* people, int num_people) {
    for (int i = 0; i < num_people; i++) {
        free(people[i].connections);
    }
    free(people);
}

/*breadth first search to calculate the total number of direct and indirect connections*/
int calculate_indegree_connections(graph* graph, int start, int end) {
  // Initialize a queue for BFS
  int queue[MAX_NODES];
  int head = 0;
  int tail = 0;
  
  // Initialize a visited array to keep track of visited nodes
  bool visited[MAX_NODES];
  for (int i = 0; i < graph->numNodes; i++) {
    visited[i] = false;
  }
  
  // Enqueue the start node and mark it as visited
  queue[tail] = start;
  tail++;
  visited[start] = true;
  
  // Initialize the number of connections to 0
  int connections = 0;
  
  // Perform BFS
  while (head < tail) {
    // Dequeue the next node
    int current = queue[head];
    head++;
    
    // Check if the current node is the end node
    if (current == end) {
      connections++;
    }
    
    // Enqueue the unvisited neighbors of the current node
    for (int i = 0; i < graph->numNodes; i++) {
      if (graph->edges[current][i] == 1 && !visited[i]) {
        queue[tail] = i;
        tail++;
        visited[i] = true;
      }
    }
  }
  
  // Return the number of connections
  return connections;
}

int calculate_indegree_connectionsBFS(graph* graph, int node) {
  // Initialize the number of connections to 0
  int connections = 0;
  
  // Iterate through the graph and count the number of connections
  // to the given node using BFS
  for (int i = 0; i < graph->numNodes; i++) {
    connections += calculate_indegree_connections(graph, i, node);
  }
  
  return connections;
}
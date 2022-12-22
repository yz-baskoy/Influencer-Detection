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
int inDegree(graph* G, int u);
int* inDegrees(graph* G);
void removeNodesByinDegree(graph *g, int m); 
int calculateTotalIndegreeConnections(graph* graph, int start, int end);
int calculateTotalConnectionsBFS(graph* graph, int node);
void freePeople(Person* people, int num_people);
Person* parseData(const char* data, int* num_people);
void printPersonNameById(int id, Person *people, int numPeople); 
void printInfluencers(graph *g, int x, int y, Person * p);

int main() {
	FILE *fp = fopen("socialNET.txt", "r");
	   if (!fp) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    long data_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* data = malloc(data_len + 1);
    fread(data, 1, data_len, fp);
    fclose(fp);
    data[data_len] = '\0';

    // Parse the data and create the list of Person objects
    int num_people, X, Y, M;
    Person* people = parseData(data, &num_people);
    free(data);

    printf("Number of people: %d\n", num_people);
	graph *g1 = createGraph(num_people + 1);

    // Print the list of people and create edges
    for (int i = 0; i < num_people; i++) {
        Person person = people[i];
        printf("indis: %d, ID: %d, Name: %s %s, Connections:", i, person.id, person.name, person.last_name);
        for (int j = 0; j < person.num_connections; j++) {
            printf("%d %d",person.id, person.connections[j]);
			addEdge(g1, person.id, person.connections[j]);
        }
        printf("\n");
    }
	printGraph(g1);

    printf("M degerini giriniz:\n");
    scanf("%d", &M);
    printf("X degerini giriniz:\n");
    scanf("%d", &X);
    printf("Y degerini giriniz:\n");
    scanf("%d", &Y);

	printf("************************************************\n");
    printf("* Menu *\n");
    printf("1. NORMAL MODE \n");
    printf("2. DETAIL MODE \n");
    printf("Choose Your Side!:\n");
	printf("************************************************\n");

	int deger;
	scanf("%d", &deger);

	int* degrees = inDegrees(g1);

	removeNodesByinDegree(g1, M);

	int* degreesAfterDeleted = inDegrees(g1);

    printGraph(g1);

    switch (deger)
    {
    case 1:
        printInfluencers(g1, X, Y, people);
        break;
    case 2:
        printf("BASLANGIC DURUMUNDA INDEGREE DEGERLERI\n");
        for (int i = 1; i < g1->numNodes; i++)
        {
            printf("Node %d has in-degree %d\n", i, degrees[i]);
        }

        printf("ELENDIKTEN SONRA INDEGREE DEGERLERI\n");
        // print the in-degree of each vertex
        for (int i = 1; i < g1->numNodes; i++)
        {
            printPersonNameById(people[i-1].id, people, g1->numNodes);
            printf("Node %d in-degree %d\n", i, degreesAfterDeleted[i]);
        }

        for (int i = 1; i < g1->numNodes; i++)
        {
            if (calculateTotalConnectionsBFS(g1, i) == 0)
                printf("Node %d has %d total connections (DELETED)\n", i, calculateTotalConnectionsBFS(g1, i));
            else
                printf("Node %d has %d total connections\n", i, calculateTotalConnectionsBFS(g1, i));
        }
        printInfluencers(g1, X, Y, people);
        break;
    default:
        break;
    }

    destroyGraph(g1);
	freePeople(people, num_people);
	return 0;
}

/**
 * @params The number of nodes in the graph
 * @returns graph*
 * @desc Creates a graph with the given number of nodes.
 */
graph *createGraph(int numNodes) {
	graph *g = malloc(sizeof(*g));
    g->persons = malloc(numNodes * sizeof(Person));

	if (g == NULL) {
		return NULL;	
	}
	g->numNodes = numNodes;
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

/**
 * @params A pointer to the graph to be destroyed
 * @returns void
 * @desc Destroys the given graph, freeing all memory associated with it.
 */
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

/**
 * @params A pointer to the graph
 * @returns void
 * @desc Prints the given graph in the GraphViz DOT format.
 */
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

/**
 * @params A pointer to the graph, source, destination
 * @returns bool
 * @desc Adds an edge between two nodes in a graph.
 */
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

/**
 * @param A pointer to the graph. source, destination
 * @returns bool True if the edge exists, false otherwise.
 * @desc Checks if an edge exists between two nodes in a graph.
 */
bool hasEdge(graph *g, int fromNode, int toNode) {
	assert(g != NULL);
	assert(fromNode < g->numNodes);
	assert(toNode < g->numNodes);
		
	return g->edges[fromNode][toNode];
}

/**
 * @param pointer to the graph, index of the vertex.
 * @returns int The in-degree of the vertex.
 * @desc Returns the in-degree of a given vertex in a graph.
 */
int inDegree(graph* G, int u) {
    int degree = 0;
    for (int i = 0; i < G->numNodes; i++) {
        if (G->edges[i][u] == true) {
            degree++;
        }
    }
    return degree;
}

/**
 * @param A pointer to the graph.
 * @returns: in-degrees of the vertices.
 * @desc: Returns an array of the in-degrees of each vertex in a graph.
 */
int* inDegrees(graph* g) {
    int* degrees = malloc(sizeof(int)*g->numNodes);
    for (int i = 0; i < g->numNodes; i++) {
        degrees[i] = inDegree(g, i);
    }
    return degrees;
}

/**
 * @param pointer to the graph, index of the node to remove.
 * @returns void
 * @desc: Removes a node from a graph.
 */
void remove_node(graph *g, int u) {
	int i;
	for (i = 0; i < g->numNodes; i++) {
		g->edges[u][i] = false;
		g->edges[i][u] = false;
	}
}

/**
 * @params: graph* g - a pointer to a graph structure
 *          int m - the minimum in-degree for a node to be removed
 * @returns: void
 * @desc: Removes nodes from the given graph that have an in-degree less than m.
 */
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

/**
 * @params: graph* g - a pointer to a graph structure
 *          int x - the minimum in-degree for an influencer node
 *          int y - the minimum number of total connections for an influencer node
 *          Person* p - an array of Person structures
 * @returns: void
 * @desc: Prints the names and information about influencer nodes in the given graph.
 */
void printInfluencers(graph * g, int x, int y, Person * p) {
    int i;

	printf("Influencer nodes:\n");

	for (i = 0; i < g -> numNodes; i++) {
		if (inDegree(g, i) >= x && calculateTotalConnectionsBFS(g, i) >= y) {
            printPersonNameById(p[i-1].id, p, g -> numNodes);
		    printf("Node %d: in-degree: %d, total connections: %d\n", i, inDegree(g, i), calculateTotalConnectionsBFS(g, i));
		}
	}
}

/**
 * @params: const char* data - a string containing data to be parsed
 *          int* num_people - a pointer to an integer to store the number of people
 * @returns: Person* - a pointer to an array of Person structures
 * @desc: Parses a string containing data about people and their connections and
 *        returns an array of Person structures. The number of people is stored in
 *        the integer pointed to by num_people.
 */
Person* parseData(const char* data, int* num_people) {
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

/**
 * @params: Person* people - an array of Person structures
 *          int num_people - the number of people in the array
 * @returns: void
 * @desc: Frees the memory allocated for the connections array 
 */
void freePeople(Person* people, int num_people) {
    for (int i = 0; i < num_people; i++) {
        free(people[i].connections);
    }
    free(people);
}

/**
 * @params: graph* graph - a pointer to a graph structure
 *          int start - the starting node for the search
 *          int end - the ending node to search for
 * @returns: int - the number of connections from the start node to the end node
 * @desc: Calculates the number of connections from the start node to the end node
           using BFS.
 */

int calculateTotalIndegreeConnections(graph* graph, int start, int end) {
    int queue[MAX_NODES];
    int head = 0;
    int tail = 0;

    bool visited[MAX_NODES];
    for (int i = 0; i < graph->numNodes; i++) {
        visited[i] = false;
    }

    queue[tail] = start;
    tail++;
    visited[start] = true;

    int connections = 0;

    while (head < tail) {
        int current = queue[head];
        head++;

        if (current == end) {
            connections++;
        }

        for (int i = 0; i < graph->numNodes; i++) {
        if (graph->edges[current][i] == true && !visited[i]) {
            queue[tail] = i;
            tail++;
            visited[i] = true;
            }
        }
    }

    return connections;
}

/**
 * @params: graph* graph - a pointer to a graph structure
 *          int node - the node to count connections to
 * @returns: int - the total number of connections to the given node
 * @desc: Calculates the total number of connections to the given node in the graph
 *        using BFS.
 */
int calculateTotalConnectionsBFS(graph* graph, int node) {
  int connections = 0;
  
  // Iterate through the graph and count the number of connections 
  // to the given node using BFS
  for (int i = 0; i < graph->numNodes; i++) {
    // dont call bfs on self node 
    if (i == node)
    {
        connections += 0;
    }
    else
    {
        connections  += calculateTotalIndegreeConnections(graph, i, node);
    }
  }

  return connections;
}

/**
 * @params: int id - the ID of the person to be searched for
 *          Person* people - an array of Person structures
 *          int numPeople - the number of people in the array
 * @returns: void
 * @desc: Prints the name of the person with the given ID, if found
 */
void printPersonNameById(int id, Person *people, int numPeople) {
    for (int i = 0; i < numPeople; i++) {
        if (people[i].id == id) {
            printf("Name: %s,%13s\t", people[i].name, people[i].last_name);
            return;
        }
    }
    printf("No person with id %d was found.\n", id);
}

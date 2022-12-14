#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Person {
	int numNodes;
	bool **edges;
} person;

/*Function prototypes*/
person *createGraph(int numNodes);
void destroyGraph(person* p);
void printGraph(person *p);
bool addEdge(person *p, int fromNode, int toNode);
bool hasEdge(person *p, int fromNode, int toNode);

int main() {
	return 0;
}
person *createGraph(int numNodes) {
	person *p = malloc(sizeof(*p));
	if (p == NULL) {
		return NULL;	
	}
	p->numNodes = numNodes;
	// allocte matrix
	p->edges = calloc(sizeof(bool*), p->numNodes);
	if (p->edges == NULL) {
		free(p);
		return NULL;
	}

	for(int i=0;i < p->numNodes;i++) {
		p->edges[i] = calloc(sizeof(bool), p->numNodes);
		if (p->edges[i] == NULL) {
			destroyGraph(p);
			return NULL;
		}
	}
	return p;
}
void destroyGraph(person* p){

}
void printGraph(person *p);
bool addEdge(person *p, int fromNode, int toNode);
bool hasEdge(person *p, int fromNode, int toNode);

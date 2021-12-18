//  https://adventofcode.com/2021/day/12

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EDGE_COUNT 32
#define MAX_NODE_COUNT 32
#define EDGESTR_LEN 16
#define NODESTR_LEN 8

static int edges[MAX_EDGE_COUNT][2];            // node1; node2
static char nodes[MAX_NODE_COUNT][NODESTR_LEN]; // canon names
static int nodeIsBig[MAX_NODE_COUNT];           // 0 = small; 1 = big
static int startidx = 0;
static int endidx = 1;
static int pathCounter = 0;
static int eidx = -1; // highest edge index
static int nidx = 1;  // highest node index

static void pathfinder(int currentNode, int currentPath[MAX_NODE_COUNT],
                       int cpidx) {
  // add the current node to the path
  cpidx++;
  currentPath[cpidx] = currentNode;
  // if the end node is reached, valid path is found!
  if (currentNode == endidx) {
    pathCounter++;
    // print the current path
    for (int i = 0; i < cpidx; i++) {
      printf("%s -> ", nodes[currentPath[i]]);
    }
    printf("%s\n", nodes[currentPath[cpidx]]);
    return;
  }
  // try all nodes reachable from the current one
  // unless it's a small cave already included in the path
  for (int h = 0; h <= 1; h++) {
    for (int i = 0; i <= eidx; i++) {
      // search for the current node in the edges array
      if (edges[i][(h ? 0 : 1)] == currentNode) {
        // if the connected cave is big, we can always visit it
        if (nodeIsBig[edges[i][h]]) {
          pathfinder(edges[i][h], currentPath, cpidx);
        }
        // if the connected cave is small, we can only visit once
        else {
          // check if it is on the current path already
          int previouslyVisited = 0;
          for (int j = 0; j <= cpidx; j++) {
            if (currentPath[j] == edges[i][h]) {
              previouslyVisited = 1;
            }
          }
          if (!previouslyVisited) {
            // it's never been visited on this path - do it now
            pathfinder(edges[i][h], currentPath, cpidx);
          }
        }
      }
    }
  }
  // no more valid paths available from this node
  return;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    if (fprintf(stderr, "%s\n", "provide an input file") < 0) {
      perror("fprintf");
    }
    exit(EXIT_FAILURE);
  }

  FILE *fp;
  if ((fp = fopen(argv[1], "r")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char *edgeString; // e.g. "start - DX"
  edgeString = malloc(EDGESTR_LEN);
  if (edgeString == NULL) {
    perror("malloc");
  }

  // insert the start and end node
  strncpy(nodes[startidx], "start", 6);
  strncpy(nodes[endidx], "end", 4);

  // read the edges
  while (fgets(edgeString, EDGESTR_LEN, fp) != NULL) {
    // tokenize names
    char *nodestr0 = strtok(edgeString, " - ");
    char *nodestr1 = strtok(NULL, "\r\n");

    // map name string to index
    int nodeidx0 = -1;
    int nodeidx1 = -1;
    // search for the the node0 index in the array
    for (int i = 0; i <= nidx; ++i) {
      if (!strcmp(nodes[i], nodestr0)) {
        nodeidx0 = i;
        break;
      }
    }
    // if not found - insert node0
    if (nodeidx0 == -1) {
      nidx++;
      strncpy(nodes[nidx], nodestr0, NODESTR_LEN);
      nodeIsBig[nidx] = isupper(nodes[nidx][0]);
      nodeidx0 = nidx;
    }
    // repeat for node1
    for (int i = 0; i <= nidx; ++i) {
      if (!strcmp(nodes[i], nodestr1)) {
        nodeidx1 = i;
        break;
      }
    }
    if (nodeidx1 == -1) {
      nidx++;
      strncpy(nodes[nidx], nodestr1, NODESTR_LEN);
      nodeIsBig[nidx] = isupper(nodes[nidx][0]);
      nodeidx1 = nidx;
    }

    // insert edge into the edge array
    eidx++;
    edges[eidx][0] = nodeidx0;
    edges[eidx][1] = nodeidx1;
  }
  if (!feof(fp)) {
    perror("fgets");
    exit(EXIT_FAILURE);
  }
  if (fclose(fp)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  free(edgeString);

  // find all paths
  int path[MAX_NODE_COUNT * 4];
  pathfinder(startidx, path, -1);

  if (printf("\nThere are %i paths through this cave system that visit small "
             "caves at most once.\n",
             pathCounter) < 0) {
    perror("printf");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
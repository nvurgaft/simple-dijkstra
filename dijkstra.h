
#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

/** @description vertex struct, each vertex in the list will be assigned one.
    \param id: the id of the vertex.
    \param edges_list: a generic list of all the edges attached to this vertex. */
typedef struct vertex_struct {
    int id;
    dbllist_t* edges_list;
} vertex_t;

/** @description edge struct, each edge in the list will be assigned one.
    \param target: the target node/vertex this edge is pointing to.
    \param weight: the weight of the edge */
typedef struct edge_struct {
    dbllist_node_t* target;
    int weight;
} edge_t;

/** main vertice */
dbllist_t* vertex_list;

/** the weight of infinity is set default to 1000 */
const int INFINITY = 1000;

/** prints a visula representation of the graph */
void print_graph(dbllist_t*);

/** adds a vertex (without any edges) to the graph */
void add_vertex(char*);

/** reads the config file and allocates assets accordingly */
int read_file(char*);

/** adds an edge to a vertex */
void add_edge(char*);

/** removes an edge from a vertex */
void remove_edge(char*);

/** dijkstra algorithem */
void calculate_dijkstra(char*);

/** destructs existing dynamic assets and exits the pogram */
void exit_prog();

/** checks to see if a node id is present in a list
    \return list node is found, or NULL if not. */
dbllist_node_t* check_inlist(dbllist_t* list , int e);

#endif

/**
*
*   written by: koby vurgaft, 303829063
*   for the "computer communication applications" course at JCE
*   finished current at 19.11.2012, 8PM
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "dbllist.c"
#include "dijkstra.h"

/** counters of graph assets */
int num_of_edges;
int num_of_vertices;

int main(int argc, char* argv[])
{
    vertex_list = (dbllist_t*) malloc(sizeof(dbllist_t));
    read_file("test_config");

    return 0;
}

int read_file(char* filename)
{
    FILE* conf_file = fopen(filename, "r");
    if (conf_file==NULL)
    {
        fprintf(stderr, "%s not found!\n", filename);
        return -1;
    }
    printf("%s is a valid config file\n", filename);

    char line[128];
    dbllist_init(vertex_list);

    while(fgets(line, sizeof line, conf_file)!=NULL)
    {
        char* ch = (char*) &line;
        //parse line only if the first char is a legal value between 0-4 !
        if (*ch>=48 && *ch<=52)
        {
            switch (*ch) {
            case 48: //0 add vertex
                add_vertex(ch+2);
                break;
            case 49: //1 add edge
                add_edge(ch+2);
                break;
            case 50: //2 remove edge
                remove_edge(ch+2);
                break;
            case 51: //3 calculate dijkstra
                calculate_dijkstra(ch+2);
                break;
            case 52: //4 exit
                exit_prog();
                break;
            }
        }
    }
    fclose(conf_file);
    return 0;
}

void add_vertex(char* line) //0
{
    printf("adding a vertex %s ", line);
    int n = atoi(line);

    /** traverse the vertex list to see if we already don't have an existing
        copy of this vertex, if we do, we'll exit this function with an error */
    dbllist_node_t* current = vertex_list->head;
    while (current!=NULL)
    {
        if (((vertex_t*) current->data)->id == n)
        {
            fprintf(stderr, "Cannot add node (%d)\n", n);
            return;
        }
        current = current->next;
    }

    vertex_t* ver = (vertex_t*) malloc(sizeof(vertex_t));
    ver->id = n;
    if (dbllist_prepend(vertex_list, ver)==0) // !!!!
    {
        ver->edges_list = (dbllist_t*) malloc(sizeof(dbllist_t));
        dbllist_init(ver->edges_list);
        num_of_vertices++;
    }
}

void add_edge(char* line) //1
{
    printf("adding an edge %s", line);
    int src, dest, wt=0, i=0;

	src = atoi(line);
	dest = atoi(line+=2);
	wt = atoi(line+=2);

	/**making sure the line has it's values right */
	if (wt<0 || wt>100 || dest>=num_of_vertices || dest<0 || src>=num_of_vertices)
	{
	    fprintf(stderr, "Cannot add edge(%d, %d)\n", src, dest);
	    return;
	}

	/**creating an instance of the argumented edge */
	edge_t* new_edge = (edge_t*) malloc(sizeof(edge_t));
	new_edge->weight = wt;

    /** searching for dest vertex */
	dbllist_node_t* current = vertex_list->head;
	while (current!=NULL)
	{
        if (((vertex_t*)current->data)->id==dest)
        {
            dbllist_node_t* dest_ver = current;
            new_edge->target = dest_ver;
            break;
        }
        current = current->next;
	}

	/**searching for the src vertex*/
	current = vertex_list->head;
	while (current!=NULL)
	{
        if (((vertex_t*)current->data)->id==src)
        {
            dbllist_append(((vertex_t*)current->data)->edges_list, new_edge);
            break;
        }
        current = current->next;
	}
    num_of_edges++;
}

void remove_edge(char* line) //2
{
    printf("removing an edge %s", line);
    int src, dest, i=0;

	src = atoi(line);
	dest = atoi(line+=2);

	if (dest>=num_of_vertices || dest<0 || src>=num_of_vertices)
	{
	    fprintf(stderr, "Cannot remove edge(%d, %d)\n", src, dest);
	    return;
	}

	dbllist_node_t* current = vertex_list->head;
	while (current!=NULL)
	{
        if (((vertex_t*)current->data)->id==src)
        {
            dbllist_node_t* inner = ((vertex_t*)current->data)->edges_list->head;
            while (inner!=NULL)
            {
                if (((vertex_t*)inner->data)->id==dest)
                {
                    dbllist_remove(((vertex_t*)current->data)->edges_list, inner, 1);
                    break;
                }
                inner = inner->next;
            }
        }
	    current = current->next;
	}
    num_of_edges--;
}

void calculate_dijkstra(char* line) //3
{
    int src = atoi(line);
    printf("making shortest-path calculations\n");

    if (vertex_list==NULL)
    {
        printf("can not compute dijkstra!\n");
        return;
    }
    int dist[vertex_list->size];   // the distances array
    int prev[vertex_list->size];
    int i;

    // init
    printf("initializing..........OK\n");
    for(i = 0; i<vertex_list->size; i++)
	{
        dist[i] = INFINITY ;
        prev[i] = -1;
	}

    //Creating the N list
    printf("creating the N list...OK\n");
	dbllist_t *N = (dbllist_t*) malloc (sizeof(dbllist_t));
	dbllist_init(N);
	dbllist_node_t* node = vertex_list->head;
	while(node != NULL)
	{
		dbllist_append(N,(void *)node->data);
		node = node->next;
	}

	dist[src]= 0 ;

	while((N->size) > 0)
	{
		int min= INFINITY;
		int index;
		for(i=0;i < (vertex_list->size);i++)
		{
			node = check_inlist(N,i);
			if(dist[i]<=min && node != NULL)
			{
				min=dist[i];
				index= i;
			}
		}
		node = check_inlist(N,index);

		if (node->data == NULL) printf("\n Catched \n ");
		dbllist_node_t* edge_node = ((vertex_t*) node->data)->edges_list->head;

		while(edge_node!= NULL)
		{
			if ((((edge_t*)(edge_node->data))->weight + dist[index] ) < dist[((vertex_t*)((edge_t*)edge_node->data)->target->data)->id] )
			{
				dist[((vertex_t*)((edge_t*)edge_node->data)->target->data)->id] = ((edge_t*)(edge_node->data))->weight + dist[index] ;
				prev[((vertex_t*)((edge_t*)edge_node->data)->target->data)->id] = index;
			}
			edge_node = edge_node->next;
		}
		dbllist_remove(N,node,DBLLIST_LEAVE_DATA);

	}

	printf("representation of the end graph:\n");
	print_graph(vertex_list);

    FILE* result_file = fopen("result", "w");
	fprintf(result_file, "Source %d \n",src);
	for(i = 0 ; i < (vertex_list->size) ; i++)
	{
		if(i!=src)
		{
            if(prev[i]==-1) printf("Dst %d unreachable!\n",i);
            else
            {
                fprintf(result_file, "Dst %d Cost %d Via %d\n",i,dist[i],prev[i]);
            }
		}
	}
	fclose(result_file);
	printf("\ncalculation results written into 'result' file!\n");
}

dbllist_node_t* check_inlist(dbllist_t* list , int e)
{
    dbllist_node_t* i = list->head;

    while(i != NULL )
    {
		int x = (int)( (( vertex_t*)(i->data))->id );
		if (x == e ) return i;
		i = i->next ;
    }
    return NULL;
}

void print_graph(dbllist_t* list)
{
    dbllist_node_t* node = list->head;
    while (node!=NULL)
    {
        printf("%d: ", ((vertex_t*) node->data)->id);
        dbllist_node_t* edge = ((vertex_t*) node->data)->edges_list->head;
        while (edge!=NULL)
        {
                printf("%d ", ((vertex_t*)((edge_t*) edge->data)->target->data)->id);
                edge = edge->next;
        }
        node = node->next;
        printf("\n");
    }
}

void exit_prog() //4
{
    printf("releasing assets and terminating... goodbye!\n");
    dbllist_destroy(vertex_list, DBLLIST_FREE_DATA);
}

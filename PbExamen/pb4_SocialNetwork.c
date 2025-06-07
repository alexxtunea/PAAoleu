/*A social media platform wants to analyze user connections to understand community formation. Given friendship data between users, the platform needs to:

    Identify isolated groups of friends who have no connections to other groups => numara comp conexe 
    Determine the size of the largest friend circle => comp conexa cu cele mai multe elemente 
    Find users who could serve as bridges between different communities => punctele de articulatie
    Calculate how many separate social clusters exist in the network => => nr de comp conexe

    NU VAD CARE E DIFERENTA DINTRE PRIMA SI ULTIMA CERINTA ???

You are provided with a dataset of user friendships where each friendship is 
bidirectional. Design an algorithm to analyze these social connections and answer queries about community structure.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100

int nodes;
int adj[MAX_NODES][MAX_NODES];
int visited[MAX_NODES];
int comp_con[MAX_NODES][MAX_NODES];

void dfs(int node, int removed) {
    if (node == removed) return;
    visited[node] = 1;
    for (int i = 1; i <= nodes; i++) {
        if (adj[node][i] && !visited[i] && i != removed) {
            dfs(i, removed);
        }
    }
}

int count_components(int removed) {
    memset(visited, 0, sizeof(visited));
    int count = 0;
    for (int i = 1; i <= nodes; i++) {
        if (!visited[i] && i != removed) {
            dfs(i, removed);
            count++;
        }
    }
    return count;
}

void find_articulation_points() {
    int base = count_components(-1);
    printf("Puncte de articulatie: ");
    for (int i = 1; i <= nodes; i++) {
        int comps = count_components(i);
        if (comps > base)
            printf("%d ", i);
    }
    printf("\n");
}

void dfs_max(int node, int comp_id, int *index) {
    visited[node] = 1;
    comp_con[comp_id][(*index)++] = node;

    for (int i = 1; i <= nodes; i++) {
        if (adj[node][i] && !visited[i])
            dfs_max(i, comp_id, index);
    }
}

void largest_component() {
    memset(visited, 0, sizeof(visited));
    int comp_id = 0, max_size = 0, best_id = 0;

    for (int i = 1; i <= nodes; i++) {
        if (!visited[i]) {
            int index = 0;
            dfs_max(i, comp_id, &index);
            if (index > max_size) {
                max_size = index;
                best_id = comp_id;
            }
            comp_id++;
        }
    }

    printf("Componenta conexa maxima are %d elemente si e compusa din: ", max_size);
    for (int i = 0; i < max_size; i++)
        printf("%d ", comp_con[best_id][i]);
    printf("\n");
}

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }

    fscanf(f, "%d", &nodes);
    int x, y;
    while (fscanf(f, "%d %d", &x, &y) == 2) {
        adj[x][y] = 1;
        adj[y][x] = 1;
    }
    fclose(f);

    printf("Nr de componente conexe: %d\n", count_components(-1));
    largest_component();
    find_articulation_points();

    return 0;
}

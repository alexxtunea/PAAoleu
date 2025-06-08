/*
FUNCTII DE BAZA PT GRAFURI 
-> DFS 
-> BFS
-> Shortest_path folosind BFS
-> Nr de componente conexe 
-> Determinarea punctelor de articulatie 
-> Determinare daca un graf are sau nu cicluri
-> Determinarea ciclului de lungime maxima 
-> Determinare daca un graf e bipartit
-> Componente Biconexe


Un graf neorientat este:

    -> Biconex dacă este conex și nu are puncte de articulație

    -> O componentă biconexă este o componenta conexa care NU are PUNCTE
    DE ARTICULATIE 
*/
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NODES 25

int m_a[MAX_NODES][MAX_NODES], nodes;
int visited[MAX_NODES];

//implementare COADA pentru BFS
//o sa am un infarct cerebral daca mai scriu queue sorry

int coada[MAX_NODES], begin = 0, end = -1;

void push(int val)
{
    coada[++end] = val;
}

void pop()
{
    coada[begin++];
}

int isEmpty()
{
    return (begin > end);
}

void BFS(int start_node)
{
    int vizitat[MAX_NODES];
    //vectorul de predecesor ne trebuie doar daca folosim BFS pt shortest path !!!!
    //PT AFISARE: 
    /*
    int rez[MAX_NODES, index = 0;
    */
    vizitat[start_node] = 1;
    push(start_node);

    while(!isEmpty())
    {
        int nod = coada[begin];
        pop();
        //rez[index++] = nod;

        for(int i=0; i<nodes; i++)
            if(vizitat[i]==0 && m_a[nod][i])
            {
                vizitat[i] = 1;
                push(i);
            }
    }

    //PT AFISARE: bagi printf la vectorul rez 
}

//=======SHORTEST PATH folosind BFS==============
void shortest_path(int nodes, int m_a[MAX_NODES][MAX_NODES], int start, int end)
{
    //O sa avem nevoie de un vector de vizitare si de unul de predecesori
    int visited[MAX_NODES], pred[MAX_NODES];
    for(int i=0; i<MAX_NODES; i++)
    {
        visited[i] = 0;
        pred[i] = -1;
    }

    //bagam nodul de start in coada si il marcam ca vizitat
    push(start);
    visited[start] = 1;

    while(!isEmpty())
    {
        int node = coada[begin];
        pop();

        if(node == end)
            break;

        for(int i=0; i<nodes; i++)
            if(visited[i]==0 && m_a[node][i])
            {
                visited[i] = 1;
                push(i); //il bagam pe i in coada ca sa verificam ce cale urmam
                pred[i] = node;
            }
    }

    //daca dupa ce am epuizat coada, nodul destinatie inca nu a fost vizitat inseamna ca nu avem drum existent pana acolo
    if(visited[end]==0)
    {
        printf("Nu exista drum!");
        return;
    }

    //reconstruim calea de la start la end 
    int path[MAX_NODES], path_len = 0;
    int current = end; //luam de la capat pt ca ne legam de PREDECESORI

    while(current!=-1)
    {
        path[path_len++] = current;
        current = pred[current];
    }

    for(int i=path_len-1; i>=0; i--)
        printf("%d ", path[i]);
    
    printf("\n");
}

void DFS(int start_node)
{
    visited[start_node] = 1;
    //ii futi un printf aici daca vr sa afisezi 

    for(int i=0; i<nodes; i++)
        if(visited[i]==0 && m_a[start_node][i]!=0)
        {
            visited[i] = 1;
            DFS(i);
        }
}

int componente_conexe(int nodes, int m_a[MAX_NODES][MAX_NODES])
{
    int count = 0;

    for(int i=0; i<nodes; i++)
        if(visited[i] == 0) //vezi ca vectorul de visited trebuie sa ti fie ACELASI pe care-l folosesti si la functia de DFS 
        {
            count++;
            DFS(i);
        }
    return count;
}

//=======DETERMINARE PUNCTE DE ARTICULATIE=========
//un punct de articulatie e un nod pe care, daca il eliminam, se mareste nr de componente conexe din graf
int visited_art[MAX_NODES] = {0};

//functie dfs care ignora nodul "eliminat"
void dfs_art(int node, int removed)
{
    if(node == removed) return;

    visited_art[node] = 1;

    for(int i=0; i<nodes; i++)
        if(visited_art[node]==0 && m_a[node][i] && i!=removed)
        {
            visited_art[i] = 1;
            dfs_art(i, removed);
        }
}

//functie care nr comp conexe, ignorand nodul "eliminat"
int count_comp_art(int removed)
{
    memset(visited_art, 0, sizeof(visited_art));
    int count = 0;
    for(int i=0; i<nodes; i++)
        if(visited_art[i]==0 && i!=removed)
        {
            count++;
            dfs_art(i, removed);
        }
    return count;
}

//functia care ne gaseste punctele de articulatie efective
void find_articulation_points()
{
    int ref_value = count_comp_art(-1); //punem -1 ca sa nu identifice niciun nod ca fiind removed
    int ok=0;

    for(int i=0; i<nodes; i++)
    {
        int count = count_comp_art(i);
        if(count > ref_value)
            {
                printf("%d ", i);
                ok = 1;
            }
    }

    if(ok == 0)
        printf("Nu au fost gasite puncte de articulatie!!");
    
    printf("\n");
}

//=====DETERMINAM DACA UN GRAF ARE CICLURI SAU NU
int areCiclu(int nodes, int m_a[MAX_NODES][MAX_NODES], int nod, int parinte, int visited_c[MAX_NODES])
{
    // !!! se apeleaza initial cu parinte = -1
    visited_c[nod] = 1;

    for(int i=0; i<nodes; i++)
    {
        if(m_a[nod][i])
        {
            if(visited_c[i] == 0)
            {
                if(areCiclu(nodes, m_a, i, nod, visited_c))
                    return 1;
            }
            else if(i != parinte) //Nodul i a fost vizitat deja dar nu e parintele => CICLU
            {
                return 1;
            }
        }
    }
    
    return 0; //nu am gasit cicluri
}

//=====DETERMINAREA CICLULUI DE LUNGIME MAXIMA=========
int maxCycle = 0;
void dfsCiclu(int start, int u, int len, int* visited, int parent) {
    visited[u] = 1;

    for (int v = 0; v < nodes; v++) {
        if (m_a[u][v]) {
            if (!visited[v]) {
                dfsCiclu(start, v, len + 1, visited, u);
            } else if (v == start && v != parent && len >= 2) {
                if (len + 1 > maxCycle)
                    maxCycle = len + 1;
            }
        }
    }

    visited[u] = 0; // backtrack
}

int main()
{
    FILE *f = fopen("date.in", "r");
    if (!f) {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }

    fscanf(f, "%d", &nodes);
    int x, y;
    while (fscanf(f, "%d %d", &x, &y) == 2) {
        m_a[x][y] = 1;
        m_a[y][x] = 1;
    }
    fclose(f);

    //int visited[MAX_NODES] = {0};
    for (int i = 0; i < nodes; i++) {
        dfsCiclu(i, i, 0, visited, -1);
    }
    printf("Lungimea celui mai lung ciclu: %d\n", maxCycle);

}
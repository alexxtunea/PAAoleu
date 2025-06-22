/*
Cerințe pentru finalizare

A city planning department needs to connect all neighborhoods to essential utilities (water, electricity, gas) at minimum cost. They have cost estimates for running utility lines between different areas, including:

    Installation costs that vary based on terrain and distance
    The requirement that all neighborhoods must be connected (directly or indirectly)
    Budget constraints requiring the most economical solution
    The need to ensure no redundant connections that don't reduce total cost

Given the cost matrix for connecting different neighborhoods, design an algorithm to determine the minimum cost to connect the entire city to utilities. Compare different approaches and analyze which is more efficient for this specific application.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NODES 20
#define MAXIM 100
#define INF 10000

int m_costuri[MAX_NODES][MAX_NODES], nodes;

void Prim(int m_costuri[MAX_NODES][MAX_NODES], int nodes)
{
    printf("\n||PRIM||\n");
    //consideram ca incepem de la primul nod, gen nodul 0
    int used[MAX_NODES] = {0};
    used[0] = 1;

    for(int pas = 1; pas <nodes; pas++) //se selecteaza nodes-1 muchii (pt ca e arbore duhh)
    {
        int minim = MAXIM, i_min = -1, j_min = -1;
        for(int i=0; i < nodes; i++)
        {
            for(int j=0; j< nodes; j++)
            {
                if(used[i] != used[j] && m_costuri[i][j]!=0 && m_costuri[i][j] < minim)
                    {
                        minim = m_costuri[i][j];
                        i_min = i;
                        j_min = j;
                    }
            }
        }

        if(i_min != -1 && j_min != -1)
        {
            used[j_min] = 1; //includem nodul nou in MST
            printf("Muchie adaugata: %c - %c cu cost %d\n", i_min + 'A', j_min + 'A', m_costuri[i_min][j_min]);
        }
    }
}

void Kruskal()
{
    printf("\n||KRUSKAL||\n");
    int s[MAX_NODES];
    //Vectorul s tine evidenta componentelor in care se afla fiecare nod
    //Este un sistem de identificare pt a NU crea CICLURI in MST
    for(int i=0; i<nodes; i++)
        s[i] = i; //la inceput, fiecare nod e o componenta in sine

    for(int pas = 1 ; pas < nodes; pas++) //avem nodes-1 muchii in MST
    {
        int minim = MAXIM, i_min = -1, j_min = -1;
        
        for(int i=0; i<nodes; i++)
            for(int j=0; j<nodes; j++)
            {
                if(s[i]!=s[j] && m_costuri[i][j]!=0 && m_costuri[i][j] < minim)
                {
                    minim = m_costuri[i][j];
                    i_min = i;
                    j_min = j;
                }
            }
        for(int i=0; i<nodes; i++)
            if(s[i] == s[j_min])
                s[i] = s[i_min];

        if(i_min!=-1 && j_min!=-1)
        {
            printf("Muchie adaugata: %c - %c cu cost %d\n", i_min + 'A', j_min + 'A', m_costuri[i_min][j_min]);
        }
    }
}

void dijkstra(int nodes, int m_a[MAX_NODES][MAX_NODES], int dist[MAX_NODES], int pred[MAX_NODES])
{
    //initializam vectorii de vizitare, distanta si predecesori
    int vizitat[MAX_NODES] = {0};
    for(int i=0; i<nodes; i++)
    {
            dist[i] = INF;
            pred[i] = -1;
    }

    for(int pas = 1; pas < nodes - 1; pas++)
    {
        int minim = MAXIM, u = -1;

        for(int i=0; i<nodes; i++)
            if(vizitat[i]==0 && dist[i]<minim)
                {
                    minim = dist[i];
                    u = i;
                }

        if(u == -1) break;
        vizitat[u] = 1;
        for(int v=0; v<nodes; v++)
                if(vizitat[v]==0 && m_a[u][v]>0 && m_a[u][v] + dist[u] < dist[v])
                {
                    dist[v] = dist[u] + m_a[u][v];
                    pred[v] = u;
                }
    }
}

void afisare_drum(int pred[MAX_NODES], int dest)
{
    if(pred[dest] == -1)
    {
        printf("%d ", dest);
        return;
    }
    afisare_drum(pred, pred[dest]);
    printf(" -> %d", dest);
}


void fw(int nodes, int m_a[MAX_NODES][MAX_NODES], int d[MAX_NODES][MAX_NODES], int t[MAX_NODES][MAX_NODES])
{
    //initializam vectorul de dist si de noduri intermediare
    for(int i=0; i<nodes; i++)
        for(int j=0; j<nodes; j++)
        {
            if(i == j)
                d[i][j] == 0;
            else if(m_a[i][j]!=0)
                d[i][j] = m_a[i][j];
            else
                d[i][j] = INF;
            t[i][j] = -1; //nu avem noduri intermediare for now
        }

    for(int k=0; k<nodes; k++)
        for(int i=0; i<nodes; i++)
            for(int j=0; j<nodes; j++)
            {
                if(d[i][k] + d[k][j] < d[i][j])
                {
                    d[i][j] = d[i][k] + d[k][j];
                    t[i][j] = k;
                }
            }
}

//functie care afiseaza drumul cu nodurile intermediare
void afisare_traseu(int i, int j, int t[MAX_NODES][MAX_NODES])
{
    int k = t[i][j];
    if(k == -1)
    {
        return;
    }
    afisare_traseu(i, k, t);
    printf("%d ", k);
    afisare_traseu(k, j, t);
}

int main()
{
    char file_path[20] = "input2.txt";

    FILE *f = NULL;

    if((f = fopen(file_path, "r")) == NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    fscanf(f, "%d", &nodes);

    int x, y, cost;

    while(fscanf(f, "%d %d %d", &x, &y, &cost) == 3)
    {
        m_costuri[x][y] = cost;
        m_costuri[y][x] = cost;
    }

    Prim(m_costuri, nodes);
    Kruskal();
    return 0;
}
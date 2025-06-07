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
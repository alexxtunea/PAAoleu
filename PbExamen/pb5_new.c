#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NODES 20
#define INF 1000

int m_a[MAX_NODES][MAX_NODES], nodes;
int visited[MAX_NODES];
int d[MAX_NODES][MAX_NODES], t[MAX_NODES][MAX_NODES];

/*Which suppliers can ultimately affect each product (direct and indirect dependencies)
Cautam drumuri cu BFS
Mai intai trebuie sa implementam o structura de tip coada pe care sa o folosim pt BFS
*/

int queue[MAX_NODES], begin=0, end=-1;

void push(int value)
{
    queue[++end] = value;
}

void pop()
{
    queue[begin++];
}

int isEmpty()
{
    if(begin>end || end == -1)
        return 1;
    else
        return 0;
}

void print_queue()
{
    for(int i=begin; i<=end; i++)
        printf("%d ", queue[i]);
    printf("\n");
}

//returneaza 1 daca exista drum de la start la end, respectiv 0 in caz contrar
int shortest_path(int m_a[MAX_NODES][MAX_NODES], int nodes, int start, int end)
{
    int visited[MAX_NODES];
    int predecesor[MAX_NODES];

    for(int i=0; i<MAX_NODES; i++)
    {
        visited[i] = 0;
        predecesor[i] = -1;
    }

    push(start);
    visited[start] = 1;

    while(!isEmpty())
    {
        int nod = queue[begin];
        pop();
        
        if(nod == end)
            break;

        for(int i=1; i<nodes; i++)
        {
            if(visited[i]==0 && m_a[nod][i]!=0)
            {
                visited[i] = 1;
                push(i);
                predecesor[i] = nod;
            }
        }

        if(visited[end] == 0)
        {
            return 0;
        }
        else    
        {
            return 1;
        }
    }
}

void suppliers_affection()
{
    int cnt = 0;
    for(int supplier = 0; supplier < nodes; supplier++)
    {
        int ok = 1;
        for(int node = 0; node < nodes; node ++)
            {
               
                if(shortest_path(m_a, nodes, supplier, node)==0)
                    ok = 0;
            }
        if(ok == 1)
            {
                printf("Supplier %d affects all\n", supplier);
                cnt++;
            }
    }
    if(cnt == 0)
    {
        printf("No suppliers affect that much the system!\n");
    }
}

/*How disruption at one supplier could cascade through the supply chain*/

void DFS_disruption_count(int supplier, int *count)
{
    visited[supplier] = 1;

    for(int i=0; i<nodes; i++)
        if(visited[i]==0 && m_a[supplier][i])
        {
            visited[i] = 1;
            (*count)++;
            DFS_disruption_count(i, count);
        }
}

/*Which supplier relationships create the most vulnerability*/

void vulnerability()
{
    int maxim = -1, index;
    for(int supplier = 0; supplier < nodes; supplier++)
    {
        int val = 0;
        DFS_disruption_count(supplier, &val);

        if(val > maxim)
            {
                maxim = val;
                index = supplier;
            }
    }

    printf("The supplier %d creates the most vulnerability", index);

}

/*The complete dependency matrix for risk planning*/
void fw(int nodes, int m_a[MAX_NODES][MAX_NODES], int d[MAX_NODES][MAX_NODES], int t[MAX_NODES][MAX_NODES])
{
    //initializam vectorii d si t 
    for(int i=0; i<nodes; i++)
        for(int j=0; j<nodes; j++)
        {
            if(i==j)
                d[i][j] == 0;
            else if(m_a[i][j])
                d[i][j] = m_a[i][j];
            else
                d[i][j] = INF;

            t[i][j] = -1;
        }

    for(int k=0; k<nodes; k++)
        for(int i=0; i<nodes; i++)
            for(int j=0; j<nodes; j++)
                if(d[i][k] + d[k][j] < d[i][j])
                {
                    d[i][j] = d[i][k] + d[k][j];
                    t[i][j] = k;
                }
}

int main()
{
    //o sa innebunesc daca mai scriu  O DATA citire de mmatrice de adiacenta asa ca no thanks 
}
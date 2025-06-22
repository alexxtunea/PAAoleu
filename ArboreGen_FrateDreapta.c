#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#define MAX_NODES 50

typedef uint8_t TKey;
typedef uint8_t TNodeRef;

typedef struct{
    TKey key;
    uint8_t prim_fiu, frate_dreapta;
}TNode;

typedef struct{
    TNode nodes[MAX_NODES];
    uint8_t size; //nr de noduri din arbore 
}TTree;

TTree initTree(TTree a)
{
    for(int i=0; i<MAX_NODES; i++)
    {
        a.nodes[i].prim_fiu = 0;
        a.nodes[i].frate_dreapta = 0;
    }
    a.size = 0;
}

//======FUNCTIE DE INSERARE NOD IN ARBORE======
TTree insertNode(TTree a, TNodeRef parent, TKey key)
{
    //actualizam nr de noduri
    a.size++;

    //initializam nodul 
    a.nodes[a.size].key = key;
    a.nodes[a.size].prim_fiu = 0;
    a.nodes[a.size].frate_dreapta = 0;

    //Avem doua cazuri: parintele n-are niciun fiu + parintele are deja fii
    
    //CAZ I: Parintele n-are niciun fiu => key este primul fiu
    if(!a.nodes[parent].prim_fiu)
    {
        a.nodes[parent].prim_fiu = a.size;
        return a;
    }

    TNodeRef tempNode = a.nodes[parent].prim_fiu;
    while(a.nodes[tempNode].frate_dreapta!=0)
          tempNode = a.nodes[tempNode].frate_dreapta;

    a.nodes[tempNode].frate_dreapta = a.size;
    return a;
}

TNodeRef getParent(TTree a, TNodeRef noderef)
{
    //Functia ia fiecare nod din arbore, si verifica daca nu cumva noderef se gaseste printre copiii lui
    for(int i=0; i<a.size; i++)
    {
        //Incepem cautarea cu primul fiu si dupa ne uitam la fratii lui sa vedem daca-l gasim pe noderef
        for(int child = a.nodes[i].prim_fiu; child!=0; child = a.nodes[child].frate_dreapta)
        {
            if(child == noderef)
            {
                return i;
            }
        }
    }
    return 0; //radacina sau nod inexistent 
}

TNodeRef getRoot(TTree a)
{
    if(a.size<1) return 0;
    return 1;
}

TKey nodeKey(TTree a, TNodeRef index)
{
    if(index > a.size) return 0;
    return a.nodes[index].key;
}

TNodeRef firstChild(TTree a, TNodeRef index)
{
    return a.nodes[index].prim_fiu;
}

TNodeRef rightSibling(TTree a, TNodeRef index)
{
    return a.nodes[index].frate_dreapta;
}


//=======FUNCTII DE PARCURGERE======
void preOrderDo(TTree a, TNodeRef root)
{
    printf("%d ", nodeKey(a, root));
    TNodeRef fc = firstChild(a, root);
    if(fc) preOrderDo(a, fc);
    TNodeRef rs = fc;
    while((rs = rightSibling(a, rs)))
        preOrderDo(a, rs);
}

void inOrderDo(TTree a, TNodeRef root)
{
    TNodeRef fc = firstChild(a, root);
    if(!fc)
    {
        printf("%d", nodeKey(a, root));
        return;
    }
    preOrderDo(a, fc);
    printf("%d ", nodeKey(a, root));
    TNodeRef rs = fc;
    while((rs = rightSibling(a, rs)))
        inOrderDo(a, rs);
}

void postOrderDo(TTree a, TNodeRef root)
{
    if(!root) return;
    TNodeRef fc = firstChild(a, root);
    postOrderDo(a, fc);
    TNodeRef rs = fc;
    while((rs = rightSibling(a, rs)))
        postOrderDo(a, rs);
    printf("%d ", nodeKey(a, root));
}

//=======CALCULAT INALTIME ARBORE=======
int heightDo(TTree a, TNodeRef root)
{
    if(root == 0) return 0;
    int maxH = 0;
    for(int child = firstChild(a, root); child!=0; child = rightSibling(a, child))
    {
        int h = heightDo(a, child);
        if(h > maxH)
            maxH = h;
    }
    return 1+maxH;
}

int getHeight(TTree a)
{
    if(a.size == 0) return 0;
    return heightDo(a, getRoot(a));
}

int main()
{
    return 0;
}
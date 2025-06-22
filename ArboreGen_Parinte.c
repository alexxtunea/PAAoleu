#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#define MAXNODES 40

typedef uint8_t TKey;

typedef struct 
{
    uint8_t parent;
    TKey key;
}TNode;

typedef uint8_t TNodeRef;

typedef struct 
{
    TNode nodes[MAXNODES];
    uint8_t size;
}TTree;

TTree initTree(TTree arbore)
{
    for(int i=0; i<MAXNODES; i++)
    {
        arbore.nodes[i].key = 0;
        arbore.nodes[i].parent = 0;
    }
    arbore.size = 0;
    return arbore;
}

TTree insertNode(TTree arbore, TNodeRef parent, TKey key)
{
    arbore.size++;
    arbore.nodes[arbore.size].key = key;
    arbore.nodes[arbore.size].parent = parent;
    return arbore;
}

TNodeRef getRoot(TTree arbore) //basically functia asta returneaza indicele 1, adica radacina mereu va fi pe pozitia 1 in vectorul de noduri
{
    if(arbore.size<1) return 0;
    else              return 1;
}

TNodeRef firstChild(TTree arbore, TNodeRef index)
{
    if(index == 0) return 0;

    for(int i=1; i<=arbore.size; i++)
        if(arbore.nodes[i].parent == index)
            return i;

    return 0; //indexul nu e parinte pt niciun nod 
}

TNodeRef rightSibling(TTree arbore, TNodeRef index)
{
    if(index == 0) return 0;
    for(int i=index+1; i<=arbore.size; i++)
        if(arbore.nodes[i].parent == arbore.nodes[index].parent)
            return i;
    return 0;
}

TKey nodeKey(TTree arbore, TNodeRef index)
{
    if(index>arbore.size) return 0;
    return arbore.nodes[index].key;
}

TTree deleteNode(TTree a, TNodeRef noderef) //sterge nodul si toti descendentii lui
{
    a.nodes[noderef].key = 0;
    a.nodes[noderef].parent = 0;

    for(TNodeRef i = noderef; i<=a.size; i++)
        if(a.nodes[i].parent == noderef)
        {
            a.nodes[i].key = 0;
            a.nodes[i].parent = 0;
        }

    for(TNodeRef i=noderef; i<=a.size; i++)
    {
       if(a.nodes[i].parent == 0 && a.nodes[i].key == 0)
        {
            for(TNodeRef j=i+1; j<=a.size; j++)
            {
                if(a.nodes[j].parent > i)
                    a.nodes[j].parent--;
                
                a.nodes[j-1] = a.nodes[j];
            }
            a.size--;
        }
        if(a.nodes[i].parent != 0 && a.nodes[i].key!=0)
            i++;//sarim peste
    }

    return a;

}

int height(TTree a, TNodeRef node)
{
    int maxH = 0;
    for(TNodeRef i = 1; i<=a.size; i++)
    {
        if(a.nodes[i].parent == node)
            {
                int h = height(a, i);
                if(h > maxH)
                    maxH = h;
            }
    }
    return maxH+1;
}

int getHeight(TTree a)
{
    if(a.size == 0) return 0;
    return height(a, getRoot(a));
}

void preOrderDo(TTree arbore, TNodeRef root)
{
    printf("%d ", nodeKey(arbore, root));
    TNodeRef fc = firstChild(arbore, root);
    if(fc) preOrderDo(arbore, fc);
    TNodeRef rs = fc;
    while((rs = rightSibling(arbore, rs))){
        preOrderDo(arbore, rs);
    }
}

void preOrder(TTree arbore)
{
    if(arbore.size<1) return;
    preOrderDo(arbore, getRoot(arbore));
}

void inOrderDo(TTree arbore, TNodeRef root)
{
    TNodeRef fc = firstChild(arbore, root);
    if(!fc)
    {
        printf("%d ", nodeKey(arbore, root));
        return;
    }
    inOrderDo(arbore, fc);
    printf("%d ", nodeKey(arbore, root));
    TNodeRef rs = fc;
    while((rs = rightSibling(arbore, rs))){
        inOrderDo(arbore, rs);
    }
}

void inOrder(TTree arbore)
{
    if(arbore.size<1) return;
    inOrderDo(arbore, getRoot(arbore));
}

void postOrderDo(TTree arbore, TNodeRef root)
{
    if(!root) return;
    TNodeRef fc = firstChild(arbore, root);
    postOrderDo(arbore, fc);
    TNodeRef rs = fc;
    while((rs = rightSibling(arbore, rs))){
        postOrderDo(arbore, rs);
    }
    printf("%d ", nodeKey(arbore, root));
}

void postOrder(TTree arbore)
{
    if(arbore.size<1) return;
    postOrderDo(arbore, getRoot(arbore));
}

void afiseazaPeNivel(TTree arbore, int nivelCautat) {
    int nivel[MAXNODES] = {0};

    // Radacina este mereu pe poziția 1, și e pe nivelul 1
    nivel[1] = 1;

    // Calculăm nivelurile pentru toate nodurile
    for (int i = 2; i <= arbore.size; i++) {
        int parinte = arbore.nodes[i].parent;
        nivel[i] = nivel[parinte] + 1;
    }

    printf("Noduri de pe nivelul %d: ", nivelCautat);
    for (int i = 1; i <= arbore.size; i++) {
        if (nivel[i] == nivelCautat) {
            printf("%d ", arbore.nodes[i].key);
        }
    }
    printf("\n");
}

void traseuInfluenta(TTree tree, TNodeRef a, TNodeRef b) {
    int pathA[MAXNODES], lenA = 0;
    int pathB[MAXNODES], lenB = 0;

    //LCA = Lowest Common Ancestor
    // Construim drumul de la A la rădăcină
    while (a != 0) {
        pathA[lenA++] = a;
        a = tree.nodes[a].parent;
    }

    // Construim drumul de la B la rădăcină
    while (b != 0) {
        pathB[lenB++] = b;
        b = tree.nodes[b].parent;
    }

    // Căutăm ultimul strămoș comun (LCA)
    int i = lenA - 1, j = lenB - 1;
    while (i >= 0 && j >= 0 && pathA[i] == pathB[j]) {
        i--; j--;
    }
    int lca = pathA[i+1];

    // Afișăm drumul: de la A până la LCA
    for (int k = 0; k <= i; k++) {
        printf("%d -> ", tree.nodes[pathA[k]].key);
    }

    // Afișăm LCA
    printf("%d", tree.nodes[lca].key);

    // De la B până la LCA (invers)
    for (int k = j; k >= 0; k--) {
        printf(" -> %d", tree.nodes[pathB[k]].key);
    }

    printf("\n");
}

int main() {
    TTree arbore = initTree(arbore);

    // arbore:       10
    //             /    \
    //           20      30  70
    //          /  \      
    //        40    50
    //                 \
    //                  60

    arbore = insertNode(arbore, 0, 10); // rădăcina (index 1)
    arbore = insertNode(arbore, 1, 20); // index 2
    arbore = insertNode(arbore, 1, 30); // index 3
    arbore = insertNode(arbore, 1, 70); // index 4
    arbore = insertNode(arbore, 2, 40); // index 5
    arbore = insertNode(arbore, 2, 50); // index 6
    arbore = insertNode(arbore, 5, 60); // index 7

    //stergem 20 (cu 40, 50, 60)
    //arbore = deleteNode(arbore, 2);  

    printf("Inaltimea arborelui este: %d\n", getHeight(arbore));
    printf("Radacina este %d\n", nodeKey(arbore, getRoot(arbore)));

    printf("Afisarea in PREordine este: \n");
    preOrder(arbore); printf("\n");

    printf("Afisarea in INordine este: \n");
    inOrder(arbore); printf("\n");

    printf("Afisarea in POSTordine este: \n");
    postOrder(arbore); printf("\n");

    afiseazaPeNivel(arbore, 2);

    traseuInfluenta(arbore, 2, 4);

    return 0;
}
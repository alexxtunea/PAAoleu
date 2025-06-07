#include <stdio.h>
#include <stdlib.h>
#define MAX_NODURI 100

typedef int TipCheie;

// =============================
// 1. Arbore cu vector (părinte)
// =============================
typedef struct {
    TipCheie cheie;
    int parinte;
} NodTabel;

typedef struct {
    NodTabel noduri[MAX_NODURI];
    int nr_noduri;
} ArboreTabel;

ArboreTabel initializeazaTabel() {
    ArboreTabel A;
    A.nr_noduri = 0;
    return A;
}

void insereazaTabel(ArboreTabel *A, TipCheie valoare, int index_parinte) {
    if (A->nr_noduri >= MAX_NODURI) {
        printf("Depășire dimensiune\n");
        return;
    }
    A->noduri[A->nr_noduri].cheie = valoare;
    A->noduri[A->nr_noduri].parinte = index_parinte;
    A->nr_noduri++;
}

void preordineTabel(ArboreTabel *A, int index_nod) {
    printf("%d ", A->noduri[index_nod].cheie);
    for (int i = 0; i < A->nr_noduri; i++)
        if (A->noduri[i].parinte == index_nod)
            preordineTabel(A, i);
}

void postordineTabel(ArboreTabel *A, int index_nod) {
    for (int i = 0; i < A->nr_noduri; i++)
        if (A->noduri[i].parinte == index_nod)
            postordineTabel(A, i);
    printf("%d ", A->noduri[index_nod].cheie);
}

void inordineTabel(ArboreTabel *A, int index_nod) {
    int copii[MAX_NODURI], nr = 0;
    for (int i = 0; i < A->nr_noduri; i++)
        if (A->noduri[i].parinte == index_nod)
            copii[nr++] = i;
    if (nr > 0) inordineTabel(A, copii[0]);
    printf("%d ", A->noduri[index_nod].cheie);
    for (int i = 1; i < nr; i++)
        inordineTabel(A, copii[i]);
}


// =============================
// 2. Arbore cu liste (fii/frati)
// =============================
typedef struct NodLista {
    TipCheie cheie;
    struct NodLista *frate;
    struct NodLista *fii;
} NodLista;

typedef NodLista* ArboreLista;

ArboreLista initializeazaLista() {
    return NULL;
}

NodLista* creazaNodLista(TipCheie cheie) {
    NodLista *nod = (NodLista*)malloc(sizeof(NodLista));
    if (!nod) {
        printf("Eroare la alocare\n");
        exit(EXIT_FAILURE);
    }
    nod->cheie = cheie;
    nod->frate = NULL;
    nod->fii = NULL;
    return nod;
}

void insereazaLista(NodLista *parinte, NodLista *copil) {
    if (!parinte->fii) {
        parinte->fii = copil;
    } else {
        NodLista *p = parinte->fii;
        while (p->frate)
            p = p->frate;
        p->frate = copil;
    }
}

void suprimaLista(NodLista *nod) {
    if (!nod) return;
    suprimaLista(nod->fii);
    suprimaLista(nod->frate);
    free(nod);
}

void preordineLista(NodLista *nod) {
    if (!nod) return;
    printf("%d ", nod->cheie);
    preordineLista(nod->fii);
    preordineLista(nod->frate);
}

void postordineLista(NodLista *nod) {
    if (!nod) return;
    postordineLista(nod->fii);
    printf("%d ", nod->cheie);
    postordineLista(nod->frate);
}

void inordineLista(NodLista *nod) {
    if (!nod) return;
    NodLista *copil = nod->fii;
    if (copil) {
        inordineLista(copil);
        copil = copil->frate;
    }
    printf("%d ", nod->cheie);
    while (copil) {
        inordineLista(copil);
        copil = copil->frate;
    }
}

// =============================
// MAIN
// =============================
int main() {
    printf("==== Implementare cu vector (părinte) ====\n");
    ArboreTabel A = initializeazaTabel();

    // Index 0 este rezervat ca "nod fictiv"
    insereazaTabel(&A, 0, -1); // index 0 (fictiv)

    //        1
    //       / \\
    //      2   3
    //     / \\
    //    4   5
    insereazaTabel(&A, 1, 0);  // index 1 = radacina
    insereazaTabel(&A, 2, 1);  // index 2
    insereazaTabel(&A, 3, 1);  // index 3
    insereazaTabel(&A, 4, 2);  // index 4
    insereazaTabel(&A, 5, 2);  // index 5

    printf("Preordine: "); preordineTabel(&A, 1); printf("\n");
    printf("Inordine: ");  inordineTabel(&A, 1);  printf("\n");
    printf("Postordine: "); postordineTabel(&A, 1); printf("\n");
    
    printf("\n==== Implementare cu liste (fii/frati) ====\n");

    ArboreLista B = creazaNodLista(1);
    NodLista *n2 = creazaNodLista(2);
    NodLista *n3 = creazaNodLista(3);
    NodLista *n4 = creazaNodLista(4);
    NodLista *n5 = creazaNodLista(5);

    insereazaLista(B, n2);
    insereazaLista(B, n3);
    insereazaLista(n2, n4);
    insereazaLista(n2, n5);

    printf("Preordine: "); preordineLista(B); printf("\n");
    printf("Inordine: ");  inordineLista(B);  printf("\n");
    printf("Postordine: "); postordineLista(B); printf("\n");

    suprimaLista(B);
    return 0;
}

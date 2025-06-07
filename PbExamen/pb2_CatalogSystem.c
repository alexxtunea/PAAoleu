#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N   3               // ordinul B-Tree (pentru m=5, N = 3)
#define NN  (N * 2)         // număr maxim de chei per nod (6)

typedef const char* TipCheie;

typedef struct Nod {
    char* cheie;            // Copie locală (nu const)
    struct Pagina* p;
} Nod;

typedef struct Pagina {
    int m;
    struct Pagina* p0;
    Nod e[NN + 1];          // 1-based indexing
} Pagina;

Nod v;

// === Inserare în arbore B ===
Pagina* insereaza(Pagina* pag, TipCheie x, Nod* nod) {
    int i, s, d, mij;
    Pagina* q, * r;

    if (!nod) {
        v.cheie = strdup(x); // Copiem valoarea
        v.p = NULL;
    } else {
        v = *nod;
    }

    if (pag == NULL) {
        pag = (Pagina*)calloc(1, sizeof(Pagina));
        pag->e[++pag->m] = v;
        return pag;
    }

    s = 1;
    d = pag->m;
    while (s <= d) {
        mij = (s + d) / 2;
        int cmp = strcmp(x, pag->e[mij].cheie);
        if (cmp == 0) return pag; // deja exista
        if (cmp < 0) d = mij - 1;
        else s = mij + 1;
    }

    if (pag->p0 == NULL || nod) {
        if (pag->m < NN) {
            ++pag->m;
            for (i = pag->m; i > d + 1; i--)
                pag->e[i] = pag->e[i - 1];
            pag->e[i] = v;
        } else {
            Pagina* a = pag;
            Pagina* b = (Pagina*)calloc(1, sizeof(Pagina));
            pag = (Pagina*)calloc(1, sizeof(Pagina));
            for (i = 1; i <= N; i++)
                b->e[i] = a->e[i + N];
            a->m = b->m = N;

            if (strcmp(v.cheie, a->e[N].cheie) > 0 && strcmp(v.cheie, b->e[1].cheie) < 0) {
                pag->e[++pag->m] = v;
            } else {
                if (strcmp(v.cheie, a->e[N].cheie) < 0) {
                    pag->e[++pag->m] = a->e[N];
                    for (i = a->m; i > 1 && strcmp(a->e[i - 1].cheie, v.cheie) > 0; i--)
                        a->e[i] = a->e[i - 1];
                    a->e[i] = v;
                } else {
                    pag->e[++pag->m] = b->e[1];
                    for (i = 1; i < b->m && strcmp(b->e[i + 1].cheie, v.cheie) < 0; i++)
                        b->e[i] = b->e[i + 1];
                    b->e[i] = v;
                }
            }

            b->p0 = pag->e[1].p;
            pag->p0 = a;
            pag->e[1].p = b;
        }
    } else {
        q = (d == 0) ? pag->p0 : pag->e[d].p;
        r = insereaza(q, x, NULL);
        if (r != q) {
            pag = insereaza(pag, r->e[1].cheie, &r->e[1]);
            free(r);
        }
    }

    return pag;
}

// === Afișare în ordine ===
void afisare(Pagina* pag, int nivel) {
    if (!pag) return;

    afisare(pag->p0, nivel + 1);
    for (int i = 1; i <= pag->m; i++) {
        afisare(pag->e[i].p, nivel + 1);
    }

    printf("Nivel %d: ", nivel);
    for (int i = 1; i <= pag->m; i++) {
        printf("%s ", pag->e[i].cheie);
    }
    printf("\n");
}

// === Căutare în arbore B ===
int cautare(Pagina* pag, TipCheie x, int* comparisons) {
    int s, d, mij;

    while (pag) {
        s = 1;
        d = pag->m;
        while (s <= d) {
            (*comparisons)++;
            mij = (s + d) / 2;
            int cmp = strcmp(x, pag->e[mij].cheie);
            if (cmp == 0) return 1;
            if (cmp < 0) d = mij - 1;
            else s = mij + 1;
        }
        pag = (d == 0) ? pag->p0 : pag->e[d].p;
    }

    return 0;
}

// === Determină înălțimea arborelui ===
int inaltime(Pagina* pag) {
    if (!pag) return 0;
    return 1 + inaltime(pag->p0);
}

// === Verifică echilibrul (toate frunzele la același nivel) ===
int verificaEchilibru(Pagina* pag, int nivel, int* nivelFrunza) {
    if (!pag) return 1;

    if (pag->p0 == NULL) {
        if (*nivelFrunza == -1) *nivelFrunza = nivel;
        else if (*nivelFrunza != nivel) return 0;
        return 1;
    }

    if (!verificaEchilibru(pag->p0, nivel + 1, nivelFrunza)) return 0;
    for (int i = 1; i <= pag->m; i++) {
        if (!verificaEchilibru(pag->e[i].p, nivel + 1, nivelFrunza)) return 0;
    }

    return 1;
}

void testEchilibru(Pagina* radacina) {
    int nivelFrunza = -1;
    if (verificaEchilibru(radacina, 0, &nivelFrunza))
        printf("Arborele este echilibrat optim (toate frunzele la nivelul %d).\n", nivelFrunza);
    else
        printf("Arborele NU este echilibrat optim.\n");
}

void afisareVizuala(Pagina* pag, int nivel, const char* prefix) {
    if (!pag) return;

    char newPrefix[256];
    strcpy(newPrefix, prefix);
    strcat(newPrefix, "    ");  // pentru indentare

    // Afișăm subarborele p0 (primul copil)
    if (pag->p0) {
        printf("%s└──[p0]\n", prefix);
        afisareVizuala(pag->p0, nivel + 1, newPrefix);
    }

    // Afișăm fiecare cheie și subarborele corespunzător
    for (int i = 1; i <= pag->m; i++) {
        printf("%s└── %s\n", prefix, pag->e[i].cheie);
        if (pag->e[i].p) {
            afisareVizuala(pag->e[i].p, nivel + 1, newPrefix);
        }
    }
}


// === MAIN ===
int main() {
    Pagina* catalog = NULL;
    TipCheie categorii[] = {
        "Fiction", "Adventure", "Biography", "History", "Mathematics", "Physics",
        "Art", "Chemistry", "Drama", "Fantasy", "Geology", "Medicine",
        "Poetry", "Science", "Technology", "Zoology"
    };

    int n = sizeof(categorii) / sizeof(categorii[0]);
    for (int i = 0; i < n; i++) {
        catalog = insereaza(catalog, categorii[i], NULL);
    }

    printf("Structura arborelui B:\n");
    afisare(catalog, 0);

    TipCheie cautat = "Mathematics";
    int comparisons = 0;
    int found = cautare(catalog, cautat, &comparisons);
    printf("Categoria '%s' %s in catalog (comparatii = %d).\n",
           cautat, found ? "EXISTA" : "NU EXISTA", comparisons);

    int h = inaltime(catalog);
    printf("Inaltime arbore: %d\n", h);
    printf("Numar maxim comparatii posibile (aprox): %d\n", h * N);

    testEchilibru(catalog);

    afisareVizuala(catalog, 0, "");
    return 0;

    //Nu pricep ce se intampla aici scuze 
}

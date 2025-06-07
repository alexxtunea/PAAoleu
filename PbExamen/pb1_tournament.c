/*A sports tournament organizer needs to manage elimination brackets for 64 teams. Each match eliminates one team, and winners advance to the next round until a champion is determined. The system must efficiently:

    Track which teams advance at each round
    Determine the path any team took to reach their final position
    Calculate the minimum number of matches needed
    Find all teams that were eliminated by the eventual winner

Design an algorithm to represent this tournament structure and implement functions to answer queries about team progression and match outcomes.*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAX_LEVELS 10
#define MAX_TEAMS_PER_ROUND 64


typedef uint8_t TKey;

typedef struct {
    TKey values[MAX_TEAMS_PER_ROUND];
    int count;
} RoundList;


typedef struct BTreeNode {
    TKey key;
    struct BTreeNode* left;
    struct BTreeNode* right;
} BTreeNode;

BTreeNode* createBTreeNode(TKey key) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    if (!newNode) {
        printf("Error at malloc\n");
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Afișează drumul de la echipă la rădăcină (traseu în turneu)
int showPathToRoot(BTreeNode* root, TKey team) {
    if (!root) return 0;
    if (root->key == team) {
        printf("%d ", root->key);
        return 1;
    }
    if (showPathToRoot(root->left, team) || showPathToRoot(root->right, team)) {
        printf("-> %d ", root->key);
        return 1;
    }
    return 0;
}

// basically ne uitam in ambii subarbori si afisam nodul din subarbore care nu coincide cu castigatorul
void listDefeatedByChampion(BTreeNode* root) {
    if (!root) return;
    if (root->left && root->left->key != root->key)
        printf("%d ", root->left->key);
    if (root->right && root->right->key != root->key)
        printf("%d ", root->right->key);
    listDefeatedByChampion(root->left);
    listDefeatedByChampion(root->right);
}

// Creează arbore binar pentru un meci eliminatoriu
BTreeNode* simulateMatch(TKey winner, TKey leftTeam, TKey rightTeam) {
    BTreeNode* node = createBTreeNode(winner);
    node->left = createBTreeNode(leftTeam);
    node->right = createBTreeNode(rightTeam);
    return node;
}

void printBTreeVisual(BTreeNode* root, int level) {
    if (root == NULL) return;

    printBTreeVisual(root->right, level + 1);

    for (int i = 0; i < level; i++) {
        printf("    ");  // 4 spații pentru fiecare nivel
    }
    printf("%d\n", root->key);

    printBTreeVisual(root->left, level + 1);
}


void adaugaEchipaLaRunda(RoundList rounds[], int level, TKey key) {
    rounds[level].values[rounds[level].count++] = key;
}


void bfsPeNiveluri(BTreeNode* root) {
    if (!root) return;

    BTreeNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    int nivel = 0;

    while (front < rear) {
        int count = rear - front;  // numărul de noduri la acest nivel
        nivel++;
        printf("Runda %d: ", 5-nivel);

        for (int i = 0; i < count; i++) {
            BTreeNode* node = queue[front++];
            printf("%d ", node->key);

            if (node->left) queue[rear++] = node->left;
            if (node->right) queue[rear++] = node->right;
        }

        printf("\n");
    }
}

void preorderPathToTeam(BTreeNode* root, TKey team) {
    if (!root) return;

    // Dacă subarborele curent conține echipa, îl parcurgem
    if (root->key == team) {
        printf("%d ", root->key);
        preorderPathToTeam(root->left, team);
        preorderPathToTeam(root->right, team);
    }
}

void minim_meciuri(BTreeNode* root, int *parents)
{
    if(root == NULL) 
    {
        return;
    }
    if(root->left || root->right) //daca are cel putin un copil => e meci
        (*parents)++;
    minim_meciuri(root->left, parents);
    minim_meciuri(root->right, parents);
}


int main() {
    // Simulăm semifinale + finală (8 echipe: 1-8)
    // sf1: 1 vs 2 -> 1
    // sf2: 3 vs 4 -> 3
    // sf3: 5 vs 6 -> 5
    // sf4 : 7 vs 8 -> 7

    BTreeNode* sf1 = simulateMatch(1, 1, 2);
    BTreeNode* sf2 = simulateMatch(3, 3, 4);
    BTreeNode* sf3 = simulateMatch(5, 5, 6);
    BTreeNode* sf4 = simulateMatch(7, 7, 8);

    // qf1: 1 vs 3 -> 1
    BTreeNode* qf1 = createBTreeNode(1);
    qf1->left = sf1;
    qf1->right = sf2;

    // qf2: 5 vs 7 -> 5
    BTreeNode* qf2 = createBTreeNode(5);
    qf2->left = sf3;
    qf2->right = sf4;

    // final: 1 vs 5 -> 1 (campion)
    BTreeNode* finalMatch = createBTreeNode(1);
    finalMatch->left = qf1;
    finalMatch->right = qf2;

    printf("\nEchipele care au avansat la fiecare rundă:\n");
    bfsPeNiveluri(finalMatch);

    printf("Traseu echipa 5: ");
    //showPathToRoot(finalMatch, 5);
    preorderPathToTeam(finalMatch, 5);
    printf("\n");

    int parents = 0;
    printf("Nr minim de meciuri jucate: ");
    minim_meciuri(finalMatch, &parents);
    printf("%d \n", parents);

    printf("Echipe eliminate de campionul %d: ", finalMatch->key);
    listDefeatedByChampion(finalMatch);
    printf("\n");

    printf("Structura arbore:\n");
    printBTreeVisual(finalMatch, 0);

    return 0;
}
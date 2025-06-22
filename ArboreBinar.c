#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#define MAXNODES 50

typedef uint8_t TKey;

typedef struct BTreeNode
{
    TKey key;
    struct BTreeNode *left;
    struct BTreeNode *right;
}BTreeNode;


BTreeNode *createBTreeNode(TKey key)
{
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    if(newNode == NULL)
    {
        perror("Eroare la alocarea memoriei!\n");
        exit(EXIT_FAILURE);
    }
    
    newNode->key = key;
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}


//=====INSERARE NOD IN ARBORE BINAR ORDONAT=====
BTreeNode *insertNode(BTreeNode* root, TKey key)
{
    if(root == NULL)
    {
        return createBTreeNode(key);
    }
    if(key < root->key)
    {
        root->left = insertNode(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = insertNode(root->right, key);
    }
    return root;
}

//======PARCURGERI ARBORE BINAR===============
void preOrder(BTreeNode *root)
{
    if(root == NULL) return;
    printf("%d ", root->key);
    preOrder(root->left);
    preOrder(root->right);
}

void inOrder(BTreeNode *root)
{
    if(root == NULL) return;
    inOrder(root->left);
    printf("%d ", root->key);
    inOrder(root->right);
}

void postOrder(BTreeNode *root)
{
    if(root == NULL) return;
    postOrder(root->left);
    postOrder(root->right);
    printf("%d ", root->key);
}

int main()
{
    return 0;
}
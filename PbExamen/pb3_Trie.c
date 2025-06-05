/*
A search engine needs to implement an autocomplete feature for millions of search queries. Users type partial queries and expect to see the most relevant completions instantly. The system must:

    Handle prefix matching for any partial input
    Return the top 10 most frequent completions for each prefix
    Support real-time updates when new search terms become popular
    Efficiently store queries that share common prefixes

Given a dataset of search queries with their frequency counts, design an algorithm to build and query this autocomplete system. Consider memory efficiency since common prefixes like "how to" appear in thousands of queries.

Mentioneaza prefixe => arbori de regasire 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_SIZE 27  //a-z + space character
#define MAX_SUGGESTIONS 10
#define MAX_WORD_LEN 100

typedef struct TrieNode{
	struct TrieNode *children[ALPHABET_SIZE];
	bool end_of_word;
	int frequency; //frequency of the word if end_of_word = true
}TrieNode;

typedef struct Suggestion{
	char word[MAX_WORD_LEN];
	int frequency;
}Suggestion;

//function that maps character to index (a-z = 0-25 + space = 26)
int chartoIndex(char c)
{
	if(c == ' ') return 26;
	else
				 return c-'a';
}

char indextoChar(int i)
{
	if(i == 26) return ' ';
	else	
				return (char)('a' + i);
}

//create new trie node
TrieNode *createNode(){
	TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
	node->end_of_word =  false;
	node->frequency = 0;
	for(int i=0; i<ALPHABET_SIZE; i++)
		node->children[i] = NULL;
	
	return node;
}

//insert word with frequency
void insert(TrieNode *root, const char *key, int frequency)
{
	TrieNode *current = root;
	for(int i=0; key[i]!='\0'; i++){
		int index = chartoIndex(key[i]);
		if(current->children[index]==NULL){
			current->children[index] = createNode();
		}
		current = current->children[index];
	}
	current->end_of_word = true;
	current->frequency = frequency;
}

//function that returns true if the word is found
bool search(TrieNode *root, const char *key)
{
	TrieNode *current = root;

	for(int i=0; key[i]!='\0'; i++)
	{
		int index = chartoIndex(key[i]);
		if(index<0 || index>=ALPHABET_SIZE || current->children[index]==NULL)
			return false;

		current = current->children[index];
	}
	return current!=NULL && current->end_of_word;
}


//finds the last node corresponding to the input prefix 
TrieNode *findNode(TrieNode *root, const char *prefix)
{
	TrieNode *current = root;

	for(int i=0; prefix[i]!='\0'; i++)
	{
		int index = chartoIndex(prefix[i]);

		if(index<0 || index>=ALPHABET_SIZE || current->children[index]==NULL)
			return NULL;

		current = current->children[index];
	}

	return current;
}


void insertSuggestion(Suggestion *top, int *size, const char *word, int frequency)
{
	//daca cuvantul este deja in top, actualizam si iesim din functie
	for(int i=0; i< *size; i++)
	{
		if(strcmp(top[i].word, word) == 0){
			top[i].frequency = frequency;
			return;
		}
	}

	//daca nu este deja, il adaugam
	if(*size < MAX_SUGGESTIONS)
	{
		strcpy(top[*size].word, word);
		top[*size].frequency = frequency;
		(*size)++;
	}
	else
	{
		//daca nu mai avem loc in top 10, inlocuieste sugestia cu frecventa cea mai mica 
		int min_index = 0;
		for(int i=1; i < MAX_SUGGESTIONS; i++)
		{
			if(top[i].frequency < top[min_index].frequency){
				min_index = i;
			}
		}
		if(frequency > top[min_index].frequency){
			strcpy(top[min_index].word, word);
			top[min_index].frequency = frequency;
		}
	}
}


//
void collectSuggestions(TrieNode *node, char *prefix, Suggestion *top, int *size)
{
	if(node == NULL) return;

	if(node->end_of_word){
		insertSuggestion(top, size, prefix, node->frequency);
	}


	for(int i=0; i < ALPHABET_SIZE; i++)
	{
		if(node->children[i])
		{
			char next[MAX_WORD_LEN];
			strcpy(next, prefix);
			int len = strlen(next);
			next[len] =  indextoChar(i);
			next[len+1] = '\0';
			collectSuggestions(node->children[i], next, top, size);
		}
	}
}

void autocomplete(TrieNode *root, const char *prefix)
{
	TrieNode *node = findNode(root, prefix);
	if(node == NULL)
	{
		printf("No suggestions for prefix %s \n", prefix);
		return;
	}

	Suggestion top[MAX_SUGGESTIONS];
	int size = 0;
	char buffer[MAX_WORD_LEN];
	strcpy(buffer, prefix);
	collectSuggestions(node, buffer, top, &size);

	printf("Top suggestions for %s: \n", prefix);
	for(int i=0; i<size; i++)
		printf("%s (%d)\n", top[i].word, top[i].frequency);

}

int main()
{
	TrieNode *root = createNode();

	insert(root, "how", 5);
	insert(root, "how to", 15);
	insert(root, "house", 10);
	insert(root, "how to cook", 3);
	insert(root, "hope", 9);
	insert(root, "howard", 5);
	insert(root, "how to code", 25);

	autocomplete(root, "how");
	return 0;
}
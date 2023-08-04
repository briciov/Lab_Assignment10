#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};

// Initializes a new trie node
struct TrieNode* createTrieNode()
{
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (newNode != NULL)
    {
        newNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            newNode->children[i] = NULL;
    }
    return newNode;
}

// Trie structure
struct Trie
{
    struct TrieNode* root;
};

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (newTrie != NULL)
    {
        newTrie->root = createTrieNode();
    }
    return newTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return;

    struct TrieNode* currNode = pTrie->root;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currNode->children[index] == NULL)
            currNode->children[index] = createTrieNode();

        currNode = currNode->children[index];
    }

    currNode->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return 0;

    struct TrieNode* currNode = pTrie->root;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currNode->children[index] == NULL)
            return 0;

        currNode = currNode->children[index];
    }

    return currNode->count;
}

// Helper function to deallocate the trie nodes
void deallocateTrieHelper(struct TrieNode* node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != NULL)
            deallocateTrieHelper(node->children[i]);
    }

    free(node);
}

// Deallocate the trie structure
struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie == NULL)
        return NULL;

    deallocateTrieHelper(pTrie->root);
    free(pTrie);
    return NULL;
}

// Reads all the words in the dictionary file and returns the number of words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file: %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';

        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    // Deallocate the memory for inWords
    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }

    return 0;
}

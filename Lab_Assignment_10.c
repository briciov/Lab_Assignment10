#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[26]; // Assuming only lowercase alphabetical characters
    int count; // To keep track of word occurrences
};

// Initializes a trie node
struct TrieNode *createNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node)
    {
        for (int i = 0; i < 26; ++i)
        {
            node->children[i] = NULL;
        }
        node->count = 0;
    }
    return node;
}

// Inserts the word into the trie
void insert(struct TrieNode *root, char *word)
{
    struct TrieNode *node = root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!node->children[index])
        {
            node->children[index] = createNode();
        }
        node = node->children[index];
    }
    node->count++;
}

// Computes the number of occurrences of a word in the trie
int numberOfOccurrences(struct TrieNode *root, char *word)
{
    struct TrieNode *node = root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!node->children[index])
        {
            return 0;
        }
        node = node->children[index];
    }
    return node->count;
}

// Deallocates the trie
void deallocateTrie(struct TrieNode *node)
{
    if (node)
    {
        for (int i = 0; i < 26; ++i)
        {
            deallocateTrie(node->children[i]);
        }
        free(node);
    }
}

// Reads the dictionary from a file and populates the array of words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(1);
    }

    int numWords = 0;
    char word[256];
    while (fgets(word, sizeof(word), file))
    {
        size_t len = strlen(word);
        if (len > 0 && word[len - 1] == '\n')
        {
            word[len - 1] = '\0'; // Remove the newline character
        }
        pInWords[numWords] = strdup(word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *pTrie = createNode();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Words to check for occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    return 0;
}

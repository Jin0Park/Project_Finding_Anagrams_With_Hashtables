#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifndef HASHTABLE_H
#define HASHTABLE_H

/*
 * Hashtables has N number of buckets (LinkedLists)
 * In the LinkedLists, each node is HTKeyValue where
 * it stores key as sorted word and value as a list of
 * words that shares same sorted output.
 * The index of bucket is assigned after calculation with
 * FNVHash64 and FNVHashInt64 functions.
 * If collision occurs, it uses chaining to solve the collision
 * by adding a new HTKeyValue to the LinkedList as a node.
 *
 * Data Structure: 
 * Hashtable has N number of buckets.
 * Each bucket is a LinkedList itself.
 * Each node of a LinkedList is HTKeyValue.
 * HTKeyValue holds key for a sorted string and value for an array of strings that shares a same sorted output.
 */
typedef struct ht_key_value{
    char*   key;    
    char**     value;
    int num_values;
    struct ht_key_value *next;
    struct ht_key_value *prev;
} HTKeyValue, *HTKeyValuePtr;


typedef struct ll_head {
    uint64_t num_nodes;
    HTKeyValuePtr head;
    HTKeyValuePtr tail;
} LinkedListHead;


typedef struct ll_head *LinkedList;

struct hashtableInfo {
    int num_buckets;
    int num_elements;
    LinkedList* buckets;
};

typedef struct hashtableInfo* Hashtable;

/*
 *   Functions for LinkedList and HTKeyValue.
 */

// Removes a tailing \n from a string
char* removeNewLine(char* string);

// Sorts a given string by character
char* SortString(char* string);

// Creates HTKeyValue with given key as a sorted string and given value as a string
HTKeyValue* CreateHTKeyValue(char* k, char* v);

// Appends a new string which shares same sorted output to the given HTKeyValue. The given HTKeyValue must have the shared sorted output as its key.
int AppendValue(HTKeyValue *kv, char* string);

// Searches if the given string is in the HTKeyValue's value array
int SearchValue(HTKeyValue *kv, char* string);

// Deletes the string from the HTKeyValue if exists.
int DeleteValue(HTKeyValue *kv, char* string);

// Prints all values from the HTKeyValue
int PrintHTKeyValue(HTKeyValue *kv);

// Destroys (frees) HTKeyValue
int DestroyHTKeyValue(HTKeyValue *kv);

// Creates HTKeyValue based on the given key and value, and appends to the LinkedList
int AppendHTKeyValue(LinkedList list, char* key, char* value);

// Pops the last HTKeyValue node from the LinkedList
int PopHTKeyValue(LinkedList list, HTKeyValue *kv);

// Creates a LInkedList
LinkedList CreateLinkedList();

// Destroys (frees) a LinkedList
int DestroyLinkedList(LinkedList list);

// Returns a number of nodes in the LinkedList
unsigned int NumElementsInLinkedList(LinkedList list);

/*
 * Functions for Hashtable
 */

// Calculates Load Factor of the hashtable
double GetAlpha(Hashtable ht);

// Get the index of bucket that the string will be stored in the hashtable
uint64_t GetBucketNum(Hashtable ht, char* sortedString);

// Creates a hashtable
Hashtable CreateHashtable(int num_buckets);

// Destroys (frees) hashtable
void DestroyHashtable(Hashtable ht);

// Puts in a given string into corresponding HTKeyValue
// If collision occurs, use chaining to solve it
int PutInHashtable(Hashtable ht, char* string);

// Prints out all values in the hashtable
int PrintAllValues(Hashtable ht);

// Finds anagram(s) for a given string from the hashtable
int FindAnagram(Hashtable ht, char* string);

// Reads a text file and puts the strings into the hashtable using PutInHashtable functions.
int ReadFile(char* filename, Hashtable ht);

// Prints out all anagrams in the hashtable
int GetAllAnagrams(Hashtable ht);

// Finds out if the given string is in the hashtable
int LookupInHashtable(Hashtable ht, char* string);

// Returns total number of elements in the hashtable
int NumElemsInHashtable(Hashtable ht);

// Removes the given string from the hashtable
int RemoveFromHashtable(Hashtable ht, char* string);

// Computes an int from a string, to be used for a key in a HTKeyValue.
// //
// // INPUT:
// //   buffer: a pointer to the array holding the string
// //   len: the length of the string
// //
// // Returns an int to be used as an input to FNVHashInt64 for the hash value.
uint64_t FNVHash64(unsigned char *buffer, unsigned int len);

// Creates a hashed value from a given key.
// //
// // INPUT:
// //   makehash: an int to be hashed for the key of a HTKeyValue pair
// //
// // Returns the int to be used as a key.
uint64_t FNVHashInt64(uint64_t makehash);

#endif  // HASHTABLE_H


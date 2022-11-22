#include "Hashtable.h"
const int MEMORY_FOR_VALUES_ARRAY = 20;

uint64_t FNVHash64(unsigned char *buffer, unsigned int len) {
  static const uint64_t FNV1_64_INIT = 0xcbf29ce484222325ULL;
  static const uint64_t FNV_64_PRIME = 0x100000001b3ULL;
  unsigned char *bp = (unsigned char *) buffer;
  unsigned char *be = bp + len;
  uint64_t hval = FNV1_64_INIT;
  while (bp < be) {
    hval ^= (uint64_t) * bp++;
    hval *= FNV_64_PRIME;
  }
  return hval;
}


uint64_t FNVHashInt64(uint64_t makehash) {
  unsigned char buf[8];
  int i;
  for (i = 0; i < 8; i++) {
    buf[i] = (unsigned char) (makehash & 0x00000000000000FFULL);
    makehash >>= 8;
  }
  return FNVHash64(buf, 8);
}


char* removeNewLine(char* string) {
    int i;
    for (i=0; string[i]!='\0'; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
        }
   }
   return string;
}


char* SortString(char* string) {
    char temp;
    char* sorted = malloc(strlen(string)+1);
    strcpy(sorted, string);
    int i, j;
    for (i=0; i < strlen(string); i++) {
        for (j=0; j<strlen(string)-1; j++) {
            if(sorted[i] < sorted[j]) {
                temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    return sorted;
}


HTKeyValue* CreateHTKeyValue(char* k, char* v) {
    HTKeyValue *kv = (HTKeyValuePtr)malloc(sizeof(HTKeyValue));
    kv->key = k;
    kv->value = (char**)malloc(sizeof(char*)*MEMORY_FOR_VALUES_ARRAY);
    kv->value[0] = v;
    kv->num_values = 1;
    return kv;
}


int AppendValue(HTKeyValue *kv, char* string) {
    if (kv == NULL) {
        return -1;
    }
    int index = kv->num_values;
    kv->value[index] = string;
    kv->num_values += 1;
    return 0;
}


int SearchValue(HTKeyValue *kv, char* string) {
    if (kv == NULL) {
        return -1;
    }
    string = removeNewLine(string);
    HTKeyValue* temp = kv;
    int i;
    for (i=0; i<temp->num_values; i++) {
        if (strcmp(temp->value[i], string) == 0) {
            return 1;
        }
    }
    return 0;
}

 int DeleteValue(HTKeyValue *kv, char* string) {
    if (kv == NULL) {
        return -1;
    }
    HTKeyValue* temp = kv;
    int j;
    int found = 0;
    for (j=0; j<temp->num_values; j++) {
        if (strcmp(temp->value[j], string) == 0) {
            found = 1;
            if (j == temp->num_values - 1) {
                break;
            } else {
                temp->value[j] = temp->value[j+1];
            }
        }
    }
    if (found == 1) {
        temp->value[temp->num_values-1] = NULL;
        temp->num_values -= 1;
        return 0;
    } else {
        return -1;
    }
}


int PrintHTKeyValue(HTKeyValue *kv) {
   if (kv == NULL) {
       return -1;
   }
   int num = kv->num_values;
   int i;
   for (i=0; i<num; i++) {
        printf("%s ", kv->value[i]);
    }
    printf("\n");
    return 0;
}


int DestroyHTKeyValue(HTKeyValue *kv) {
    if (kv == NULL) {
        return -1;
    }
    free(kv);
    return 0;
}

int AppendHTKeyValue(LinkedList list, char* key, char* value) {
    if (list == NULL || key == NULL || value == NULL) {
        return -1;
    }
    HTKeyValuePtr newKV = CreateHTKeyValue(key, value);
    HTKeyValuePtr tailKV = list->tail;
    newKV->prev = tailKV;
    tailKV->next = newKV;
    list->tail = newKV;
    list->num_nodes += 1;
    return 0;
}

int PopHTKeyValue(LinkedList list, HTKeyValue *kv) {
    if (list == NULL || kv == NULL) {
        return -1;
    }
    int i;
    for (i=0; i<list->num_nodes; i++) {
    HTKeyValue* temp = list->head;
        if (temp == kv) {
            if (list->num_nodes == 1) {
                list->head = NULL;
                list->tail = NULL;
                list->num_nodes = 0;
            } else if (list->num_nodes == 2) {
                list->head = list->tail;
                list->num_nodes -= 1;
            } else {
                HTKeyValue* tempPrev = temp->prev;
                HTKeyValue* tempNext = temp->next;
                tempPrev->next = tempNext;
                tempNext->prev = tempPrev;    
                list->num_nodes -= 1;
            }
            free(temp);
            break;
        } else {
            temp = temp->next;
        }
    }    
    return 0;
}


LinkedList CreateLinkedList() {
    LinkedList newList = (LinkedList)malloc(sizeof(LinkedListHead));
    if (newList != NULL) {
        return;
    }
    newList->head = NULL;
    newList->tail = NULL;
    newList->num_nodes = 0;
}


int DestroyLinkedList(LinkedList list) {
    if (list == NULL){
        return -1;
    }
    while(list->head != NULL){
        DestroyHTKeyValue(list->head);
        list->head = list->head->next;
        list->num_nodes -= 1;
    }
    free(list);
    return 0;
}


unsigned int NumElementsInLinkedList(LinkedList list) {
    if (list == NULL) {
        return -1;
    }
    return list->num_nodes;
}


uint64_t GetBucketNum(Hashtable ht, char* sortedString) {
    sortedString = removeNewLine(sortedString);
    uint64_t hashValue = FNVHash64(sortedString, strlen(sortedString));
    int key = hashValue % ht->num_buckets;
    return key;
}

Hashtable CreateHashTable(int num) {
    Hashtable newHash = malloc(sizeof(struct hashtableInfo));
    newHash->num_buckets = num;
    newHash->num_elements = 0;
    newHash->buckets = malloc(sizeof(LinkedListHead)*num);
    int i;
    for (i=0; i<num; i++) {
        newHash->buckets[i] = NULL; 
    }
    return newHash;
}


int PutInHashtable(Hashtable ht, char* string) {
    char* sorted = SortString(string);
    int k = GetBucketNum(ht, sorted);
    LinkedList tempLL = ht->buckets[k];
    int i;
    int match = 0;
    if (tempLL != NULL) {
        HTKeyValue* tempKV = ht->buckets[k]->head;
        while (tempKV != NULL) {
            if (strcmp(tempKV->key, sorted) == 0) {
                if (SearchValue(tempKV, string) == 1) {
                    printf("The value already exists!\n");
                    return -1;
                }
                    AppendValue(tempKV, string);
                    tempLL->num_nodes += 1;
                    match = 1;
                break;
            }
            tempKV = tempKV->next;
        }
        if (match == 0) {
            AppendHTKeyValue(tempLL, sorted, string);
        }
        ht->num_elements += 1;
        return 0;
    } else {
        ht->buckets[k] = CreateLinkedList();
        HTKeyValuePtr newKV = CreateHTKeyValue(sorted, string);
        ht->buckets[k]->head = newKV;
        ht->buckets[k]->tail = newKV;
        ht->buckets[k]->num_nodes = 1;
        ht->num_elements += 1;
        return 0;
    }
    return -1;
}   


int PrintAllValues(Hashtable ht) {
    if (ht == NULL) {
        return -1;
    }
    int count = 0;
    int i;
    for (i=0; i<ht->num_buckets; i++) {
        LinkedList tempLL = ht->buckets[i];
        if (tempLL != NULL){
            HTKeyValue* tempKV = ht->buckets[i]->head;
            while(tempKV != NULL) {
                printf("--The key is %s and bucket number is %d\n", tempKV->key, i);
                PrintHTKeyValue(tempKV);
                count += tempKV->num_values;
                tempKV = tempKV->next;
            }
        }
    }   
    printf("The total number of values are %d\n", count);
    return 0;
    
}

int FindAnagram(Hashtable ht, char* string) {
    if (ht == NULL) {
        return -1;
    }
    char* sorted = SortString(string);
    int k = GetBucketNum(ht, sorted);
    LinkedList tempLL = ht->buckets[k];
    int i;
    int match = 0;
    if (tempLL != NULL) {
        HTKeyValue* tempKV = ht->buckets[k]->head;
        while (tempKV != NULL) {
            if (strcmp(tempKV->key, sorted) == 0) {
                if (tempKV->num_values > 1) {
                    printf("The anagram for %s is ", string);
                    PrintHTKeyValue(tempKV);
                    return 0;
                } else {
                    printf("There is no anagram of %s...\n", string);
                    return 0;
                }
            }
            tempKV = tempKV->next;
        }
    }
    printf("%s doesn't exist in the system\n", string);
    return -1;
}


int ReadFile(char* filename, Hashtable ht) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Unable to open file!");
        return -1;
    }
    struct stat sb;
    stat(filename, &sb);
    char* line = malloc(sb.st_size);
    while (fscanf(fp, "%[^\n] ", line) != EOF) {
        char* curr = strdup(line);
        PutInHashtable(ht, curr);
    }
    fclose(fp);
    return 0;
}


int GetAllAnagrams(Hashtable ht) {
    printf(" ** ALL ANAGRAMS **\n");
    if (ht == NULL) {
        return -1;
    }
    Hashtable temp = ht;
    int i;
    for (i=0; i<temp->num_buckets; i++) {
        LinkedList tempLL = ht->buckets[i];
        if (tempLL != NULL) {
            HTKeyValue* tempKV = ht->buckets[i]->head;
            while (tempKV != NULL) {
                if (tempKV != NULL && tempKV->num_values > 1) {
                    printf("BUCKET NO.: %d KEY: %s  VALUES: ", i, tempKV->key);
                    PrintHTKeyValue(tempKV);
                }
                tempKV = tempKV->next;
            }  
        }
    }
    return 0;
}


int NumElemsInHashtable(Hashtable ht) {
    return ht->num_elements;
}


int LookUpInHashtable(Hashtable ht, char* string) {
    if (ht == NULL || string == NULL) {
        return -1;
    }
    string = removeNewLine(string);
    char* sorted = SortString(string);
    Hashtable temp = ht;
    int bucket_index = GetBucketNum(ht, sorted);
    int i;
    int count = 1;
    LinkedList tempLL = ht->buckets[bucket_index];
    if (tempLL != NULL) {
        HTKeyValue* tempKV = tempLL->head;
        while (tempKV != NULL) {
            if (strcmp(tempKV->key, sorted)==0) {
                printf("%s is in the Hastable in bucket no. %d in node no. %d with key \"%s\"\n", string, bucket_index, count, tempKV->key);
                return 1;
            }
            count++;
            tempKV = tempKV->next;
        }
    }
    printf("%s is not in the hashtable.\n", string);
    return 0; 
}


int RemoveFromHashtable(Hashtable ht, char* string) {
    if (ht == NULL) {
        return -1;
    }
    string = removeNewLine(string);
    char* sorted = SortString(string);
    int bucket_index = GetBucketNum(ht, sorted);
    Hashtable temp = ht;

    int i;
    int removed = -1;
    LinkedList tempLL = ht->buckets[bucket_index];
    if (tempLL != NULL) {
        HTKeyValue* tempKV = ht->buckets[bucket_index]->head;
        printf("%d\n", tempKV->num_values);
        while (tempKV != NULL) {
            if (strcmp(tempKV->key, sorted)==0) {
                removed = DeleteValue(tempKV, string);
                break;
            }
            tempKV = tempKV->next;
        }
    }
    if (ht->buckets[bucket_index]->head->num_values == 0) {
        DestroyLinkedList(ht->buckets[bucket_index]);
    }
    return removed;
}


void DestroyHashtable(Hashtable ht) {
    if (ht == NULL) {
        return;
    }
    int i;
    for (i=0; i<ht->num_buckets; i++) {
        DestroyLinkedList(ht->buckets[i]);
    }
    free(ht);
}

double GetAlpha(Hashtable ht) {
    return (double)((double)ht->num_elements / (double)ht->num_buckets);
}



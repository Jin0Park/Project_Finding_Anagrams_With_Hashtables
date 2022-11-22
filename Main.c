#include "Hashtable.h"
#include "Hashtable.c"

int main() {
    printf("Putting words into a hashtable and will show all the anagrams\n");
    int num_bucket = 400;

    Hashtable h = CreateHashTable(num_bucket);

    ReadFile("words.txt", h);

    GetAllAnagrams(h);
    int i = 33;

//    PrintAllValues(h);
//    printf("\n");
    RemoveFromHashtable(h, "flour");
//    printf("\n");
//    PrintAllValues(h);
    printf("Total number of elements are %d, number of buckets of hashtable is %d, the load factor is %f\n", h->num_elements, h->num_buckets, GetAlpha(h));

    LookUpInHashtable(h, "dog");
    LookUpInHashtable(h, "god");
    LookUpInHashtable(h, "balloon");
    LookUpInHashtable(h, "computer");
    LookUpInHashtable(h, "cat");

    DestroyHashtable(h);

}

#include "gtest/gtest.h"
extern "C" {
    #include "Hashtable.h"
}
const char* first = "first";
const char* second="second";
const char* third = "third";
const char* fourth = "fourth";

#define MAX_VALUE_LEN 75



TEST(Hashtable, Create) {
  Hashtable ht = CreateHashtable(5);
  EXPECT_FALSE(ht == NULL);
  EXPECT_FALSE(ht->buckets == NULL);
  EXPECT_EQ(NumElemsInHashtable(ht), 0);
  DestroyHashtable(ht, NULL);
}

TEST(SortString, GetBucketNum) {
  char* word = "dog";
  char* sorted = SortString(word);
  ASSERT_EQ(sorted, "dgo");

  int num_buckets = 5;
  ASSERT_EQ(GetBucketNum(sorted), 3);
}


TEST(Hashtable, AddOneRemoveOne) {
    Hashtable ht = CreateHashtable(5);
    
    EXPECT_EQ(NumElemsInHashtable(ht), 0);

    char* word = "cat";
    PutInHashtable(ht, word);
    EXPECT_EQ(NumElemsInHashtable(ht), 1);

    RemoveFromHashtable(ht, "cat");

    EXPECT_EQ(NumElemsInHashtable(ht), 0);

    DestroyHashtable(ht);
}

TEST(Hashtable, AddOneElemDestroy) {
    Hashtable ht = CreateHashtable(5);

    EXPECT_EQ(NumElemsInHashtable(ht), 0);

    char* word = "cat";

    PutInHashtable(ht, word);

    EXPECT_EQ(NumElemsInHashtable(ht), 1);

    DestroyHashtable(ht);
}

TEST(Hashtable, AddOneElemTwoTimes) {
    Hashtable ht = CreateHashtable(5);

    EXPECT_EQ(NumElemsInHashtable(ht), 0);

    char* word = "cat";

    int result = PutInHashtable(ht, word);

    EXPECT_EQ(result, 0);
    EXPECT_EQ(NumElemsInHashtable(ht), 1);

    result = PutInHashtable(ht, word);
    EXPECT_EQ(result, -1);
    EXPECT_EQ(NumElemsInHashtable(ht), 1);

    char* new_word = "tac"

    result = PutInHashtable(ht, new_word);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(NumElemsInHashtable(ht), 2);

    DestroyHashtable(ht);
}

TEST(Hashtable, AddOneRemoveTwice) {
    Hashtable ht = CreateHashtable(5);

    EXPECT_EQ(NumElemsInHashtable(ht), 0);

    char* word = "cat";

    int result = PutInHashtable(ht, word);

    EXPECT_EQ(result, 0);
    EXPECT_EQ(NumElemsInHashtable(ht), 1);

    result = RemoveFromHashtable(ht, word);

    EXPECT_EQ(result, 0);
    EXPECT_EQ(NumElemsInHashtable(ht), 0);
 
    result = RemoveFromHashtable(ht, word);
    EXPECT_EQ(result, -1);
    EXPECT_EQ(NumElemsInHashtable(ht), 0);

    DestroyHashtable(ht);
}

TEST(Hashtable, AddMultipleItems) {
    Hashtable ht = CreateHashtable(100);

    EXPECT_EQ(NumElemsInHashtable(ht), 0);

    char* word = "cat";
    PutInHashtable(ht, kv, &old_kv);

    ASSERT_EQ(NumElemsInHashtable(ht), 1);

    char* word1 = "dog";
    PutInHashtable(ht, word1);

    ASSERT_EQ(NumElemsInHashtable(ht), 2);

    char* word2 = "racoon";
    PutInHashtable(ht, word2);

    ASSERT_EQ(NumElemsInHashtable(ht), 3);

    DestroyHashtable(ht);
}


TEST(Hashtable, TwoElemsOneBucket) {
    Hashtable ht = CreateHashtable(5);

    EXPECT_EQ(NumElemsInHashtable(ht), 0);


    char* word = "cat";
    PutInHashtable(ht, word);


    ASSERT_EQ(NumElemsInHashtable(ht), 1);

    char* word1 = "balloon";
    PutInHashtable(ht, word1);

    ASSERT_EQ(NumElemsInHashtable(ht), 2);

    ASSERT_EQ(ht->bucket[4]->num_nodes, 2);
    ASSERT_EQ(ht->bucket[4]->head->value[0], "cat");
    ASSERT_EQ(ht->bucket[4]->tail->value[0], "balloon");

    DestroyHashtable(ht);
}

TEST(Hashtable, LookUpInHashtable) {
    Hashtable ht = CreateHashtable(5);
    
    EXPECT_EQ(NumElemsInHashtable(ht), 0);
    
    char* word = "cat";

    ASSERT_EQ(NumElemsInHashtable(ht), 1);

    char* word1 = "balloon";
    PutInHashtable(ht, word1);

    ASSERT_EQ(NumElemsInHashtable(ht), 2);

    ASSERT_EQ(LookupInHashtable("cat", 1);
    ASSERT_EQ(LookupInHashtable("computer", 0);
    
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

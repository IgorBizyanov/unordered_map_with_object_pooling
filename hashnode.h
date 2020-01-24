#pragma once
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include "pool.h"
#include <functional>

using namespace std;

template <typename KeyType, typename ValueType, typename F = hash<KeyType>>
class HashMap {

public:

   HashMap();

   virtual ~HashMap();

   int size() const;

   bool isEmpty() const;

   void put(KeyType key, ValueType value);

   ValueType get(KeyType key) const;

   void remove(KeyType key);

   void clear();

   ValueType & operator[](KeyType key);
   ValueType operator[](KeyType key) const;

private:

   static const int INITIAL_BUCKET_COUNT = 101;
   static const int MAX_LOAD_PERCENTAGE = 50;


   struct Cell {
      KeyType key;
      ValueType value;
      Cell *next;
   };


   F hashCode;
   ObjectPool<Cell> pool;
   vector<Cell*> buckets;
   int nBuckets;
   int numEntries;

   void createBuckets(int nBuckets) {
      if (nBuckets == 0) nBuckets = 1;
      buckets = vector<Cell *>(nBuckets, NULL);
      this->nBuckets = nBuckets;
      numEntries = 0;
   }
 
   void deleteBuckets(vector <Cell *> & buckets) {
      for (int i = 0; i < buckets.size(); i++) {
         Cell* cp = buckets[i];
         while (cp) {
            Cell *np = cp->next;
            cp = NULL;
            cp = np;
         }
         buckets[i] = NULL;
      }
   }

   void expandAndRehash() {
      vector<Cell *>oldBuckets = buckets;
      createBuckets(oldBuckets.size() * 2 + 1);
      for (int i = 0; i < oldBuckets.size(); i++) {
         for (Cell *cp = oldBuckets[i]; cp != NULL; cp = cp->next) {
            put(cp->key, cp->value);
         }
      }
      deleteBuckets(oldBuckets);
   }

   Cell *findCell(int bucket, KeyType key) const {
      Cell *dummy;
      return findCell(bucket, key, dummy);
   }

   Cell *findCell(int bucket, KeyType key, Cell * & parent) const {
      parent = NULL;
      Cell *cp = buckets[bucket];
      while (cp != NULL && key != cp->key) {
         parent = cp;
         cp = cp->next;
      }
      return cp;
   }

   void deepCopy(const HashMap & src) {
      createBuckets(src.nBuckets);
      for (int i = 0; i < src.nBuckets; i++) {
         for (Cell *cp = src.buckets.get(i); cp != NULL; cp = cp->next) {
            put(cp->key, cp->value);
         }
      }
   }

};

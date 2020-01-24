#pragma once
#include <iostream>
#include "hashnode.h"

using namespace std;


template <typename KeyType,typename ValueType,typename F>
HashMap<KeyType,ValueType,F>::HashMap() {
   createBuckets(INITIAL_BUCKET_COUNT);
}

template <typename KeyType,typename ValueType,typename F>
HashMap<KeyType,ValueType,F>::~HashMap() {
   deleteBuckets(buckets);
}

template <typename KeyType,typename ValueType,typename F>
int HashMap<KeyType,ValueType,F>::size() const {
   return numEntries;
}

template <typename KeyType,typename ValueType,typename F>
bool HashMap<KeyType,ValueType,F>::isEmpty() const {
   return size() == 0;
}

template <typename KeyType,typename ValueType,typename F>
void HashMap<KeyType,ValueType,F>::put(KeyType key, ValueType value) {
   (*this)[key] = value;
}

template <typename KeyType,typename ValueType,typename F>
ValueType HashMap<KeyType,ValueType,F>::get(KeyType key) const {
   Cell *cp = findCell(hashCode(key) % nBuckets, key);
   if (cp == NULL) return ValueType();
   return cp->value;
}

template <typename KeyType,typename ValueType,typename F>
void HashMap<KeyType,ValueType,F>::remove(KeyType key) {
   int bucket = hashCode(key) % nBuckets;
   Cell *parent;
   Cell *cp = findCell(bucket, key, parent);
   if (cp != NULL) {
      if (parent == NULL) {
         buckets[bucket] = cp->next;
      } else {
         parent->next = cp->next;
      }

      pool.deleteObject(cp); 
      numEntries--;
   }
}


template <typename KeyType,typename ValueType,typename F>
void HashMap<KeyType,ValueType,F>::clear() {
   deleteBuckets(buckets);
   numEntries = 0;
}

template <typename KeyType,typename ValueType,typename F>
ValueType & HashMap<KeyType,ValueType,F>::operator[](KeyType key) {
   int bucket = hashCode(key) % nBuckets;
   Cell *cp = findCell(bucket, key);
   if (cp == NULL) {
      if (numEntries > MAX_LOAD_PERCENTAGE * nBuckets / 100.0) {
         expandAndRehash();
         bucket = hashCode(key) % nBuckets;
      }
      cp = pool.createNewObject();     
      cp->key = key;
      cp->value = ValueType();
      cp->next = buckets[bucket];
      buckets[bucket] = cp;
      numEntries++;
   }
   return cp->value;
}


template <typename KeyType, typename ValueType,typename F>
ValueType HashMap<KeyType,ValueType,F>::operator[](KeyType key) const {
   return get(key);
}

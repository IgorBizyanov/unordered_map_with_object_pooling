#include <iostream>
#include "hashmap.h"

using namespace std;

struct Date {
   int month, year;
};


bool operator==(const Date& l, const Date& r) {
   return l.year == r.year && l.month == r.month;
}

bool operator!=(const Date& l, const Date& r) {
   return !(l == r);
}
int main() {
   HashMap<int, Date> hm;
   Date d = {2, 2020};
   Date d1 = {2, 2020};
   Date d2 = {3, 2021};

   hm[1] = d;
   hm[2] = d1;
   hm[500] = d2;
   
   hm.remove(1);
   hm.remove(2);
   hm.remove(500);

   
   return 0;
}
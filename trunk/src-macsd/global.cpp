#include "global.h"

using namespace std;

#if VERSION == V_WWW
  
  bool operator<(const CANDIDATE& a, const CANDIDATE& b) {return ((a.first < b.first) or ((a.first <= b.first) and (a.second < b.second)) or ((a.first <= b.first) and (a.second <= b.second) and (a.third < b.third)));};

#endif

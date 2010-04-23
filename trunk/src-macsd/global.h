// List of global declarations

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <string>
#include <utility>
#include <iostream>

using namespace std;

// *** VERSION of the code
// Constants
#define V_SHAPE 1		// Shape version
#define V_GO 2			// Gene Ontology version
#define V_SCIENCEMAP 3		// Visual Science Map version
#define V_WWW 4			// WWW version

// *** Definition of the tuplax3 type
template <typename T>
class tuplax3 {
    public:
        tuplax3() {};
        tuplax3(T a, T b, T c) {first = a; second = b; third = c;};
        bool operator==(const tuplax3<T>& o) const {return ((first == o.first) && (second == o.second) && (third == o.third));};
	tuplax3& operator=(const tuplax3<T>& o) { first = o.first; second = o.second; third = o.third; return *this;};
        
        T first;
        T second;
        T third;
};

template<typename T>
bool operator< (const tuplax3<T>& a, const tuplax3<T>& b) {
    return ((a.first < b.first) || ((a.first == b.first) && (a.second < b.second)) || ((a.first == b.first) && (a.second == b.second) && (a.third < b.third)));
};

// *** Definition of the CANDIDATE type
#if VERSION == V_SHAPE
	typedef pair<unsigned int, string> CANDIDATE;
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
	template <typename T> class tuplax3;
	typedef tuplax3<unsigned int> CANDIDATE;
#elif VERSION == V_WWW
	class CANDIDATE {
	  public:
	  unsigned int first;
	  unsigned int second;
	  string third;
	  
	  CANDIDATE() {};
	  CANDIDATE(const CANDIDATE& b) {first = b.first; second = b.second; third = b.third;};
	  CANDIDATE(unsigned int a, unsigned int b, string c) : first(a), second(b), third(c) {};
	  
  	  bool operator==(const CANDIDATE& b) const {CANDIDATE a = *this; cout << "R" << endl; return ((a.first == b.first) and (a.second == b.second) and (a.third == b.third));};
	  bool operator!=(const CANDIDATE& b) const {return !(*this == b);};
	  CANDIDATE& operator=(const CANDIDATE& b) {first = b.first; second = b.second; third = b.third; return *this;};
	};
	
	 bool operator<(const CANDIDATE& a, const CANDIDATE& b);
#endif


// *** Definition of the SOLUTION type
#if VERSION == V_SHAPE
//	class shapes;
//	typedef shapes SOLUTION;
	#define SOLUTION shapes
#elif VERSION == V_GO
//	#include "go/shapes.h"
//	#include "go/ontologia.h"
//	class go;
//	typedef pair<string,go> SOLUTION;
	#define SOLUTION go
#elif VERSION == V_SCIENCEMAP
	#define SOLUTION vmap
#elif VERSION == V_WWW
	#define SOLUTION www
#endif

#endif // __GLOBAL_H__

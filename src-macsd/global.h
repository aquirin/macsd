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
        bool operator==(const tuplax3<T>& o) const {return ((first == o.first) and (second == o.second) and (third == o.third));};
	bool operator!=(const tuplax3<T>& o) const {return !(*this == o);};
	tuplax3& operator=(const tuplax3<T>& o) { first = o.first; second = o.second; third = o.third; return *this;};
        
        T first;
        T second;
        T third;
};

template<typename T>
bool operator< (const tuplax3<T>& a, const tuplax3<T>& b) {
    return ((a.first < b.first) or ((a.first == b.first) and (a.second < b.second)) or ((a.first == b.first) and (a.second == b.second) and (a.third < b.third)));
};

// *** Definition of the CANDIDATE type
typedef tuplax3<unsigned int> CANDIDATE;

// *** Definition of the SOLUTION type
#if VERSION == V_SHAPE
	#define SOLUTION shapes	
#elif VERSION == V_GO
	#define SOLUTION go
	#define NO_ISOMORFISM 5
#elif VERSION == V_SCIENCEMAP
	#define SOLUTION vmap
	#define NO_ISOMORFISM 5
#elif VERSION == V_WWW
	#define SOLUTION www
#endif

#endif // __GLOBAL_H__

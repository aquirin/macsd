// List of global declarations

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <string>
#include <utility>
//#include "utils.h"
using namespace std;


// *** VERSION of the code
// Constants
#define V_SHAPE 1		// Shape version
#define V_GO 2			// Gene Ontology version
#define V_SCIENCEMAP 3		// Visual Science Map version


// *** Definition of the tuplax3 type
template <typename T>
class tuplax3 {
    public:
        tuplax3() {};
        tuplax3(T a, T b, T c) {first = a; second = b; third = c;};
        bool operator==(const tuplax3<T>& o) const {return ((first == o.first) && (second == o.second) && (third == o.third));};
        
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
#endif


// *** Definition of the SOLUTION type
#if VERSION == V_SHAPE
//	class shapes;
//	typedef shapes SOLUTION;
	#define SOLUTION shapes
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
//	#include "go/shapes.h"
//	#include "go/ontologia.h"
//	class go;
//	typedef pair<string,go> SOLUTION;
	#define SOLUTION go
#endif

#endif // __GLOBAL_H__

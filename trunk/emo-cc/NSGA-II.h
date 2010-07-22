/***************************************************************************
 *   Copyright (C) 2008 by Rocio Celeste Romero Zaliz   *
 *   rromero@gemini   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <string>
#include <vector>
#include <list>
#include "termination_criteria.h"
#include "triple.h"
#include "global.h"
#include <iostream>
#include <algorithm>

#if VERSION == V_SHAPE
#include "shapes.h"
#elif (VERSION == V_GO)
#include "ontologia.h"
#elif (VERSION == V_SCIENCEMAP)
#include "vmap.h"
#elif (VERSION == V_WWW)
#include "www.h"
#endif

using namespace std;

template <class G, class H>
bool crowding(const triple<G,double,double> &me, const triple<G,double,double> &other) {
	return ((me.third < other.third) or ((me.third == other.third) and (me.second > other.second)));
}

template <class G, class H>
class nsgaii {
    private:
		class population {
                    private:
                        vector< triple<G,double,double> > _data;

                    public:
                        population() {};
                        ~population();

                        // make_from_object
                        population(const vector<G>& base);
                        // make_random
                        population(const vector<H>& base);
                        // copia
                        population(const population& v) : _data(v._data) {};

                        population operator=(const population& v) {_data = v._data; return *this;};

                        bool operator==(const population& other) const;

                        G any() const;
                        bool is_empty() {return _data.empty();};
                        bool is_dominated(G &v);
                        unsigned int size() const {return _data.size();};

                        void purge_repeated();
                        void purge_invalid();
                        void purge_dominated();
                        void clear() {_data.clear();};

                        typedef typename vector< triple<G,double,double> >::iterator iterator;
                        typedef typename vector< triple<G,double,double> >::const_iterator const_iterator;
                        iterator begin() {return _data.begin();};
                        iterator end() {return _data.end();};
                        const_iterator begin() const {return _data.begin();};
                        const_iterator end() const {return _data.end();};

                        const_iterator find(const triple<G,double,double> & v) const;
                        void push_back(const triple<G,double,double> & v) {_data.push_back(v);};

                        // Genetic operations
                        void selection();
                        void crossover(const unsigned int &cross, const bool &species = false);
                        void mutation(const unsigned int &mut);
                        void binary_tournament(const int &cross, const int &mut, const bool &species = false);

                        string show();
                };

               /* ostream & operator<<(ostream & os, const population& p) {
                  os << "Population:" << endl;
                  unsigned int i = 1;
                  for (typename population::const_iterator q = p.begin(); q != p.end(); q++, i++) {
                    stringstream ss;
                    ss << i;
                    os << "Chromosome: " << ss.str() << endl << *q << "out" << ss.str() << endl;
                //    (*q).save("out" + ss.str());
                  }
                  return os;
                }*/

                static unsigned int _N;
		static termination_criteria _criteria;
		static unsigned int _niche;

                static vector<H> _db;

	public:
		// Constructor and destructors
		nsgaii() {};
		~nsgaii() {};
		
		nsgaii(const vector<H> &database, const int &tampop, const int &nic, termination_criteria &cri);
		
		// Auxiliary functions
		vector< list< triple<G,double,double> > > FastNonDominatedSort(const list< triple<G,double,double> > &v);
		list< triple<G,double,double> > CrowdingDistanceAssignment(const list< triple<G,double,double> > &v);
		list<G> execute(int iter, string name);
		list< triple<G,double,double> > MakeNewPop(const list< triple<G,double,double> > & v);
};

#include "NSGA-II.template"

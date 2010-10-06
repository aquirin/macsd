
#ifndef _EMOCC_SOLUTION
#define _EMOCC_SOLUTION
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

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "termination_criteria.h"
#include "global.h"

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

template <class G>
class emocc_solution {
	private:
		bool _costeValido;
		vector<float> _costes;
		int _niche;
		termination_criteria _criteria;
                G _subestructura;
                static vector<G> _instancias;
                vector<unsigned int> _support;
                unsigned int _numObjetivos;

	public:
		emocc_solution() {};
		emocc_solution(const vector<G>& base, termination_criteria &cri, const unsigned int numObjetivos, const int& nic);
                emocc_solution(const vector<G>& base, termination_criteria &cri, const unsigned int numObjetivos, const int& nic, G sub);
		emocc_solution(const emocc_solution& other);

		bool operator==(const emocc_solution& other) const {return (_subestructura == other._subestructura);};
		bool operator!=(const emocc_solution& other) const {return !((*this) == other);}

		unsigned int size() const {return _subestructura.size();};
		vector<unsigned int> support() {if (!_costeValido) calculaCostes(); return _support;}
		void calculaCostes();
                float getCoste(const unsigned int id);
                int dominancia(emocc_solution<G>& v, bool x, int y);

		double max(int i) const {return 1.;}
		double min(int i) const {return (i == 1) ? 0 : 1.;}
		emocc_solution<G> crossover(const emocc_solution<G>& other, bool species);
		emocc_solution<G> mutation();
		bool better(const emocc_solution<G> &other, const int &obj) const {return (_costes[obj] < other._costes[obj]);}
		string show() const;
                bool valid() {calculaCostes(); return ((getCoste(1) != 0) and (getCoste(0) != 0));}
};

template <class G>
ostream& operator<< (ostream& os, const emocc_solution<G>& yo);

#include "emocc_solution.template"

#endif
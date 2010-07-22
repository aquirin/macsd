#ifndef _TRIPLE_
#define _TRIPLE_

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

#include <iostream>

using namespace std;

template <class G, class H, class T>
class triple;

template <class G, class H, class T>
ostream& operator<< (ostream& os, const triple<G,H,T>& yo);

template <class G, class H, class T>
class triple {
	public:
		triple() {};
		triple(const G &a, const H &b, const T &c) {first = a; second = b; third = c;};
		triple(G &a, H &b, T &c) {first = a; second = b; third = c;};

		G first;
		H second;
		T third;

		bool operator==(const triple<G,H,T>& other) const {return (first == other.first) and (second == other.second) and (third == other.third);};
		bool operator!=(const triple<G,H,T>& other) const {return !(this==other);};
                bool operator<(const triple<G,H,T>& other) const {return !(third < other.third);};

		friend ostream& operator<< <G,H,T>(ostream& os, const triple<G,H,T>& yo);
};

template <class G, class H, class T>
ostream& operator<< (ostream& os, const triple<G,H,T>& yo) {
  os << yo.first << endl;
  os << yo.second << endl;
  os << yo.third << endl;
  return os;
}

#endif
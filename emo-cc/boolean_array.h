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

using namespace std;

class boolean_array : public vector<bool> {
	private:
	public:
		boolean_array() {};
		boolean_array(const size_t &max) {reserve(max);};
		boolean_array(const vector<bool> &v) {*this = v;};
		boolean_array(const size_t &max, const bool& b) {for (unsigned int i = 0; i < max; i++) push_back(b);};
		unsigned int count_true() const {
			int cant = 0;
			for (unsigned int i = 0; i < size(); i++)
				cant += (((*this)[i]) ? 1 : 0);
			return cant;
		}
		unsigned int count_false() const {return size() - count_true();}
		bool subsumed_by(const boolean_array &v);
		boolean_array ba_union(const boolean_array &v) const;
		boolean_array ba_inter(const boolean_array &v) const;
};

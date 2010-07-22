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

#include "boolean_array.h"

using namespace std;

bool boolean_array::subsumed_by(const boolean_array &v) {
	bool res = (count_true() <= v.count_true());
	for (unsigned int i = 0; (i < size()) and res; i++) {
		res = (((*this)[i]) ? v[i] : true);
	}

	return res;
}

boolean_array boolean_array::ba_union(const boolean_array &v) const {
	boolean_array sol(size());
	for (unsigned int i = 0; i < size(); i++) {
		sol[i] = ((*this)[i] or v[i]);
	}
	return sol;
}

boolean_array boolean_array::ba_inter(const boolean_array &v) const {
	boolean_array sol(size());
	for (unsigned int i = 0; i < size(); i++) {
		sol[i] = ((*this)[i] and v[i]);
	}
	return sol;
}

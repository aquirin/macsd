#ifndef _TERMINATION_CRITERIA_
#define _TERMINATION_CRITERIA_

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

#include <time.h>
#include <iostream>
#include <string>
using namespace std;

class termination_criteria {
	public:
		termination_criteria() {};
		termination_criteria(const int &v) { _actual = time(NULL); _limit = v;};
		bool finish() {time_t fin = time(NULL); cout << difftime(fin,_actual) << endl; return (difftime(fin,_actual) > _limit);};
	private:
		int _limit;
		time_t _actual;
};

#endif
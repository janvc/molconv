/*
 * Copyright 2014 Jan von Cosel
 *
 * This file is part of molconv.
 *
 * molconv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * molconv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have recieved a copy of the GNU General Public License
 * along with molconv. If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include"atom.h"
#include"atom_properties.h"


atom::atom(int at_num, Eigen::Vector3d pos)
{
	this->atomicnumber = at_num;

	this->position = pos;

	this->mass = atomprops[at_num-1].mass;
	this->atomicsymbol = atomprops[at_num-1].symbol;
}


void atom::shift(Eigen::Vector3d shift_vector)
{
	this->position += shift_vector;
}


int atom::get_atomicnumber()
{
	return this->atomicnumber;
}


std::string atom::get_atomicsymbol()
{
	return this->atomicsymbol;
}


double atom::get_atomicmass()
{
	return this->mass;
}


Eigen::Vector3d atom::get_position()
{
	return this->position;
}


double atom::get_x()
{
	return this->position(0);
}


double atom::get_y()
{
	return this->position(1);
}


double atom::get_z()
{
	return this->position(2);
}

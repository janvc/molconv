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


namespace molconv
{
	/*
	 * The constructor of the atom class.
	 */
	atom::atom(int at_num, Eigen::Vector3d pos)
	{
		this->atomicnumber = at_num;

		this->position = pos;

		this->mass = atomprops[at_num-1].mass;
		this->atomicsymbol = atomprops[at_num-1].symbol;
	}


	/*
	 * This function shifts the atom about a specified shift vector.
	 */
	void atom::shift(Eigen::Vector3d shift_vector)
	{
		this->position += shift_vector;
	}


	/*
	 * This function returns the atomic number of an atom.
	 */
	int atom::get_atomicnumber()
	{
		return this->atomicnumber;
	}


	/*
	 * This function returns the atomic symbol of an atom.
	 */
	std::string atom::get_atomicsymbol()
	{
		return this->atomicsymbol;
	}


	/*
	 * This function returns the mass of an atom.
	 */
	double atom::get_atomicmass()
	{
		return this->mass;
	}


	/*
	 * This function returns the internal position vector of an atom.
	 */
	Eigen::Vector3d atom::get_int_position()
	{
		return this->position;
	}


	/*
	 * This function returns the x-value of an atoms internal position.
	 */
	double atom::get_int_x()
	{
		return this->position(0);
	}


	/*
	 * This function returns the y-value of an atoms internal position.
	 */
	double atom::get_int_y()
	{
		return this->position(1);
	}


	/*
	 * This function returns the z-value of an atoms internal position.
	 */
	double atom::get_int_z()
	{
		return this->position(2);
	}


	/*
	 * This function transforms the position vector of an atom by
	 * multiplying it with a matrix.
	 */
	void atom::transform(Eigen::Matrix3d tmatrix)
	{
		this->position = tmatrix * this->position;
	}

}

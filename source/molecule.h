/*
 * Copyright 2014 Jan von Cosel
 *
 * This file is part of molconv.
 *
 * molconv if free software: you can redistribute it and/or modify
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


#ifndef MOLECULE_H_
#define MOLECULE_H_


#include<vector>
#include<string>


class atom
{
public:
	atom();                                      		// standard constructor (everything set to zero)
	atom(int at_num, double x, double y, double z);		// specify type and position of atom
	void shift_x(double dx);							// move the atom in x-direction
	void shift_y(double dy);							// move the atom in y-direction
	void shift_z(double dz);							// move the atom in z-direction

private:
	int atomicnumber;

	double x_pos;
	double y_pos;
	double z_pos;
};


class molecule
{
public:
	molecule(int number_of_atoms);
	molecule(std::ifstream input_file);					// read a molecular structure from an xyz file
	void shift_x(double dx);							// move the molecule in x-direction
	void shift_y(double dy);							// move the molecule in y-direction
	void shift_z(double dz);							// move the molecule in z-direction

private:
	std::string comment_line;		// the comment line between the atom number and the coordinates
	int number_of_atoms;			// the number of atoms in the molecule
	std::vector<atom> theatoms;		// the atoms
};



#endif /* MOLECULE_H_ */

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
#include<eigen3/Eigen/Dense>


class atom
{
public:
	atom(int at_num, Eigen::Vector3d pos);		// specify type and position of atom
	void shift(Eigen::Vector3d shift_vector);	// move the atom
	int get_atomicnumber();						// return the atomic number
	std::string get_atomicsymbol();				// return the atomic symbol
	double get_atomicmass();					// return the mass
	Eigen::Vector3d get_position();				// return the position as a vector
	double get_x();								// return the x-position
	double get_y();								// return the y-position
	double get_z();								// return the z-position
private:
	int atomicnumber;
	std::string atomicsymbol;
	double mass;

	Eigen::Vector3d position;	// the position in the internal molecular coordinate system
};


class molecule
{
public:
	molecule(const char *input_file);			// read a molecular structure from an xyz file
	void shift(Eigen::Vector3d shift_vector);	// move the molecule
	void print_stdout();						// print the structur to std out
	std::string get_commentline();
	void show_info();							// print info about the molecule to stdout

private:
	int number_of_atoms;				// the number of atoms in the molecule
	std::string comment_line;			// the comment line between the atom number and the coordinates

	std::vector<atom> theatoms;			// the atoms

	double mass;						// the total mass of the molecule
	Eigen::Vector3d internal_origin;	// origin of the internal coordinate system in terms of the absolute coordinates
	Eigen::Vector3d center_of_mass;		// position of the center of mass
	Eigen::Vector3d center_of_geometry;	// like the center of mass, but without the mass weighting
	Eigen::Matrix3d inertia_tensor;		// tensor of the moments of inertia

	void calc_mass();					// calculate the total mass of the molecule
	void calc_com();					// calculate the center of mass
	void calc_cog();					// calculate the center of geometry
	void calc_inertia();				// calculate the tensor of moments of inertia
	void diag_inertia();				// diagonalize the tensor of inertia
};



#endif /* MOLECULE_H_ */

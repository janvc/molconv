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


#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include"molecule.h"
#include"atom_properties.h"
#include"utilities.h"


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


molecule::molecule(const char *input_file)
{
	std::ifstream input(input_file);
	std::string line;
	int lines_read = 0;
	while (getline(input, line))
	{
		lines_read++;
		std::istringstream iss(line);
		std::string atomsymbol_dummy;
		double dummy_x, dummy_y, dummy_z;
		switch (lines_read)
		{
		case 1:
			iss >> this->number_of_atoms;
			break;
		case 2:
			this->comment_line = line;
			break;
		default:
			iss >> atomsymbol_dummy >> dummy_x >> dummy_y >> dummy_z;
			std::cout << "line number: " << lines_read << " " << atomsymbol_dummy
					  << " " << dummy_x << " " << dummy_y << " " << dummy_z << std::endl;

			int atomicnumber_dummy = symbol2number(atomsymbol_dummy);
			Eigen::Vector3d position_dummy(dummy_x, dummy_y, dummy_z);

			this->theatoms.push_back(atom(atomicnumber_dummy, position_dummy));
			break;
		}
	}
}


void molecule::shift(Eigen::Vector3d shift_vector)
{
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		atiter->shift(shift_vector);
	}
}


void molecule::print_stdout()
{
	std::cout << "The number of atoms is: " << this->number_of_atoms << std::endl;
	std::cout << this->comment_line << std::endl;
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		std::cout << atiter->get_atomicsymbol() << "  "
				  << atiter->get_x() << "  " << atiter->get_y() << "  " << atiter->get_z() << std::endl;
	}
}


std::string molecule::get_commentline()
{
	return this->comment_line;
}

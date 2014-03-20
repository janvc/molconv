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
#include"configuration.h"


atom::atom()
{
	atomicnumber = 0;

	x_pos = 0.0;
	y_pos = 0.0;
	z_pos = 0.0;
}


atom::atom(int at_num, double x, double y, double z)
{
	atomicnumber = at_num;

	x_pos = x;
	y_pos = y;
	z_pos = z;
}


void atom::shift_x(double dx)
{
	this->x_pos += dx;
}


void atom::shift_y(double dy)
{
	this->y_pos += dy;
}


void atom::shift_z(double dz)
{
	this->z_pos += dz;
}

int atom::get_atomicnumber()
{
	return this->atomicnumber;
}

double atom::get_x()
{
	return this->x_pos;
}

double atom::get_y()
{
	return this->y_pos;
}

double atom::get_z()
{
	return this->z_pos;
}


molecule::molecule(const char *input_file)
{
//    try
//    {
//        std::ifstream input(input_file);
//
//        input >> this->number_of_atoms;
//        std::cout << this->number_of_atoms << std::endl;
//        getline(input, this->comment_line);
//        std::cout << this->comment_line << std::endl;
//
//        std::string atomsymbol_dummy;
//        double dummy_x, dummy_y, dummy_z;
//
//        while (input >> atomsymbol_dummy >> dummy_x >> dummy_y >> dummy_z)
//        {
//        	std::cout << atomsymbol_dummy << "  " << dummy_x << "  " << dummy_y << "  " << dummy_z << std::endl;
//        	this->theatoms.push_back(atom(1, dummy_x, dummy_y, dummy_z));
//        }
//    }
//
//    catch (...)
//    {
//        std::cerr << "There was a problem reading the input file " << std::endl;
//    }
	std::ifstream input(input_file);
	std::string line;
	int lines_read = 0;
	while (getline(input, line))
	{
		lines_read++;
		std::cout << line << std::endl;
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
			break;
		}
	}
}


void molecule::shift_x(double dx)
{
	for (int i = 0; i < this->number_of_atoms; i++)
	{
		this->theatoms.at(i).shift_x(dx);
	}
}


void molecule::shift_y(double dy)
{
	for (int i = 0; i < this->number_of_atoms; i++)
	{
		this->theatoms.at(i).shift_y(dy);
	}
}


void molecule::shift_z(double dz)
{
	for (int i = 0; i < this->number_of_atoms; i++)
	{
		this->theatoms.at(i).shift_z(dz);
	}
}

void molecule::print_stdout()
{
	std::cout << this->number_of_atoms << std::endl;
	//std::cout <<  "# this is a test-strucure" << std::endl;
	std::cout << this->comment_line << std::endl;
	for (int i = 1; i < this->number_of_atoms; i++)
	{
		std::cout << this->theatoms.at(i+1).get_x() << this->theatoms.at(i+1).get_y() << this->theatoms.at(i+1).get_z() << std::endl;
	}
}

std::string molecule::get_commentline()
{
	return this->comment_line;
}

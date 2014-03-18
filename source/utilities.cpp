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
#include"utilities.h"


void print_header()
{
	std::cout << std::string(72,'-') << std::endl
			  << "    molconv development version     Copyright 2014 by Jan von Cosel" << std::endl
			  << std::endl
			  << "    molconv is free software." << std::endl
			  << "    You can redistribute and/or modify it under the terms" << std::endl
			  << "    of the GNU General Public License." << std::endl
			  << std::endl
			  << "    molconv comes with absolutely no warranty." << std::endl
			  << std::string(72,'-') << std::endl
			  << std::endl;
}


void print_help_msg()
{
	print_header();

	std::cout << "    Command options for molconv:" << std::endl
			  << std::endl
			  << "        -h             print this help text" << std::endl
			  << "        -i [filename]  name of the input file" << std::endl
			  << "        -o [filename]  name of the output file" << std::endl
			  << std::endl;
}


//std::ifstream open_input_file(std::string inputfile)
//{
//	// open the input file:
//	std::ifstream input(inputfile, std::ios::in);
//	if (!input.good())
//	{
//		std::cerr << "Error: could not open the input file " << inputfile << std::endl;
//	}
//
//	// read some dummy variables to see if the format is correct:
//	std::string input_line;
//	int dummy_Natoms;
//	std::string dummy_commentline;
//	std::string dummy_atomsymbol;
//	double dummy_x, dummy_y, dummy_z;
//
//	int line_number = 0;
//	while (std::getline(input, input_line))
//	{
//		line_number++;
//		std::istringstream input_stream(input_line);
//
//		if (line_number == 1)	// this should be the number of atoms
//		{
//			if (!(input_stream >> dummy_Natoms && input_stream.eof()))
//				std::cerr << "There was an error reading the number of atoms from input file " << inputfile << std::endl;
//		}
//		else if (line_number == 2)	// this should be the comment line
//		{
//			if (!(input_stream >> dummy_commentline && input_stream.eof()))
//				std::cerr << "There was an error reading the comment line from input file " << inputfile << std::endl;
//		}
//		else
//		{
//			if (!(input_stream >> dummy_atomsymbol >> dummy_x >> dummy_y >> dummy_z && input_stream.eof()))
//				std::cerr << "There was an error reading the atomic coordinates from line " << line_number
//				          << " of input file " << inputfile << std::endl;
//		}
//	}
//
//}

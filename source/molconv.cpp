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


#include<iostream>
#include<fstream>
#include"utilities.h"
#include"configuration.h"
#include"molecule.h"


int main(int argc, char *argv[])
{
	configuration input_paras(argc, argv);

	if (input_paras.help_wanted())
	{
		print_help_msg();
		return 0;
	}
	else
		print_header();


	if (input_paras.input_exists())
	{
	    molecule testmolecule(input_paras.get_inputfile().c_str());

	    if (input_paras.output_exists())
	    	testmolecule.write_to_file(input_paras.get_outputfile().c_str());
	}

	return 0;
}


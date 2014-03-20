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
#include<unistd.h>
#include"configuration.h"


configuration::configuration(int argc, char *argv[])
{
	// first set the default values:
	this->help_flag = false;
	this->input_flag = false;
	this->input_file = "";

	int option;
	opterr = 0;

	// now get the command options:
	while ((option = getopt(argc, argv, "hi:")) != -1)
	{
		switch (option)
		{
		case 'h':
			this->help_flag = true;
			break;
		case 'i':
			this->input_flag = true;
			this->input_file = optarg;
			break;
		case '?':
			if (optopt == 'i')
				std::cout << "There is no input file." << std::endl;
			else if (isprint(optopt))
				std::cout << "Unknown option: " << optopt << std::endl;
			else
				std::cout << "WTF?" << std::endl;
			break;
		default:
			break;
		}
	}
}


bool configuration::help_wanted()
{
	return this->help_flag;
}


bool configuration::input_exists()
{
	return this->input_flag;
}


std::string configuration::get_inputfile()
{
	return this->input_file;
}
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
#include"utilities.h"
#include"configuration.h"


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
		std::cout << "The input file is: " << input_paras.get_inputfile() << std::endl;

	return 0;
}

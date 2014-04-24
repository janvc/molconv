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


//#include"exceptions.h"
#include"utilities.h"
#include"molecule.h"
#include"configuration.h"


int main(int argc, char *argv[])
{
	print_header();

	molconv::configuration config(argc, argv);

	if (config.help_wanted())
	{
		config.print_help();
		return 0;
	}

	if (config.input_exists())
	{
		molconv::molecule testmolecule(config.get_input(0).c_str(), &config);

		if (config.cleanup_wanted())
			testmolecule.clean_up();

		if (config.output_exists())
			testmolecule.write_to_file(config.get_output().c_str());

		return 0;
	}
}


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


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include<boost/program_options.hpp>

namespace molconv
{
	class configuration
	{
	public:
		configuration(int argc, char *argv[]);		// the constructor that will parse the command line
		bool help_wanted();
		bool cleanup_wanted();
		bool input_exists();
		bool output_exists();
		int get_Nofinputs();
		std::string get_input(int index);
		std::string get_output();
		void print_help();
	private:
		std::vector<std::string> inputfiles;		// the input files
		std::string outputfile;						// the output file
		boost::program_options::options_description opt_desc;
		boost::program_options::variables_map var_map;

		bool help_flag;
		bool cleanup_flag;
		bool input_flag;
		bool output_flag;
	};
} /* namespace molconv */
#endif /* CONFIGURATION_H_ */

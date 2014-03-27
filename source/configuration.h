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


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_


#include<string>


class configuration
{
public:
	configuration(int argc, char *argv[]);
	bool help_wanted();
	bool cleanup_wanted();
	bool input_exists();
	bool output_exists();
	std::string get_inputfile();
	std::string get_outputfile();
private:
	bool help_flag;
	bool input_flag;
	bool output_flag;
	bool clean_flag;
	std::string input_file;
	std::string output_file;
};


#endif /* CONFIGURATION_H_ */

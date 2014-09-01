/*
 * Copyright 2014 Jan von Cosel & Sebastian Lenz
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
#include<sstream>
#include<string>
#include"utilities.h"


void print_header()
{
    std::cout << std::string(72,'-') << std::endl
              << "    molconv development version       Copyright 2014 by Jan von Cosel" << std::endl
              << "                                                      & Sebastian Lenz" << std::endl
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
    std::cout << "USAGE: molconv [OPTIONS] inputfile [file ..]" << std::endl
              << std::endl
              << "Options:" << std::endl
              << std::endl
              << "   -h             display help text" << std::endl
              << "   -o <filename>  output file" << std::endl

              << std::endl;
}

void print_err_header()
{
    print_header();
    std::cout << std::string(45,'-') << std::endl
              << "    ############### ERROR ###############" << std::endl;
}

void print_err_footer()
{
    std::cerr << std::string(45,'-') << std::endl << std::endl;
    print_help_msg();
}

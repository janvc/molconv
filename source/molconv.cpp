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
#include<boost/program_options.hpp>
#include<boost/filesystem.hpp>
#include"utilities.h"
#include"configuration.h"
#include"molecule.h"

// Set namespace po for easier use of program_options
namespace po = boost::program_options;

namespace
{
  const size_t ERROR_IN_COMMAND_LINE = 1;
  const size_t SUCCESS = 0;
  const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace

int main(int argc, char *argv[])
{

	std::string inputfile = "";
	std::string appName = boost::filesystem::basename(argv[0]);

	// Declare the supported options.
	po::options_description opts;
	opts.add_options()
		("help,h", "Display help message")
	;

    po::options_description hidden("Hidden options");
    hidden.add_options()
		("inputfile",po::value< std::vector<std::string> >()->required(), "input file")
    ;

	po::positional_options_description positionalOptions;
	positionalOptions.add("inputfile", -1);

    po::options_description cmdline_opts;
    cmdline_opts.add(opts).add(hidden);

    po::options_description visible("Options");
    visible.add(opts);

	po::variables_map vm;

	try
	{
		po::store(po::command_line_parser(argc, argv).options(cmdline_opts)
					.positional(positionalOptions).run(),
				  vm); // throws on error

		if( vm.count("help") )
		{
            print_help_msg(appName);
			return SUCCESS;
		}

		po::notify(vm);
	}
	catch(boost::program_options::required_option& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		return ERROR_IN_COMMAND_LINE;
	}
	catch(boost::program_options::error& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		return ERROR_IN_COMMAND_LINE;
	}




	//    configuration input_paras(argc, argv);

	//    if (input_paras.help_wanted())
	//    {
	//    	print_help_msg();
	//    	return 0;
	//    }
	//    else
	//    	print_header();


	//    if (input_paras.input_exists())
	//    {
	//        molecule testmolecule(input_paras.get_inputfile().c_str());

	//        if (input_paras.output_exists())
	//        	testmolecule.write_to_file(input_paras.get_outputfile().c_str());
	//    }

	return 0;
}


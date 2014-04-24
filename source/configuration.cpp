/*
 * configuration.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: jan
 */

#include"configuration.h"

namespace molconv
{
	configuration::configuration(int argc, char *argv[])
	{
		// set the default values:
		this->help_flag = false;
		this->cleanup_flag = false;
		this->input_flag = false;
		this->output_flag = false;

		// here are the options:
		this->opt_desc.add_options()
			("help,h", "print this help text")
			("input,i", boost::program_options::value< std::vector<std::string> >(&inputfiles), "input file(s) to be read")
			("output,o", boost::program_options::value<std::string>(&outputfile), "output, where the structure will be written to")
			("cleanup,c", "clean up the structure")
		;

		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt_desc), var_map);
		boost::program_options::notify(var_map);

		// set the flags according to the cmd options:
		if (this->var_map.count("help"))
			this->help_flag = true;

		if (this->var_map.count("input"))
			this->input_flag = true;

		if (this->var_map.count("cleanup"))
			this->cleanup_flag = true;

		if (this->var_map.count("output"))
			this->output_flag = true;
	}


	bool configuration::help_wanted()
	{
		return this->help_flag;
	}


	bool configuration::cleanup_wanted()
	{
		return this->cleanup_flag;
	}


	bool configuration::input_exists()
	{
		return this->input_flag;
	}


	bool configuration::output_exists()
	{
		return this->output_flag;
	}


	int configuration::get_Nofinputs()
	{
		return this->inputfiles.size();
	}


	std::string configuration::get_input(int index)
	{
		return this->inputfiles.at(index);
	}


	std::string configuration::get_output()
	{
		return this->outputfile;
	}


	void configuration::print_help()
	{
		std::cout << "Usage:" << std::endl;
		std::cout << this->opt_desc << std::endl;
	}
} /* namespace molconv */

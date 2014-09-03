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
        this->gui_flag = false;
        this->input_flag = false;
        this->output_flag = false;

        // here are the options:
        this->opt_desc.add_options()
            ("help,h", "print this help text")
            ("input,i", boost::program_options::value< std::vector<std::string> >(&this->inputfiles)->multitoken(), "input file(s) to be read")
            ("output,o", boost::program_options::value<std::string>(&this->outputfile), "output, where the structure will be written to")
            ("cleanup,c", "clean up the structure")
            ("gui,g", "start the GUI")
            ("origin", boost::program_options::value< std::vector<std::string> >(&this->origin_string)->multitoken(),
                    "specify the origin of the internal coordinate system: com, cog, atom X")
            ("axes", boost::program_options::value< std::vector<std::string> >(&this->axes_string)->multitoken(),
                    "specify the internal coordinate axes: inert, covar, atoms X Y Z")
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

        if ((this->var_map.count("gui")) || (argc == 1))
            this->gui_flag = true;

        if (this->var_map.count("output"))
            this->output_flag = true;

        if (this->var_map.count("origin"))
        {
            this->origin_flag = true;

            if (this->origin_string.size() == 1)    // only one value: either com or cog
            {
                if (this->origin_string.at(0) == "com")
                    this->internal_origin_type = 1;
                else if (this->origin_string.at(0) == "cog")
                    this->internal_origin_type = 2;
                else
                    std::cerr << "ERROR: Invalid value of option 'origin'" << std::endl;
            }
            else if (this->origin_string.size() == 2)    // two values: 'atom' + the number of the atom
            {
                if (this->origin_string.at(0) == "atom")
                {
                    this->internal_origin_type = 3;

                    std::istringstream iss(this->origin_string.at(1));
                    iss >> this->origin_atom;
                }
                else
                    std::cerr << "ERROR: Invalid value of option 'origin'" << std::endl;
            }
            else
                std::cerr << "ERROR: Wrong number of values of option 'origin'" << std::endl;
        }

        if (this->var_map.count("axes"))
        {
            this->axes_flag = true;

            if (this->axes_string.size() == 1)    // only one value: either inert or covar
            {
                if (this->axes_string.at(0) == "inert")
                    this->internal_coords_type = 1;
                else if (this->axes_string.at(0) == "covar")
                    this->internal_coords_type = 2;
                else
                    std::cerr << "ERROR: Invalid value of option 'axes'" << std::endl;
            }
            else if (this->axes_string.size() == 4)    // two values: 'atoms' + the number of the atom
            {
                if (this->axes_string.at(0) == "atoms")
                {
                    this->internal_coords_type = 3;

                    for (std::vector<std::string>::const_iterator iter = this->axes_string.begin()+1; iter != this->axes_string.end(); iter++)
                    {
                        std::istringstream iss(*iter);
                        int atomnumber;
                        iss >> atomnumber;
                        this->axes_atoms.push_back(atomnumber);
                    }
                }
                else
                    std::cerr << "ERROR: Invalid value of option 'axes'" << std::endl;
            }
            else
                std::cerr << "ERROR: Wrong number of values of option 'axes'" << std::endl;
        }
    }


    bool configuration::help_wanted() const
    {
        return this->help_flag;
    }


    bool configuration::cleanup_wanted() const
    {
        return this->cleanup_flag;
    }


    bool configuration::gui_wanted() const
    {
        return this->gui_flag;
    }

    bool configuration::input_exists() const
    {
        return this->input_flag;
    }


    bool configuration::output_exists() const
    {
        return this->output_flag;
    }


    bool configuration::origin_exists() const
    {
        return this->origin_flag;
    }

    bool configuration::axes_exist() const
    {
        return this->axes_flag;
    }

    int configuration::get_orig_type() const
    {
        return this->internal_origin_type;
    }

    int configuration::get_axes_type() const
    {
        return this->internal_coords_type;
    }

    int configuration::get_orig_atom() const
    {
        return this->origin_atom;
    }

    std::vector<int> configuration::get_axes_atoms() const
    {
        return this->axes_atoms;
    }

    int configuration::get_Nofinputs() const
    {
        return this->inputfiles.size();
    }


    std::string configuration::get_input(int index) const
    {
        return this->inputfiles.at(index);
    }


    std::string configuration::get_output() const
    {
        return this->outputfile;
    }


    void configuration::print_help() const
    {
        std::cout << "Usage:" << std::endl;
        std::cout << this->opt_desc << std::endl;
    }
} /* namespace molconv */

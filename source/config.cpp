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

#include"config.h"
#include"types.h"

namespace molconv {
    Config::Config(int argc, char *argv[])
    {
        // set the default values:
        this->help_flag_ = false;
        this->cleanup_flag_ = false;
        this->gui_flag_ = false;
        this->input_flag_ = false;
        this->output_flag_ = false;
        this->origin_type_ = kCenterOnZero;
        this->axes_type_ = kIdentityVectors;
        std::vector<std::string> origin_string;
        std::vector<std::string> axes_string;

        // here are the options:
        this->opt_desc.add_options()
            ("help,h", "print this help text")
            ("input,i", boost::program_options::value< std::vector<std::string> >(&this->inputfiles_)->multitoken(), "input file(s) to be read")
            ("output,o", boost::program_options::value<std::string>(&this->outputfile_), "output, where the structure will be written to")
            ("cleanup,c", "clean up the structure")
            ("gui,g", "start the GUI")
            ("origin", boost::program_options::value< std::vector<std::string> >(&origin_string)->multitoken(),
                    "specify the origin of the internal coordinate system: com, cog, atom X")
            ("axes", boost::program_options::value< std::vector<std::string> >(&axes_string)->multitoken(),
                    "specify the internal coordinate axes: inert, covar, atoms X Y Z")
        ;

        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt_desc_), var_map_);
        boost::program_options::notify(var_map_);

        // set the flags according to the cmd options:
        if (this->var_map_.count("help"))
            this->help_flag_ = true;

        if (this->var_map_.count("input"))
            this->input_flag_ = true;

        if (this->var_map_.count("cleanup"))
            this->cleanup_flag_ = true;

        if ((this->var_map_.count("gui")) || (argc == 1))
            this->gui_flag_ = true;

        if (this->var_map_.count("output"))
            this->output_flag_ = true;

        if (this->var_map_.count("origin"))
        {
            this->origin_flag_ = true;

            if (this->origin_string.size() == 1)    // only one value: either com or cog
            {
                if (this->origin_string.at(0) == "com")
                    this->origin_type_ = kCenterOfMass;
                else if (this->origin_string.at(0) == "cog")
                    this->origin_type_ = kCenterOfGeometry;
                else
                    std::cerr << "ERROR: Invalid value of option 'origin'" << std::endl;
            }
            else if (this->origin_string.size() == 2)    // two values: 'atom' + the number of the atom
            {
                if (this->origin_string.at(0) == "atom")
                {
                    this->origin_type = kCenterOnAtom;

                    std::istringstream iss(origin_string.at(1));
                    iss >> this->origin_atom_;
                    this->origin_atom_--;    // reduce the read-in number by one because C++ begins counting at 0, not 1
                }
                else
                    std::cerr << "ERROR: Invalid value of option 'origin'" << std::endl;
            }
            else
                std::cerr << "ERROR: Wrong number of values of option 'origin'" << std::endl;
        }

        if (this->var_map_.count("axes"))
        {
            this->axes_flag_ = true;

            if (axes_string.size() == 1)    // only one value: either inert or covar
            {
                if (axes_string.at(0) == "inert")
                    this->axes_type_ = kInertiaVectors;
                else if (this->axes_string.at(0) == "covar")
                    this->axes_type_ = kCovarianceVectors;
                else
                    std::cerr << "ERROR: Invalid value of option 'axes'" << std::endl;
            }
            else if (axes_string.size() == 4)    // four values: 'atoms' + the numbers of the atoms
            {
                if (axes_string.at(0) == "atoms")
                {
                    this->axes_type_ = kVectorsFromAtoms;

                    for (std::vector<std::string>::const_iterator iter = axes_string.begin()+1; iter != axes_string.end(); iter++)
                    {
                        std::istringstream iss(*iter);
                        int atomnumber;
                        iss >> atomnumber;
                        this->axes_atoms_.push_back(atomnumber);
                        this->axes_atoms_.back()--;  // reduce the read-in number by one because C++ begins counting at 0, not 1
                    }
                }
                else
                    std::cerr << "ERROR: Invalid value of option 'axes'" << std::endl;
            }
            else
                std::cerr << "ERROR: Wrong number of values of option 'axes'" << std::endl;
        }
    }


    bool Config::help_wanted() const
    {
        return this->help_flag_;
    }


    bool Config::cleanup_wanted() const
    {
        return this->cleanup_flag_;
    }


    bool Config::gui_wanted() const
    {
        return this->gui_flag_;
    }

    bool Config::input_exists() const
    {
        return this->input_flag_;
    }


    bool Config::output_exists() const
    {
        return this->output_flag_;
    }


    bool Config::origin_exists() const
    {
        return this->origin_flag_;
    }

    bool Config::axes_exist() const
    {
        return this->axes_flag_;
    }

    int Config::origin_type() const
    {
        return this->origin_type_;
    }

    int Config::axes_type() const
    {
        return this->axes_type_;
    }

    int Config::origin_atom() const
    {
        return this->origin_atom_;
    }

    std::vector<int> Config::axes_atoms() const
    {
        return this->axes_atoms_;
    }

    int Config::get_NumberOfInputs() const
    {
        return this->inputfiles_.size();
    }


    std::string Config::get_input(int index) const
    {
        return this->inputfiles_.at(index);
    }


    std::string Config::get_output() const
    {
        return this->outputfile_;
    }


    void Config::print_help() const
    {
        std::cout << "Usage:" << std::endl;
        std::cout << this->opt_desc_ << std::endl;
    }
} // namespace molconv

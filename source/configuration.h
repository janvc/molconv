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

#ifndef Q_MOC_RUN
    #include<boost/program_options.hpp>
#endif

namespace molconv
{
    class configuration
    {
    public:
        configuration(int argc, char *argv[]);  // the constructor that will parse the command line
        bool help_wanted() const;
        bool cleanup_wanted() const;
        bool gui_wanted() const;
        bool input_exists() const;
        bool output_exists() const;
        bool origin_exists() const;
        bool axes_exist() const;
        int get_Nofinputs() const;
        int get_orig_type() const;
        int get_orig_atom() const;
        int get_axes_type() const;
        std::vector<int> get_axes_atoms() const;
        std::string get_input(int index) const;
        std::string get_output() const;
        void print_help() const;
    private:
        int internal_origin_type;               // location of internal origin: 1 - com, 2 - cog, 3 - atom
        int internal_coords_type;               // type of internal coodinate axes: 1 - inert, 2 - covar, 3 - atoms
        int origin_atom;                        // atom to be used as the internal origin
        std::vector<int> axes_atoms;            // atoms used to define the internal coordinate system
        std::vector<std::string> inputfiles;    // the input files
        std::string outputfile;                 // the output file
        std::vector<std::string> origin_string;
        std::vector<std::string> axes_string;
        boost::program_options::options_description opt_desc;
        boost::program_options::variables_map var_map;

        bool help_flag;
        bool cleanup_flag;
        bool gui_flag;
        bool input_flag;
        bool output_flag;
        bool origin_flag;
        bool axes_flag;
    };
} /* namespace molconv */
#endif /* CONFIGURATION_H_ */

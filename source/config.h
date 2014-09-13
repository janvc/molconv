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


#ifndef CONFIG_H_
#define CONFIG_H_

#ifndef Q_MOC_RUN
    #include<boost/program_options.hpp>
#endif

namespace molconv {
    class Config
    {
    public:
        Config(int argc, char *argv[]);  // the constructor that will parse the command line
        bool help_wanted() const;
        bool cleanup_wanted() const;
        bool gui_wanted() const;
        bool input_exists() const;
        bool output_exists() const;
        bool origin_exists() const;
        bool axes_exist() const;

        int get_Nofinputs() const;
        origin origin_type() const;
        int origin_atom() const;
        basis axes_type() const;
        std::vector<int> axes_atoms() const;
        std::string inputfile(int index) const;
        std::string outputfile() const;
        void help_message() const;
    private:
        bool help_flag_;
        bool cleanup_flag_;
        bool gui_flag_;
        bool input_flag_;
        bool output_flag_;
        bool origin_flag_;
        bool axes_flag_;

        origin origin_type_;                     // location of origin
        basis axes_type_;                        // type of internal coodinate axes
        int origin_atom_;                        // atom to be used as the internal origin
        std::vector<int> axes_atoms_;            // atoms used to define the internal coordinate system
        std::vector<std::string> inputfiles_;    // the input files
        std::string outputfile_;                 // the output file
        boost::program_options::options_description opt_desc_;
        boost::program_options::variables_map var_map_;
    };
} // namespace molconv
#endif // CONFIG_H_

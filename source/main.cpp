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

#include<iostream>
#include<boost/make_shared.hpp>
#include"utilities.h"
#include"molecule.h"
#include"configuration.h"
#include"molconv_window.h"


int main(int argc, char *argv[])
{
    print_header();

    molconv::Config config(argc, argv);

    if (config.help_wanted())
    {
        config.print_help();
        return 0;
    }

    QApplication app(argc, argv);
    MolconvWindow the_window;

    if (config.input_exists())
    {

        for (int i = 0; i < config.get_NumberOfInputs(); i++)
        {
            the_window.openFile(QString::fromStdString(config.get_input(i)));
            the_window.set_intbasis(i, config);
            the_window.clean_up(i, config);
            the_window.add_molecule();
        }

        if (config.output_exists())
        {
            the_window.saveFile(QString::fromStdString(config.get_output()));
        }
    }

    if (config.gui_wanted())
    {
        std::cout << "GUI wanted." << std::endl;
        the_window.show();

        return app.exec();
    }
    return 0;
}

/*
 * Copyright 2014 - 2016 Jan von Cosel & Sebastian Lenz
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

#include "molconv_window.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Molconv");
    QCoreApplication::setOrganizationDomain("molconv.org");
    QCoreApplication::setApplicationName("molconv");

    QApplication app(argc, argv);
    MolconvWindow the_window;
    the_window.showMaximized();

    if (app.arguments().size() > 1)
        for (int i = 1; i < app.arguments().size(); i++)
            the_window.openFileDefault(app.arguments()[i]);

    return app.exec();
}

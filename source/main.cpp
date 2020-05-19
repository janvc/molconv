/*
 * Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz
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

#include "molconvwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Molconv");
    QCoreApplication::setOrganizationDomain("molconv.org");
    QCoreApplication::setApplicationName("molconv");
    QCoreApplication::setApplicationVersion("1.0.1");

    QApplication app(argc, argv);
    MolconvWindow the_window;

    QSettings settings(QApplication::applicationDirPath() + "/molconv-settings.ini", QSettings::IniFormat);

    if (settings.value("startMaximized").toBool())
        the_window.showMaximized();
    else
    {
        int windowWidth = settings.contains("winW") ? settings.value("winW").toInt() : 640;
        int windowHeight = settings.contains("winH") ? settings.value("winH").toInt() : 480;
        int xPos = settings.contains("xPos") ? settings.value("xPos").toInt() : 0;
        int yPos = settings.contains("yPos") ? settings.value("yPos").toInt() : 0;
        the_window.setGeometry(xPos, yPos, windowWidth, windowHeight);
        the_window.show();
    }

    if (app.arguments().size() > 1)
        for (int i = 1; i < app.arguments().size(); i++)
            the_window.openFile(app.arguments()[i]);

    return app.exec();
}

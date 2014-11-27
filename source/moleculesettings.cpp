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


#include<QDebug>
#include "moleculesettings.h"
#include "ui_moleculesettings.h"


MoleculeSettings::MoleculeSettings(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::MoleculeSettings)
{
    qDebug("this is the constructor of MoleculeSettings");

    main_window = window;

    ui->setupUi(this);

    connect(main_window, SIGNAL(new_molecule(molconv::moleculePtr)), SLOT(setValues(molconv::moleculePtr)));
}

MoleculeSettings::~MoleculeSettings()
{
    qDebug("this is the destructor of MoleculeSettings");
    delete ui;
}

void MoleculeSettings::setValues(const molconv::moleculePtr the_molecule)
{
}

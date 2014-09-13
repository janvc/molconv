/*
 * Copyright 2014 Sebastian Lenz
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



#include"molecules_dock.h"
#include"ui_molecules_dock.h"

#include"molconv_window.h"


ListOfMolecules::ListOfMolecules(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::ListOfMolecules) {

    main_window = window;

    ui->setupUi(this);
    connect(window, SIGNAL(new_molecule(molconv::Molecule*)), SLOT(show_item(molconv::Molecule*)));
}

ListOfMolecules::~ListOfMolecules() {
    delete ui;
}

void ListOfMolecules::show_item(molconv::Molecule *molecule) {
    ui->molecule_list->addItem(QString::fromStdString(molecule->formula()));
}

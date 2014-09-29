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



#include "molecules_dock.h"
#include "ui_molecules_dock.h"

#include "molconv_window.h"


ListOfMolecules::ListOfMolecules(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::ListOfMolecules) {

    main_window = window;

    ui->setupUi(this);
    connect(window, SIGNAL(new_molecule(molconv::Molecule*)), SLOT(list_new_molecule(molconv::Molecule*)));

    checkboxMapper = new QSignalMapper(this);
}

ListOfMolecules::~ListOfMolecules() {
    delete ui;
}
void ListOfMolecules::list_new_molecule(molconv::Molecule *molecule) {
    ui->molecule_settings->setRowCount(ui->molecule_settings->rowCount()+1);

    int index = ui->molecule_settings->rowCount()-1;

    QCheckBox *new_checkbox = new QCheckBox(this);
    new_checkbox->setChecked(true);
    connect(new_checkbox,SIGNAL(clicked()),checkboxMapper,SLOT(map()));
    checkboxMapper->setMapping(new_checkbox,index);
    connect(checkboxMapper,SIGNAL(mapped(int)),this,SLOT(checkbox_toggled(int)));

    ui->molecule_settings->setItem(index,0,new QTableWidgetItem(QString::fromStdString(molecule->formula())));
    ui->molecule_settings->setCellWidget(index,1,new_checkbox);
}

void ListOfMolecules::checkbox_toggled(int index)
{
    bool state = static_cast<QCheckBox*>(ui->molecule_settings->cellWidget(index,1))->isChecked();
    main_window->toggle_molecule(index,state);
}

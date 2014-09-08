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



#include"molecule_list.h"
#include"ui_molecule_list.h"

#include"molconv_window.h"


molecule_list::molecule_list(molconv_window *window)
    : QDockWidget(window)
    , ui(new Ui::molecule_list)
{
    m_window = window;

    ui->setupUi(this);
    connect(window, SIGNAL(new_molecule(molconv::Molecule*)), SLOT(show_item(molconv::Molecule*)));
}

molecule_list::~molecule_list()
{
    delete ui;
}

void molecule_list::show_item(molconv::Molecule *molecule)
{
    // clear list:
    //ui->mol_list->clear();

    ui->mol_list->addItem(QString::fromStdString(molecule->formula()));
}

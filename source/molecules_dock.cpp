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


#include<QDebug>
#include "molecules_dock.h"
#include "ui_molecules_dock.h"

#include "molconv_window.h"


ListOfMolecules::ListOfMolecules(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::ListOfMolecules)
{
    qDebug("this is the constructor of ListOfMolecules");

    main_window = window;

    ui->setupUi(this);

    ui->molecule_settings->setHeaderLabels(QStringList() << "Name" << "Formula" << "Mass [u]" << "Visible");

    connect(window, SIGNAL(new_molecule(molconv::Molecule*)), SLOT(list_new_molecule(molconv::Molecule*)));
}

ListOfMolecules::~ListOfMolecules()
{
    qDebug("this is the destructor of ListOfMolecules");
    delete ui;
}
void ListOfMolecules::list_new_molecule(molconv::Molecule *molecule)
{
    qDebug("entering ListOfMolecules::list_new_molecule()");

    QTreeWidgetItem *theItem = new QTreeWidgetItem();

    theItem->setText(0, QString::fromStdString(molecule->name()));
    theItem->setText(1, QString::fromStdString(molecule->formula()));
    theItem->setText(2, QString::number(molecule->mass()));

    theItem->setFlags(theItem->flags() | Qt::ItemIsUserCheckable);
    theItem->setCheckState(3, Qt::Checked);
    m_items.append(theItem);

    connect(ui->molecule_settings, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(checkbox_toggled(QTreeWidgetItem*)));
    ui->molecule_settings->insertTopLevelItems(0, m_items);
}

void ListOfMolecules::checkbox_toggled(QTreeWidgetItem *item)
{
    qDebug("entering ListOfMolecules::checkbox_toggled()");

    bool state;

    if (item->checkState(1) == Qt::Checked)
        state = true;
    else
        state = false;

    int index = m_items.indexOf(item);

    main_window->toggle_molecule(index,state);
}

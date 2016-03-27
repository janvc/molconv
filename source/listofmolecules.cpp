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


#include<QDebug>
#include "listofmolecules.h"
#include "ui_listofmolecules.h"

#include "molconv_window.h"
#include "moleculeitem.h"
#include "groupitem.h"


ListOfMolecules::ListOfMolecules(MolconvWindow *window, molconv::sysPtr &newSystem)
    : QDockWidget(window)
    , ui(new Ui::ListOfMolecules)
{
    qDebug("this is the constructor of ListOfMolecules");

    main_window = window;
    //m_model = new MoleculeListModel(newSystem);

    ui->setupUi(this);

    ui->system_tree->setModel(newSystem.get());
}

ListOfMolecules::~ListOfMolecules()
{
    qDebug("this is the destructor of ListOfMolecules");
    delete ui;
}
/*
void ListOfMolecules::addMolecule(molconv::moleculePtr &theMolecule)
{
    qDebug("entering ListOfMolecules::addMolecule()");
}

molconv::moleculePtr ListOfMolecules::getSelectedMolecule() const
{
    qDebug("entering ListOfMolecules::getSelectedMolecule()");
}

void ListOfMolecules::addGroup(molconv::groupPtr &theGroup)
{
    qDebug("entering ListOfMolecules::list_new_group()");
}

void ListOfMolecules::checkbox_toggled(QTreeWidgetItem *theItem)
{
    qDebug("entering ListOfMolecules::checkbox_toggled()");
}
*/

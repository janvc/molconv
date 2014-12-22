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
#include "listofmolecules.h"
#include "ui_listofmolecules.h"

#include "molconv_window.h"
#include "moleculeitem.h"
#include "groupitem.h"


ListOfMolecules::ListOfMolecules(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::ListOfMolecules)
{
    qDebug("this is the constructor of ListOfMolecules");

    main_window = window;

    ui->setupUi(this);

    ui->system_tree->setHeaderLabels(QStringList() << "Name" << "Formula" << "Mass [u]" << "Visible");

    QMenu *contextMenu = new QMenu(ui->system_tree);
    ui->system_tree->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *addElement = new QAction("add element", contextMenu);
    QAction *deleteElement = new QAction("delete element", contextMenu);
    ui->system_tree->addAction(addElement);
    ui->system_tree->addAction(deleteElement);
}

ListOfMolecules::~ListOfMolecules()
{
    qDebug("this is the destructor of ListOfMolecules");
    delete ui;
}

void ListOfMolecules::list_new_molecule(molconv::moleculePtr theMolecule)
{
    qDebug("entering ListOfMolecules::list_new_molecule()");

    MoleculeItem *theItem = new MoleculeItem(theMolecule);

    theItem->setText(0, QString::fromStdString(theMolecule->name()));
    theItem->setText(1, QString::fromStdString(theMolecule->formula()));
    theItem->setText(2, QString::number(theMolecule->mass()));

    theItem->setFlags(theItem->flags() | Qt::ItemIsUserCheckable);
    theItem->setCheckState(3, Qt::Checked);

    connect(ui->system_tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(checkbox_toggled(QTreeWidgetItem*)));

    ui->system_tree->addTopLevelItem(theItem);
}

molconv::moleculePtr ListOfMolecules::getSelectedMolecule() const
{
    qDebug("entering ListOfMolecules::getSelectedMolecule()");

    MoleculeItem *selectedMolecule = dynamic_cast<MoleculeItem*>(ui->system_tree->currentItem());

    return selectedMolecule->getMolecule();
}

void ListOfMolecules::list_new_group(boost::shared_ptr<molconv::abstractMoleculeGroup> theGroup)
{
    qDebug("entering ListOfMolecules::list_new_group()");

    GroupItem *theItem = new GroupItem(theGroup);

    theItem->setText(0, QString::fromStdString(theGroup->name()));

    ui->system_tree->addTopLevelItem(theItem);
}

void ListOfMolecules::checkbox_toggled(QTreeWidgetItem *theItem)
{
    qDebug("entering ListOfMolecules::checkbox_toggled()");

    MoleculeItem *molItem = dynamic_cast<MoleculeItem*>(theItem);
    bool state;

    if (theItem->checkState(3) == Qt::Checked)
        state = true;
    else
        state = false;

    main_window->toggle_molecule(molItem->getMolecule(),state);
}

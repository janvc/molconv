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
#include "moleculelistmodel.h"
#include "groupitem.h"


ListOfMolecules::ListOfMolecules(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::ListOfMolecules)
{
    ui->setupUi(this);
    m_window = window;

    QStringList headers;
    headers << tr("Name") << tr("Number of atoms") << tr("Mass");

    MoleculeListModel *model = new MoleculeListModel(headers);

    ui->system_tree->setModel(model);
    for (int column = 0; column < model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);
}

ListOfMolecules::~ListOfMolecules()
{
    delete ui;
}

void ListOfMolecules::insertMolecule(molconv::moleculePtr &newMol)
{
    QModelIndex index = ui->system_tree->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->system_tree->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
        return;

    for (int column = 0; column < model->columnCount(index.parent()); column++)
    {
        QModelIndex child = model->index(index.row() + 1, column, index.parent());

        switch (column)
        {
        case 0:
            model->setData(child, QVariant(QString::fromStdString(newMol->name())), Qt::EditRole);
            break;
        case 1:
            model->setData(child, QVariant(int(newMol->size())), Qt::EditRole);
            break;
        case 2:
            model->setData(child, QVariant(newMol->mass()), Qt::EditRole);
            break;
        default:
            break;
        }
    }

    for (int column = 0; column < model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);
}

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


#include "listofmolecules.h"
#include "moleculeitem.h"
#include "groupitem.h"
#include "ui_listofmolecules.h"

#include "molconv_window.h"


ListOfMolecules::ListOfMolecules(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::ListOfMolecules)
    , m_window(window)
    , m_model(new QStandardItemModel)
{
    ui->setupUi(this);

    QStringList headers;
    headers << tr("Show") << tr("Name") << tr("Number of atoms") << tr("Formula") << tr("Mass");

    ui->system_tree->setModel(m_model);

    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(toggleMolecule(QModelIndex)));
    connect(ui->system_tree->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                                                SLOT(changeSelectedItem(QModelIndex)));

    m_model->setHorizontalHeaderLabels(headers);

    for (int column = 0; column < m_model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);

}

ListOfMolecules::~ListOfMolecules()
{
    delete ui;
}

void ListOfMolecules::insertMolecule(molconv::moleculePtr &newMol)
{
    QList<QStandardItem *> items;
    for (int i = 0; i < 5; i++)
    {
        QStandardItem *item = new MoleculeItem(newMol, i);
        if (i == 0)
        {
            item->setCheckable(true);
            item->setCheckState(Qt::Checked);
        }
        items << item;
    }

    m_model->appendRow(items);
    for (int column = 0; column < m_model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);

    newMol->setListItem(static_cast<MoleculeItem *>(items.front()));

    // automatically select the molecule that was last added to make it clear, which
    // molecule is currently being edited
    ui->system_tree->selectionModel()->select(items.front()->index(),
                                  QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void ListOfMolecules::insertGroup(molconv::MoleculeGroup *newGroup)
{
    QList<QStandardItem *> items;
    for (int i = 0; i < 5; i++)
    {
        QStandardItem *item = new GroupItem(newGroup, i);
        if (i == 0)
        {
            item->setCheckable(true);
            item->setCheckState(Qt::Checked);
        }
        items << item;
    }
    m_model->appendRow(items);

    for (int i = 0; i < newGroup->nMolecules(); i++)
    {
        QStandardItem *current = newGroup->getMol(i)->listItem();
        QList<QStandardItem *> rowItems = m_model->invisibleRootItem()->takeRow(current->row());
        items.front()->appendRow(rowItems);
    }

    ui->system_tree->expand(items.front()->index());

    for (int column = 0; column < m_model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);
}

void ListOfMolecules::removeCurrentMolecule()
{
    QModelIndex index = ui->system_tree->selectionModel()->currentIndex();
    m_model->removeRow(index.row(), index.parent());
}

molconv::moleculePtr ListOfMolecules::currentMolecule()
{
    QModelIndex index = ui->system_tree->selectionModel()->currentIndex();
    MoleculeItem *currentItem = static_cast<MoleculeItem *>(m_model->itemFromIndex(index));

    return currentItem->Molecule();
}

void ListOfMolecules::toggleMolecule(const QModelIndex &index)
{
    bool state = false;
    if (m_model->itemFromIndex(index)->checkState() == Qt::Checked)
        state = true;

    // find out if the item that was clicked is a molecule or a group:
    if (m_model->itemFromIndex(index)->type() == QStandardItem::UserType + 1)       // molecule
    {
        MoleculeItem *currentItem = static_cast<MoleculeItem *>(m_model->itemFromIndex(index));
        m_window->toggle_molecule(currentItem->Molecule(), state);
    }
    else if (m_model->itemFromIndex(index)->type() == QStandardItem::UserType + 2)  // group
    {
        GroupItem *currentItem = static_cast<GroupItem *>(m_model->itemFromIndex(index));
        molconv::MoleculeGroup *currentGroup = currentItem->Group();

        for (int i = 0; i < currentGroup->nMolecules(); i++)
        {
            m_window->toggle_molecule(currentGroup->getMol(i), state);
            if (state)
                currentItem->child(i)->setCheckState(Qt::Checked);
            else
                currentItem->child(i)->setCheckState(Qt::Unchecked);
        }
    }
}

void ListOfMolecules::changeSelectedItem(const QModelIndex &current)
{
    if (current.column() > 0)
    {
        if (m_model->itemFromIndex(current)->type() == QStandardItem::UserType + 1)         // molecule
        {
            MoleculeItem *currentItem = static_cast<MoleculeItem *>(m_model->itemFromIndex(current));
            molconv::moleculePtr tmp = currentItem->Molecule();
            emit newMoleculeSelected(tmp);
        }
        else if (m_model->itemFromIndex(current)->type() == QStandardItem::UserType + 2)    // group
        {
            GroupItem *currentItem = static_cast<GroupItem *>(m_model->itemFromIndex(current));
            molconv::MoleculeGroup *tmp = currentItem->Group();
            emit newGroupSelected(tmp);
        }
    }
}

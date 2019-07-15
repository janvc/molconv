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
#include "molconv_window.h"
#include "ui_listofmolecules.h"


class ListOfMoleculesPrivate
{
public:
    ListOfMoleculesPrivate(MolconvWindow *window)
        : m_window(window)
        , m_model(new QStandardItemModel)
    {
    }

    MolconvWindow *m_window;
    QStandardItemModel *m_model;
    QMenu *m_contextMenu;
    QAction *m_actionAlign;
};



ListOfMolecules::ListOfMolecules(MolconvWindow *window)
    : QDockWidget(window)
    , d(new ListOfMoleculesPrivate(window))
    , ui(new Ui::ListOfMolecules)
{
    ui->setupUi(this);

    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    ui->system_tree->setModel(d->m_model);
    QStringList headers;
    headers << tr("Status") << tr("Name") << tr("Number of atoms") << tr("Formula") << tr("Mass");
    d->m_model->setHorizontalHeaderLabels(headers);
    for (int column = 0; column < d->m_model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);

    d->m_contextMenu = new QMenu(ui->system_tree);
    d->m_actionAlign = new QAction("Align Molecules", this);
    d->m_contextMenu->addAction(d->m_actionAlign);

    connect(d->m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(toggleMolecule(QModelIndex)));
    connect(ui->system_tree->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                                                SLOT(changeSelectedItem(QModelIndex)));
    connect(ui->system_tree, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(startContextMenu(const QPoint &)));
    connect(d->m_actionAlign, SIGNAL(triggered()), SLOT(alignMolecules()));
}

ListOfMolecules::~ListOfMolecules()
{
    delete ui;
    delete d;
}

void ListOfMolecules::startContextMenu(const QPoint &point)
{
    QModelIndex index = ui->system_tree->indexAt(point);

    if (index.isValid())
    {
        if (ui->system_tree->selectionModel()->selectedRows().count() > 1)
            d->m_actionAlign->setEnabled(true);
        else
            d->m_actionAlign->setEnabled(false);

        d->m_contextMenu->exec(ui->system_tree->mapToGlobal(point));
    }
}

void ListOfMolecules::alignMolecules()
{
    QModelIndexList indexList = ui->system_tree->selectionModel()->selectedRows();

    std::vector<unsigned long> indices;

    for (int i = 0; i < indexList.size(); i++)
    {
        MoleculeItem *item = static_cast<MoleculeItem*>(d->m_model->itemFromIndex(indexList[i]));
        indices.push_back(item->molID());
    }

    d->m_window->alignMolecules(indices);
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

//            if (d->m_window->activeMolecule())
//                d->m_window->activeMolecule()->listItem()->setIcon(QIcon(":/icons/item-inactive.png"));

            item->setIcon(QIcon(":/icons/item-active.png"));
        }
        items << item;
    }

    d->m_model->appendRow(items);
    for (int column = 0; column < d->m_model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);

//    newMol->setListItem(static_cast<MoleculeItem *>(items.front()));

    // automatically select the molecule that was last added to make it clear, which
    // molecule is currently being edited
    ui->system_tree->selectionModel()->select(items.front()->index(),
                                  QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    ui->system_tree->setCurrentIndex(items.front()->index());
}

void ListOfMolecules::insertGroup(molconv::MoleculeGroup *newGroup)
{
    QList<QStandardItem *> items;
    for (int i = 0; i < 5; i++)
    {
        QStandardItem *item = new GroupItem(newGroup, i);
        item->setBackground(QBrush(QColor(255,128,128)));
        if (i == 0)
        {
            item->setCheckable(true);
            item->setCheckState(Qt::Checked);
            item->setIcon(QIcon(":/icons/item-active.png"));
        }
        items << item;
    }
    d->m_model->appendRow(items);

    for (int i = 0; i < newGroup->nMolecules(); i++)
    {
        QStandardItem *current = newGroup->getMol(i)->listItem();
        QList<QStandardItem *> rowItems = d->m_model->invisibleRootItem()->takeRow(current->row());
        items.front()->appendRow(rowItems);
    }

    ui->system_tree->expand(items.front()->index());

    for (int column = 0; column < d->m_model->columnCount(); column++)
        ui->system_tree->resizeColumnToContents(column);
}

void ListOfMolecules::removeCurrentMolecule()
{
    QModelIndex index = ui->system_tree->selectionModel()->currentIndex();
    d->m_model->removeRow(index.row(), index.parent());
}

unsigned long ListOfMolecules::currentmolID()
{
    QModelIndex index = ui->system_tree->selectionModel()->currentIndex();
    MoleculeItem *currentItem = static_cast<MoleculeItem *>(d->m_model->itemFromIndex(index));

    return currentItem->molID();
}

void ListOfMolecules::toggleMolecule(const QModelIndex &index)
{
    bool isVisible = false;
    if (d->m_model->itemFromIndex(index)->checkState() == Qt::Checked)
        isVisible = true;

    // find out if the item that was clicked is a molecule or a group:
    if (d->m_model->itemFromIndex(index)->type() == QStandardItem::UserType + 1)       // molecule
    {
        MoleculeItem *currentItem = static_cast<MoleculeItem *>(d->m_model->itemFromIndex(index));
        d->m_window->toggle_molecule(currentItem->molID(), isVisible);
    }
    else if (d->m_model->itemFromIndex(index)->type() == QStandardItem::UserType + 2)  // group
    {
//        GroupItem *currentItem = static_cast<GroupItem *>(d->m_model->itemFromIndex(index));
//        molconv::MoleculeGroup *currentGroup = currentItem->Group();

//        for (int i = 0; i < currentGroup->nMolecules(); i++)
//        {
//            d->m_window->toggle_molecule(currentGroup->getMol(i), isVisible);
//            if (isVisible)
//                currentItem->child(i)->setCheckState(Qt::Checked);
//            else
//                currentItem->child(i)->setCheckState(Qt::Unchecked);
//        }
    }
}

void ListOfMolecules::changeSelectedItem(const QModelIndex &current)
{
    if (current.column() > 0)
    {
        if (d->m_model->itemFromIndex(current)->type() == QStandardItem::UserType + 1)         // molecule
        {
            MoleculeItem *currentItem = static_cast<MoleculeItem *>(d->m_model->itemFromIndex(current));

            unsigned long tmp = currentItem->molID();
            unsigned long active = d->m_window->activeMolID();

            if (tmp != active)
            {
                d->m_window->getMol(tmp)->listItem()->setIcon(QIcon(":/icons/item-active.png"));
                d->m_window->getMol(active)->listItem()->setIcon(QIcon(":/icons/item-inactive.png"));
            }

            emit newMoleculeSelected(tmp);
        }
        else if (d->m_model->itemFromIndex(current)->type() == QStandardItem::UserType + 2)    // group
        {
            GroupItem *currentItem = static_cast<GroupItem *>(d->m_model->itemFromIndex(current));
            molconv::MoleculeGroup *tmp = currentItem->Group();
            emit newGroupSelected(tmp);
        }
    }
}

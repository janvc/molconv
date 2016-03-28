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

#include "moleculelistitem.h"
#include "moleculelistmodel.h"

class MoleculeListModelPrivate
{
public:
    MoleculeListModelPrivate()
        : rootItem(0)
    {
    }

    MoleculeListItem *rootItem;
};

MoleculeListModel::MoleculeListModel(QObject *parent)
    : QAbstractItemModel(parent)
    , d(new MoleculeListModelPrivate)
{
    qDebug("this is the constructor of MoleculeListModel");

    QVector<QVariant> rootData;
    rootData << tr("Molecule Name (test 1)");
    rootData << tr("Mass [u] (test 2)");
    d->rootItem = new MoleculeListItem(rootData);
}

QModelIndex MoleculeListModel::index(int row, int column, const QModelIndex &parent) const
{
    qDebug("entering MoleculeListModel::index()");

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    MoleculeListItem *parentItem = item(parent);
    MoleculeListItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex MoleculeListModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    MoleculeListItem *childItem = item(child);
    MoleculeListItem *parentItem = childItem->parent();

    if (parentItem == d->rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int MoleculeListModel::rowCount(const QModelIndex &parent) const
{
}

int MoleculeListModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant MoleculeListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    MoleculeListItem *MolItem = item(index);

    if (MolItem)
        if (index.column() < MolItem->columnCount())
            return MolItem->data(index.column());

    return QVariant();
}

MoleculeListItem *MoleculeListModel::item(const QModelIndex &index) const
{
    if (index.isValid())
    {
        MoleculeListItem *item = static_cast<MoleculeListItem*>(index.internalPointer());

        if (item)
            return item;
    }

    return d->rootItem;
}

void MoleculeListModel::setMolecule(const QModelIndex &index, molconv::moleculePtr &mol)
{
    MoleculeListItem *theItem = item(index);
    theItem->setMolecule(mol);
}


bool MoleculeListModel::insertRows(int row, int count, const QModelIndex &parent)
{
}

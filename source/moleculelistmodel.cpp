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


MoleculeListModel::MoleculeListModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new MoleculeListItem(rootData);
}

MoleculeListModel::~MoleculeListModel()
{
    delete rootItem;
}

int MoleculeListModel::columnCount(const QModelIndex & /*parent */) const
{
    return rootItem->columnCount();
}

QVariant MoleculeListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    MoleculeListItem *item = getItem(index);

    if (role == Qt::CheckStateRole && index.column() == 0)
        return static_cast<int>(item->isChecked() ? Qt::Checked : Qt::Unchecked);

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    return item->data(index.column());
}

Qt::ItemFlags MoleculeListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEditable | QAbstractItemModel::flags(index);

    if (index.column() == 0)
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

MoleculeListItem *MoleculeListModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        MoleculeListItem *item = static_cast<MoleculeListItem*>(index.internalPointer());

        if (item)
            return item;
    }
    return rootItem;
}

QVariant MoleculeListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex MoleculeListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    MoleculeListItem *parentItem = getItem(parent);

    MoleculeListItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool MoleculeListModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool MoleculeListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    MoleculeListItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex MoleculeListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    MoleculeListItem *childItem = getItem(index);
    MoleculeListItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool MoleculeListModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool MoleculeListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    MoleculeListItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int MoleculeListModel::rowCount(const QModelIndex &parent) const
{
    MoleculeListItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool MoleculeListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole && role != Qt::CheckStateRole)
        return false;

    MoleculeListItem *item = getItem(index);
    bool result;
    if (role == Qt::CheckStateRole)
    {
        item->toggleChecked();
        result = true;
    }
    else
        result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool MoleculeListModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void MoleculeListModel::setMolecule(const QModelIndex &index, molconv::moleculePtr &newMol)
{
    MoleculeListItem *item = getItem(index);

    item->setMolecule(newMol);
}

molconv::moleculePtr MoleculeListModel::Molecule(const QModelIndex &index) const
{
    MoleculeListItem *item = getItem(index);
    return item->Molecule();
}

bool MoleculeListModel::isChecked(const QModelIndex &index) const
{
    MoleculeListItem *item = getItem(index);
    return item->isChecked();
}

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

#include "moleculelistmodel.h"

MoleculeListModel::MoleculeListModel(molconv::sysPtr &newSystem, QObject *parent)
    : QAbstractItemModel(parent)
    , m_system(newSystem)
{
    qDebug("this is the constructor of MoleculeListModel");
}

QModelIndex MoleculeListModel::index(int row, int column, const QModelIndex &parent) const
{
    qDebug("entering MoleculeListModel::index()");

    if (!hasIndex(row, column, parent))
        return QModelIndex();
}

QModelIndex MoleculeListModel::parent(const QModelIndex &child) const
{
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
}

bool MoleculeListModel::insertRows(int row, int count, const QModelIndex &parent)
{
}

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

#ifndef MOLECULELISTMODEL_H
#define MOLECULELISTMODEL_H

#include <QAbstractItemModel>
#include "molconv-system/system.h"

class MoleculeListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MoleculeListModel(molconv::sysPtr &newSystem, QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
signals:

public slots:

private:
    molconv::sysPtr m_system;
};

#endif // MOLECULELISTMODEL_H

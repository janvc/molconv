/*
 * Copyright 2014-2015 Jan von Cosel & Sebastian Lenz
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


#ifndef MOLECULELISTITEM_H
#define MOLECULELISTITEM_H

#include<QList>
#include<QVector>
#include<QVariant>
#include "molconv-molecule/molecule.h"

class MoleculeListItemPrivate;

class MoleculeListItem
{
public:
    MoleculeListItem(molconv::moleculePtr &molecule, MoleculeListItem *parent = 0);
    MoleculeListItem(const QVector<QVariant> &data, MoleculeListItem *parent = 0);
    ~MoleculeListItem();

    void appendChild(MoleculeListItem *child);
    MoleculeListItem *child(const int row);
    MoleculeListItem *parent();
    int columnCount() const;
    int childNumber() const;

    molconv::moleculePtr Molecule() const;
    void setMolecule(molconv::moleculePtr &mol);

    int childCount() const;
    int row() const;

    QList<MoleculeListItem*> *children() const;

    QVariant data(int column) const;
    void setData(int column, QVariant &newData);

private:
    MoleculeListItemPrivate *d;
};

#endif // MOLECULELISTITEM_H

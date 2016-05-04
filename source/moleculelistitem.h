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
#include "molecule.h"


class MoleculeListItem
{
public:
    explicit MoleculeListItem(const QVector<QVariant> &data, MoleculeListItem *parent = 0);
    ~MoleculeListItem();

    MoleculeListItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    MoleculeListItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &newData);
    bool isChecked() const;
    void setChecked(bool set);
    void toggleChecked();
    void setMolecule(molconv::moleculePtr &newMol);
    molconv::moleculePtr Molecule() const;

private:
    QList<MoleculeListItem*> childItems;
    QVector<QVariant> itemData;
    MoleculeListItem *parentItem;
    bool checked;
    molconv::moleculePtr molecule;
};

#endif // MOLECULELISTITEM_H

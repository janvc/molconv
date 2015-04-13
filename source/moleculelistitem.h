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
#include<QVariant>


class moleculeListItem
{
public:
    moleculeListItem(const bool molecule, QList<QVariant> &newData, moleculeListItem *parent = 0);
    ~moleculeListItem();

    void appendChild(moleculeListItem *child);
    moleculeListItem *child(const int row);
    moleculeListItem *parent();

    int childCount() const;
    int row() const;

    QVariant data(int column) const;
    void setData(int column, QVariant &newData);

private:
    bool isMolecule;    // the item can either represent a molecule or a group
    moleculeListItem *parentItem;
    QList<moleculeListItem*> childItems;
    QList<QVariant> itemData;
};

#endif // MOLECULELISTITEM_H

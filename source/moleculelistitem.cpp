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


#include "moleculelistitem.h"

moleculeListItem::moleculeListItem(const bool molecule, QList<QVariant> &newData, moleculeListItem *parent)
    : isMolecule(molecule)
    , itemData(newData)
    , parentItem(parent)
{
}

moleculeListItem::~moleculeListItem()
{
    qDeleteAll(childItems);
}

void moleculeListItem::appendChild(moleculeListItem *child)
{
    childItems.append(child);
}

moleculeListItem *moleculeListItem::child(const int row)
{
    return childItems.value(row);
}

moleculeListItem *moleculeListItem::parent()
{
    return parentItem;
}

int moleculeListItem::childCount() const
{
    return childItems.count();
}

int moleculeListItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<moleculeListItem*>(this));

    return 0;
}

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


MoleculeListItem::MoleculeListItem(const QVector<QVariant> &data, MoleculeListItem *parent)
{
    parentItem = parent;
    itemData = data;
}

MoleculeListItem::~MoleculeListItem()
{
    qDeleteAll(childItems);
}

MoleculeListItem *MoleculeListItem::child(int number)
{
    return childItems.value(number);
}

int MoleculeListItem::childCount() const
{
    return childItems.count();
}

int MoleculeListItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<MoleculeListItem*>(this));

    return 0;
}

int MoleculeListItem::columnCount() const
{
    return itemData.count();
}

QVariant MoleculeListItem::data(int column) const
{
    return itemData.value(column);
}

bool MoleculeListItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; row++)
    {
        QVector<QVariant> data(columns);
        MoleculeListItem *item = new MoleculeListItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool MoleculeListItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; column++)
        itemData.insert(position, QVariant());

    foreach (MoleculeListItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

MoleculeListItem *MoleculeListItem::parent()
{
    return parentItem;
}

bool MoleculeListItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; row++)
        delete childItems.takeAt(position);

    return true;
}

bool MoleculeListItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; column++)
        itemData.remove(position);

    foreach (MoleculeListItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool MoleculeListItem::setData(int column, const QVariant &newData)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = newData;
    return true;
}

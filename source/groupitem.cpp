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

#include<QDebug>
#include "groupitem.h"

GroupItem::GroupItem(QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{
    qDebug("This is the default constructor of GroupItem");
}


GroupItem::GroupItem(QTreeWidgetItem *parent, molconv::abstractMoleculeGroup *theGroup)
    : QTreeWidgetItem(parent)
    , m_group(theGroup)
{
    qDebug("This is the second constructor of GroupItem");
}

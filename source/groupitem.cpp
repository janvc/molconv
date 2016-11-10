/*
 * Copyright 2014-2016 Jan von Cosel & Sebastian Lenz
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


#include "groupitem.h"

GroupItem::GroupItem(molconv::MoleculeGroup *newGroup, int column)
    : QStandardItem()
{
    m_group = newGroup;

    switch (column)
    {
    case 1:
        setText(QString::fromStdString(m_group->name()));
        break;
    }
}

molconv::MoleculeGroup *GroupItem::Group() const
{
    return m_group;
}

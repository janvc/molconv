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


#include "moleculeitem.h"


MoleculeItem::MoleculeItem(molconv::moleculePtr &newMol, int column)
    : QStandardItem()
{
    m_molecule = newMol;

    switch (column)
    {
    case 1:
        setText(QString::fromStdString(m_molecule->name()));
        break;
    case 2:
        setText(QString::number(m_molecule->size()));
        break;
    case 3:
        setText(QString::fromStdString(m_molecule->formula()));
        break;
    case 4:
        setText(QString::number(m_molecule->mass()));
        break;
    }
}

molconv::moleculePtr MoleculeItem::Molecule() const
{
    return m_molecule;
}

int MoleculeItem::type() const
{
    return QStandardItem::UserType + 1;
}


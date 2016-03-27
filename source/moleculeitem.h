/*
 * Copyright 2014 - 2016 Jan von Cosel & Sebastian Lenz
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

#ifndef MOLECULEITEM_H
#define MOLECULEITEM_H

#include <QTreeWidgetItem>
#include "molconv-molecule/molecule.h"

class MoleculeItem : public QTreeWidgetItem
{
public:
    explicit MoleculeItem(QTreeWidgetItem *parent = 0);
    MoleculeItem(molconv::moleculePtr theMolecule, QTreeWidgetItem *parent = 0);

    molconv::moleculePtr getMolecule();

private:
    molconv::moleculePtr m_molecule;
};

#endif // MOLECULEITEM_H

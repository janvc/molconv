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

#ifndef MOLECULE_LIST_H_
#define MOLECULE_LIST_H_

#include<QtWidgets>
#include "molecule.h"
#include "abstractmoleculegroup.h"
#include "types.h"

namespace Ui
{
    class ListOfMolecules;
}

class MolconvWindow;

class ListOfMolecules : public QDockWidget
{
    Q_OBJECT

    public:
        ListOfMolecules(MolconvWindow *window);
        ~ListOfMolecules();

        void insertMolecule(molconv::moleculePtr &newMol);

signals:
        void newMoleculeSelected(molconv::moleculePtr &newMol);

private slots:
        void toggleMolecule(const QModelIndex &index);
        void on_system_tree_clicked(const QModelIndex &index);

private:
        Ui::ListOfMolecules *ui;
        MolconvWindow *m_window;
};


#endif /* MOLECULE_LIST_H_ */

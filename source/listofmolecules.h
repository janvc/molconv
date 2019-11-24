/*
 * Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz
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
#include<QStandardItemModel>
#include "moleculegroup.h"

namespace Ui
{
    class ListOfMolecules;
}

class MolconvWindow;
class ListOfMoleculesPrivate;

class ListOfMolecules : public QDockWidget
{
    Q_OBJECT

public:
    ListOfMolecules(MolconvWindow *window);
    ~ListOfMolecules();

    void insertMolecule(molconv::moleculePtr &newMol);
    void insertGroup(molconv::MoleculeGroup *newGroup);
    void removeCurrentMolecule();
    std::vector<unsigned long> getSelectedMoleculeIDs();
    unsigned long currentmolID();

public slots:
    void alignMolecules();
    void calculateRMSD();

signals:
    void newMoleculeSelected(unsigned long newMolID);
    void newGroupSelected(molconv::MoleculeGroup *newGroup);

private slots:
    void toggleMolecule(const QModelIndex &index);
    void changeSelectedItem(const QModelIndex &current);
    void startContextMenu(const QPoint &point);
    void updateActiveItem(const unsigned long newActiveMolID);

private:
    ListOfMoleculesPrivate *d;
    Ui::ListOfMolecules *ui;
};


#endif /* MOLECULE_LIST_H_ */

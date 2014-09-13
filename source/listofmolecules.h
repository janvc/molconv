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

#ifndef MOLECULE_LIST_H_
#define MOLECULE_LIST_H_

#include<QtGui>
#include"molecule.h"

namespace Ui
{
    class ListOfMolecules;
}

class molconv_window;

class ListOfMolecules : public QDockWidget
{
    Q_OBJECT

public:
    ListOfMolecules(molconv_window *window);
    ~ListOfMolecules();

private slots:
    void show_item(molconv::Molecule *molecule);

private:
    Ui::ListOfMolecules *ui;
    molconv_window *m_window;
    QListWidgetItem *the_item;
};


#endif /* MOLECULE_LIST_H_ */
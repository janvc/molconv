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

#ifndef MOLCONV_WINDOW_H_
#define MOLCONV_WINDOW_H_


#include<QtWidgets>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
#endif

#include "system.h"
#include "moleculegroup.h"
#include "moleculestack.h"

#include "open_dialog.h"
#include "newgroupdialog.h"

class ListOfMolecules;
class MolconvWindowPrivate;

namespace Ui
{
    class MolconvWindow;
}

class MolconvWindow : public QMainWindow
{
    Q_OBJECT

public:
    MolconvWindow(QMainWindow *parent = 0);
    ~MolconvWindow();
    void add_molecule(molconv::moleculePtr temp_mol);
    int nMolecules();
    molconv::moleculePtr getMol(int index);

public slots:
    void saveFile();
    void quit();
    void getMoleculeDialog();
    void startOpenDialog();
    void startExportDialog();
    void toggle_molecule(molconv::moleculePtr theMolecule, bool state);
    void about();
    void DuplicateMolecule(const molconv::moleculePtr oldMolecule);
    void newGroup();
    void startNewGroupDialog();
    void addMoleculeToGroup();
    void ResetView();
    void removeActiveMolecule();

private slots:

signals:
    void new_molecule(molconv::moleculePtr &newMolecule);

private:
    MolconvWindowPrivate *d;
    Ui::MolconvWindow *ui;
};

#endif /* MOLCONV_WINDOW_H_ */

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

#ifndef MOLCONV_WINDOW_H_
#define MOLCONV_WINDOW_H_


#include<QtWidgets>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
#endif

#include "config.h"
#include "molconv-system/system.h"
#include "molconv-moleculegroup/moleculegroup.h"
#include "molconv-moleculegroup/moleculestack.h"

#include "open_dialog.h"


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
    void clean_up(const int mol_nr, const molconv::Config &config);
    void set_intbasis(const int mol_nr, const molconv::Config &config);

public slots:
    void saveFile(const size_t index, const QString &filename);
    void saveFile();
//    void closeFile();
    void quit();
    void getMoleculeDialog();
    void startOpenDialog();
    void toggle_molecule(int position, bool state);
    void about();
    void DuplicateMolecule(const molconv::moleculePtr oldMolecule);
    void addMoleculeToGroup();

private slots:

signals:
    void new_molecule(molconv::Molecule *molecule);

private:
    Ui::MolconvWindow *ui;
    OpenDialog *m_OpenDialog;

    QDockWidget *m_ListOfMolecules;
    QDockWidget *m_MoleculeSettings;

    molconv::System m_system;

    std::vector<molconv::MoleculeGroup *> m_MoleculeGroups;
    std::vector<molconv::MoleculeStack *> m_MoleculeStacks;
    std::vector<chemkit::GraphicsMoleculeItem *> m_GraphicsItemVector;
};

#endif /* MOLCONV_WINDOW_H_ */

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

#include "importdialog.h"
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
    molconv::moleculePtr getMol(const unsigned long key);
    std::vector<unsigned long> getMolIDs();

    unsigned long activeMolID();

    void importFile(const QString &fileName, const bool showList = false);

    bool readMolconvFile(const QString &fileName);
    void writeMolconvFile(const QString &fileName);

    void selectAtom(chemkit::Atom *theAtom, bool wholeMolecule);
    void deselectAtom(chemkit::Atom *theAtom, bool wholeMolecule);
    std::vector<chemkit::Atom *> selection() const;
    void moveActiveMoleculeTo(const double x, const double y, const double z,
                              const double phi, const double theta, const double psi);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void importFile();
    void openFile();
    void openFile(const QString &fileName);
    void saveFile();
    void saveFileAs();
    void startImportDialog();
    void startExportDialog();
    void toggle_molecule(const unsigned long molID, bool state);
    void about();
    void DuplicateMolecule(const unsigned long oldMolID);
    void newGroup();
    void startNewGroupDialog();
    void startBasisDialog();
    void startSettingsDialog();
    void addMoleculeToGroup();
    void ResetView();
    void removeSelectedMolecules();
    void removeActiveMolecule();
    void removeMolecule(const unsigned long id);
    void DuplicateActiveMolecule();
    void updateActiveMolecule(const unsigned long &newActiveID);
    void changeOriginBasis();
    void updateAxes();
    void alignMolecules(std::vector<unsigned long> &molecules);
    void calculateRMSD(const unsigned long refMolID, const unsigned long otherMolID);
    void minimizeRMSD(const unsigned long refMolID, const unsigned long otherMolID);
    void useNavigateTool();
    void useSelectTool();
    void wasModified();
    void displayHydrogens(bool checked);

private slots:
    void resetCoords();
    void zeroCoords();
    void updateSelection();

signals:
    void new_molecule(unsigned long newMolID);

private:
    void selectAtom(chemkit::Atom *theAtom);
    void deselectAtom(chemkit::Atom *theAtom);
    bool maybeSave();

    MolconvWindowPrivate *d;
    Ui::MolconvWindow *ui;
};

#endif /* MOLCONV_WINDOW_H_ */

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

#include<iostream>
#include<iomanip>
#include<cmath>
#include<map>
#include<algorithm>
#include<QMessageBox>
#include<QDomDocument>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<chemkit/graphicscamera.h>
    #include<chemkit/bondpredictor.h>
    #include<boost/make_shared.hpp>
#endif

#include "types.h"
#include "molconvwindow.h"
#include "ui_molconvwindow.h"
#include "listofmolecules.h"
#include "moleculesettings.h"
#include "importdialog.h"
#include "exportdialog.h"
#include "setbasisdialog.h"
#include "graphicsaxisitem.h"
#include "graphicsselectionitem.h"
#include "aboutdialog.h"
#include "multimoldialog.h"
#include "navigatetool.h"
#include "selecttool.h"
#include "moleculeinfo.h"
#include "molconvfile.h"
#include "moleculeorigin.h"
#include "moleculebasis.h"


class MolconvWindowPrivate
{
public:
    MolconvWindowPrivate()
    {
    }

    ImportDialog *m_ImportDialog;
//    NewGroupDialog *m_NewGroupDialog;
    SetBasisDialog *m_setBasisDialog;
    MultiMolDialog *m_MultiMolDialog;

    ListOfMolecules *m_ListOfMolecules;
    MoleculeSettings *m_MoleculeSettings;
    MoleculeInfo *m_MoleculeInfo;

//    std::vector<molconv::MoleculeGroup *> m_MoleculeGroups;
    std::map<unsigned long, chemkit::GraphicsMoleculeItem *> m_GraphicsItemMap;
    std::map<unsigned long, GraphicsAxisItem *> m_GraphicsAxisMap;
    std::vector<chemkit::Atom *> m_SelectedAtoms;

    GraphicsSelectionItem *m_Selection;
    unsigned long m_activeMolID;

    boost::shared_ptr<NavigateTool> m_navigatetool;
    boost::shared_ptr<SelectTool> m_selecttool;

    QString m_currentFile;
};


MolconvWindow::MolconvWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , d(new MolconvWindowPrivate)
    , ui(new Ui::MolconvWindow)
{
    ui->setupUi(this);

    d->m_ImportDialog = new ImportDialog(this);
//    d->m_NewGroupDialog = new NewGroupDialog(this);
    d->m_setBasisDialog = new SetBasisDialog(this);
    d->m_ListOfMolecules = new ListOfMolecules(this);
    d->m_MoleculeSettings = new MoleculeSettings(this);
    d->m_MoleculeInfo = new MoleculeInfo(this);

    addDockWidget(Qt::BottomDockWidgetArea, d->m_ListOfMolecules);
    addDockWidget(Qt::LeftDockWidgetArea, d->m_MoleculeSettings);
    addDockWidget(Qt::RightDockWidgetArea, d->m_MoleculeInfo);

    ui->actionSet_internal_basis->setEnabled(false);
    ui->actionDuplicate->setEnabled(false);
    ui->actionRemove->setEnabled(false);
    ui->actionZero_Coordinates->setEnabled(false);
    ui->actionReset_Coordinates->setEnabled(false);
    ui->actionProperties->setEnabled(false);
    ui->actionAdd_To_Group->setEnabled(false);
    ui->actionAlign->setEnabled(false);
    ui->actionNew_Molecule_Group->setEnabled(false);

    connect(this, SIGNAL(new_molecule(unsigned long)), d->m_MoleculeInfo, SLOT(setMolecule(unsigned long)));
    connect(this, SIGNAL(new_molecule(unsigned long)), d->m_MoleculeSettings, SLOT(setMolecule(unsigned long)));

    connect(ui->actionSave_As, SIGNAL(triggered()), SLOT(saveFileAs()));
    connect(ui->actionSave, SIGNAL(triggered()), SLOT(saveFile()));
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
    connect(ui->actionImport_Molecule, SIGNAL(triggered()), SLOT(startImportDialog()));
    connect(ui->actionExport_Molecule, SIGNAL(triggered()), SLOT(startExportDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionNew_Molecule_Group, SIGNAL(triggered()), SLOT(startNewGroupDialog()));
    connect(ui->actionSet_internal_basis, SIGNAL(triggered()), SLOT(startBasisDialog()));
    connect(ui->actionDuplicate, SIGNAL(triggered()), SLOT(DuplicateActiveMolecule()));
    connect(ui->actionRemove, SIGNAL(triggered()), SLOT(removeActiveMolecule()));
    connect(ui->actionReset, SIGNAL(triggered()), SLOT(ResetView()));
    connect(ui->actionZero_Coordinates, SIGNAL(triggered()), SLOT(zeroCoords()));
    connect(ui->actionReset_Coordinates, SIGNAL(triggered()), SLOT(resetCoords()));
    connect(ui->actionAlign, SIGNAL(triggered()), d->m_ListOfMolecules, SLOT(alignMolecules()));
    connect(ui->actionNavigate, SIGNAL(triggered()), SLOT(useNavigateTool()));
    connect(ui->actionSelect, SIGNAL(triggered()), SLOT(useSelectTool()));

    connect(d->m_ImportDialog, SIGNAL(accepted()), SLOT(importFile()));

//    connect(d->m_NewGroupDialog, SIGNAL(accepted()), this, SLOT(newGroup()));

    connect(d->m_setBasisDialog, SIGNAL(ready()), SLOT(changeOriginBasis()));

    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(unsigned long)), d->m_MoleculeSettings, SLOT(setMolecule(unsigned long)));
    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(unsigned long)), SLOT(updateActiveMolecule(unsigned long)));
    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(unsigned long)), d->m_MoleculeInfo, SLOT(setMolecule(unsigned long)));
    connect(d->m_ListOfMolecules, SIGNAL(newGroupSelected(molconv::MoleculeGroup*)), d->m_MoleculeSettings, SLOT(setGroup(molconv::MoleculeGroup*)));

    connect(d->m_MoleculeSettings, SIGNAL(basisChanged(double ,double ,double ,double ,double ,double)),
            SLOT(moveActiveMoleculeTo(double ,double ,double ,double ,double ,double)));
    connect(d->m_MoleculeSettings, SIGNAL(editingFinished()), d->m_MoleculeInfo, SLOT(updateMan()));

    GraphicsAxisItem *axes = new GraphicsAxisItem;
    ui->molconv_graphicsview->addItem(axes);

    d->m_Selection = new GraphicsSelectionItem;
    ui->molconv_graphicsview->addItem(d->m_Selection);

    d->m_navigatetool = boost::make_shared<NavigateTool>();
    d->m_selecttool = boost::make_shared<SelectTool>(this);
    useNavigateTool();

    ui->molconv_graphicsview->update();

    d->m_activeMolID = 0;
    d->m_currentFile = QString();

    molconv::Molecule::initRand();
    setWindowTitle(tr("untitled[*] - molconv"));
}

MolconvWindow::~MolconvWindow()
{
    delete ui->molconv_graphicsview;
    delete ui;
    delete d;
}

void MolconvWindow::add_molecule(molconv::moleculePtr temp_mol)
{
    molconv::System& system = molconv::System::get();

    system.addMolecule(temp_mol);
    d->m_activeMolID = temp_mol->molId();

    chemkit::GraphicsMoleculeItem *item = new chemkit::GraphicsMoleculeItem(temp_mol.get());
    d->m_GraphicsItemMap.insert(std::make_pair(d->m_activeMolID, item));
    ui->molconv_graphicsview->addItem(item);

    GraphicsAxisItem *axis = new GraphicsAxisItem(temp_mol->originPosition(), temp_mol->basisVectors());
    d->m_GraphicsAxisMap.insert(std::make_pair(d->m_activeMolID, axis));
    ui->molconv_graphicsview->addItem(axis);

    ui->molconv_graphicsview->update();

    d->m_ListOfMolecules->insertMolecule(temp_mol);

    ui->actionSet_internal_basis->setEnabled(true);
    ui->actionDuplicate->setEnabled(true);
    ui->actionRemove->setEnabled(true);
    ui->actionZero_Coordinates->setEnabled(true);
    ui->actionReset_Coordinates->setEnabled(true);
    ui->actionProperties->setEnabled(true);
    ui->actionAdd_To_Group->setEnabled(true);
    ui->actionAlign->setEnabled(true);

    emit new_molecule(d->m_activeMolID);
}

void MolconvWindow::removeSelectedMolecules()
{
    std::vector<unsigned long> molsToRemove = d->m_ListOfMolecules->selectedMoleculeIDs();

    for (unsigned long id : molsToRemove)
    {
        removeMolecule(id);
    }
}

void MolconvWindow::removeMolecule(const unsigned long id)
{
    molconv::System& system = molconv::System::get();

    d->m_ListOfMolecules->removeRow(id);

    // remove molecule's graphics item
    ui->molconv_graphicsview->deleteItem(d->m_GraphicsItemMap.at(id));
    d->m_GraphicsItemMap.erase(id);

    // remove molecule's axis item
    ui->molconv_graphicsview->deleteItem(d->m_GraphicsAxisMap.at(id));
    d->m_GraphicsAxisMap.erase(id);

    system.removeMolecule(id);

    ui->molconv_graphicsview->update();

    if (system.nMolecules() > 0)
        d->m_activeMolID = d->m_ListOfMolecules->currentmolID();
    else
    {
        d->m_activeMolID = 0;

        // disable all the actions that need a molecule to operate on:
        ui->actionSet_internal_basis->setEnabled(false);
        ui->actionDuplicate->setEnabled(false);
        ui->actionRemove->setEnabled(false);
        ui->actionZero_Coordinates->setEnabled(false);
        ui->actionReset_Coordinates->setEnabled(false);
        ui->actionProperties->setEnabled(false);
        ui->actionAdd_To_Group->setEnabled(false);
    }

    if (d->m_activeMolID)
        d->m_MoleculeSettings->setMolecule(d->m_activeMolID);
}

void MolconvWindow::removeActiveMolecule()
{
    molconv::System& system = molconv::System::get();

    unsigned long molToRemove = d->m_activeMolID;

    // remove active molecule from the list
    d->m_ListOfMolecules->removeCurrentMolecule();

    // remove active molecule's graphics item
    ui->molconv_graphicsview->deleteItem(d->m_GraphicsItemMap.at(molToRemove));
    d->m_GraphicsItemMap.erase(molToRemove);

    // remove active molecule's axis item
    ui->molconv_graphicsview->deleteItem(d->m_GraphicsAxisMap.at(molToRemove));
    d->m_GraphicsAxisMap.erase(molToRemove);

    system.removeMolecule(molToRemove);

    ui->molconv_graphicsview->update();

    if (system.nMolecules() > 0)
        d->m_activeMolID = d->m_ListOfMolecules->currentmolID();
    else
    {
        d->m_activeMolID = 0;

        // disable all the actions that need a molecule to operate on:
        ui->actionSet_internal_basis->setEnabled(false);
        ui->actionDuplicate->setEnabled(false);
        ui->actionRemove->setEnabled(false);
        ui->actionZero_Coordinates->setEnabled(false);
        ui->actionReset_Coordinates->setEnabled(false);
        ui->actionProperties->setEnabled(false);
        ui->actionAdd_To_Group->setEnabled(false);
    }

    if (d->m_activeMolID)
        d->m_MoleculeSettings->setMolecule(d->m_activeMolID);
}

int MolconvWindow::nMolecules()
{
    return molconv::System::get().nMolecules();
}

molconv::moleculePtr MolconvWindow::getMol(const unsigned long key)
{
    return molconv::System::get().getMolecule(key);
}

std::vector<unsigned long> MolconvWindow::getMolIDs()
{
    return molconv::System::get().getMolIDs();
}

unsigned long MolconvWindow::activeMolID()
{
    return d->m_activeMolID;
}

void MolconvWindow::toggle_molecule(const unsigned long molID, bool state)
{
    if (state)
    {
        d->m_GraphicsItemMap.at(molID)->show();
        d->m_GraphicsAxisMap.at(molID)->show();
        ui->molconv_graphicsview->update();
    }
    else
    {
        d->m_GraphicsItemMap.at(molID)->hide();
        d->m_GraphicsAxisMap.at(molID)->hide();
        ui->molconv_graphicsview->update();
    }
}

void MolconvWindow::updateSelection()
{
    d->m_Selection->clear();
    for (int i = 0; i < int(d->m_SelectedAtoms.size()); i++)
        d->m_Selection->addPosition(d->m_SelectedAtoms[i]->position());

    ui->molconv_graphicsview->update();
}

void MolconvWindow::about()
{
    AboutDialog *ad = new AboutDialog(this);
    ad->exec();
}

bool MolconvWindow::maybeSave()
{
    if (isWindowModified())
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning,
                                              tr("Molconv"),
                                              tr("You have unsaved changes"),
                                              QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                              this);
        msgBox->setDefaultButton(QMessageBox::Save);

        int choice = msgBox->exec();

        if (choice == QMessageBox::Save)
        {
            delete msgBox;
            saveFileAs();
            return true;
        }
        else if (choice == QMessageBox::Cancel)
        {
            delete msgBox;
            return false;
        }
        delete msgBox;
    }

    return true;
}

void MolconvWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        QSettings settings;
        settings.setValue("startMaximized", QVariant(isMaximized()));
        settings.setValue("winW", width());
        settings.setValue("winH", height());
        settings.setValue("xPos", x());
        settings.setValue("yPos", y());

        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MolconvWindow::saveFile()
{
    if (d->m_currentFile.isEmpty())
    {
        saveFileAs();
    }
    else
    {
        writeMolconvFile(d->m_currentFile);
    }
}

void MolconvWindow::saveFileAs()
{
    QSettings settings;
    QString startSavePath = settings.value("savePath").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), startSavePath, tr("Molconv files (*.mcv)"));

    if (!fileName.isEmpty())
    {
        settings.setValue("savePath", QFileInfo(fileName).absolutePath());

        if (fileName.split(".").last() != QString("mcv"))
            fileName += QString(".mcv");

        writeMolconvFile(fileName);
    }
}

void MolconvWindow::importFile()
{
    importFile(d->m_ImportDialog->getFileName(), true);
}

void MolconvWindow::openFile()
{
    QSettings settings;
    QString startOpenPath = settings.value("openPath").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), startOpenPath, tr("Molconv files (*.mcv)"));

    if (!fileName.isEmpty())
    {
        settings.setValue("openPath", QFileInfo(fileName).absolutePath());
        readMolconvFile(fileName);
    }
}

void MolconvWindow::openFile(const QString &fileName)
{
    bool result = false;
    if (fileName.endsWith("mcv"))
    {
        result = readMolconvFile(fileName);
    }
    else
    {
        importFile(fileName);
    }

    if (!result)
    {
        QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(fileName));
    }
}

void MolconvWindow::importFile(const QString &fileName, const bool showList)
{
    chemkit::MoleculeFile *molFile = new chemkit::MoleculeFile(fileName.toStdString());

    if (! molFile->read())
    {
        std::cerr << "Could not read molecule file " << fileName.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(molFile->errorString().c_str()));
        delete molFile;
        return;
    }

    if (molFile->moleculeCount() > 0)
    {
        std::vector<bool> molsToOpen(molFile->moleculeCount(), true);

        if (molFile->moleculeCount() > 1 && showList)
        {
            MultiMolDialog *mmd = new MultiMolDialog(this);
            mmd->createMoleculeList(molFile);
            mmd->setWindowTitle("Open '" + fileName.split("/").last() + "'");
            mmd->exec();
            molsToOpen = mmd->molecules();
            delete mmd;
        }

        int index = 0;
        for (int i = 0; i < int(molsToOpen.size()); i++)
        {
            if (molsToOpen.at(i))
            {
                index++;
                chemkit::Molecule tempCMol = *molFile->molecule(i);
                molconv::moleculePtr tempMol;
                tempMol.reset(new molconv::Molecule(tempCMol));
                std::vector<bool> boolVec(tempMol->size(), true);
                tempMol->setOrigin(d->m_ImportDialog->getOriginCode(), boolVec, d->m_ImportDialog->getOriginAtom());
                tempMol->setBasis(d->m_ImportDialog->getBasisCode(), boolVec,
                                  d->m_ImportDialog->getBasisAtoms()[0],
                                  d->m_ImportDialog->getBasisAtoms()[1],
                                  d->m_ImportDialog->getBasisAtoms()[2]);
                QString tempName;
                if (d->m_ImportDialog->getMoleculeName().isEmpty())
                {
                    tempName = fileName.split("/").last().split(".").first();
                    if (molsToOpen.size() > 1)
                        tempName += "_" + QString::number(index);
                }
                else
                {
                    tempName = d->m_ImportDialog->getMoleculeName();
                    if (molsToOpen.size() > 1)
                        tempName += "_" + QString::number(index);
                }
                tempMol->setName(tempName.toStdString());
                add_molecule(tempMol);
            }
        }
        delete molFile;
        wasModified();
    }
    else
    {
        std::cerr << "No molecule found in file " << fileName.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("No molecule found in file: %1").arg(molFile->errorString().c_str()));
        delete molFile;
        return;
    }
}

void MolconvWindow::selectAtom(chemkit::Atom *theAtom, bool wholeMolecule)
{
    if (wholeMolecule)
        for (int i = 0; i < int(theAtom->molecule()->size()); i++)
            selectAtom(theAtom->molecule()->atom(i));
    else
        selectAtom(theAtom);

    updateSelection();
}

void MolconvWindow::selectAtom(chemkit::Atom *theAtom)
{
    bool selected = false;
    for (int i = 0; i < int(d->m_SelectedAtoms.size()); i++)
         if (d->m_SelectedAtoms[i] == theAtom)
         selected = true;

    if (!selected)
        d->m_SelectedAtoms.push_back(theAtom);
}

void MolconvWindow::deselectAtom(chemkit::Atom *theAtom)
{
    int index = -1;
    for (int i = 0; i < int(d->m_SelectedAtoms.size()); i++)
        if (d->m_SelectedAtoms[i] == theAtom)
            index = i;

    if (index >= 0)
        d->m_SelectedAtoms.erase(d->m_SelectedAtoms.begin() + index);
}

void MolconvWindow::deselectAtom(chemkit::Atom *theAtom, bool wholeMolecule)
{
    if (wholeMolecule)
        for (int i = 0; i < int(theAtom->molecule()->size()); i++)
            deselectAtom(theAtom->molecule()->atom(i));
    else
        deselectAtom(theAtom);

    updateSelection();
}

std::vector<chemkit::Atom *> MolconvWindow::selection() const
{
    return d->m_SelectedAtoms;
}

void MolconvWindow::startImportDialog()
{
    d->m_ImportDialog->show();
}

void MolconvWindow::startExportDialog()
{
    ExportDialog *ed = new ExportDialog(this);
    ed->createMoleculeList(getMolIDs());
    ed->setModal(true);
    ed->exec();
    delete ed;
}

void MolconvWindow::startBasisDialog()
{
    d->m_setBasisDialog->prepare(d->m_activeMolID);
    d->m_setBasisDialog->show();
}

void MolconvWindow::DuplicateActiveMolecule()
{
    DuplicateMolecule(d->m_activeMolID);
}

void MolconvWindow::DuplicateMolecule(const unsigned long oldMolID)
{
    molconv::moleculePtr newMol = boost::make_shared<molconv::Molecule>(getMol(oldMolID));

    add_molecule(newMol);
}

//void MolconvWindow::newGroup()
//{
//    molconv::System& system = molconv::System::get();

//    std::string newGroupName = d->m_NewGroupDialog->groupName();
//    std::vector<bool> members = d->m_NewGroupDialog->molecules();

//    // check if we have nested groups i.e. the molecules we are adding to this group are
//    // already members of THE SAME existing group. That group will then be the parent of
//    // this group.
//    std::vector<molconv::groupPtr> groups;
//    for (int i = 0; i < nMolecules(); i++)
//        if (members.at(i))
//            groups.push_back(getMol(i)->group());

//    for (int i = 0; i < int(groups.size()); i++)
//        if (groups[i] != groups[0])
//        {
//            QMessageBox::critical(this, "Error", "All molecules must be part of the same group");
//            return;
//        }

//    if (d->m_NewGroupDialog->isStack())
//    {
//        molconv::groupPtr newStack(new molconv::MoleculeStack(newGroupName));

//        for (int i = 0; i < nMolecules(); i++)
//            if (members.at(i))
//            {
//                static_cast<molconv::MoleculeStack*>(newStack.get())->addMolecule(getMol(i), molconv::zVec);
//                getMol(i)->addToGroup(newStack);
//            }
//        newStack->addToGroup(groups[0]);
//        system.addGroup(newStack);
//        d->m_ListOfMolecules->insertGroup(newStack.get());
//    }
//    else
//    {
//        molconv::groupPtr newGroup(new molconv::MoleculeGroup(newGroupName));

//        for (int i = 0; i < nMolecules(); i++)
//            if (members.at(i))
//            {
//                newGroup->addMolecule(getMol(i));
//                getMol(i)->addToGroup(newGroup);
//            }
//        newGroup->addToGroup(groups[0]);
//        system.addGroup(newGroup);
//        d->m_ListOfMolecules->insertGroup(newGroup.get());
//    }
//}

//void MolconvWindow::startNewGroupDialog()
//{
//    d->m_NewGroupDialog->createMoleculeList();
//    d->m_NewGroupDialog->setModal(true);
//    d->m_NewGroupDialog->exec();
//}

//void MolconvWindow::addMoleculeToGroup()
//{
//}

void MolconvWindow::ResetView()
{
    // determine largest distance of any atom from the global origin:
    double maxLength = 0;
    for (unsigned long id : getMolIDs())
    {
        molconv::moleculePtr mol = getMol(id);

        for (int j = 0; j < int(mol->size()); j++)
        {
            double length = mol->atom(j)->position().norm();
            if (length > maxLength)
                maxLength = length;
        }
    }

    // set the camera to a distance of r / tan(22.5 deg)
    // (where tan(22.5 deg) = sqrt(2) - 1)
    // but at least 10
    double dist = maxLength / 0.4142135624 > 10.0 ? maxLength / 0.4142135624 : 10.0;

    ui->molconv_graphicsview->setCamera(boost::make_shared<chemkit::GraphicsCamera>(0,0,dist));
}

void MolconvWindow::updateActiveMolecule(const unsigned long &newActiveID)
{
    d->m_activeMolID = newActiveID;
}

void MolconvWindow::changeOriginBasis()
{
    molconv::OriginCode newOriginCode = d->m_setBasisDialog->originCode();
    molconv::BasisCode newBasisCode = d->m_setBasisDialog->basisCode();

    std::array<int,2> newOriginAtoms = d->m_setBasisDialog->originAtoms();
    std::array<int,3> newBasisAtoms = d->m_setBasisDialog->basisAtoms();

    double newAtomLineScale = d->m_setBasisDialog->atomLineScale();

    std::vector<bool> newOriginList = d->m_setBasisDialog->selectedOriginAtoms();
    std::vector<bool> newBasisList = d->m_setBasisDialog->selectedBasisAtoms();

    molconv::moleculePtr tmpMolPtr = getMol(d->m_activeMolID);

    // determine if the new basis is different from the old one:
    if (
            newOriginCode != tmpMolPtr->origin()->code()
         || newBasisCode != tmpMolPtr->basis()->code()
         || newAtomLineScale != tmpMolPtr->originFactor()
         || ! std::equal(newOriginAtoms.begin(), newOriginAtoms.end(), tmpMolPtr->originAtoms().begin())
         || ! std::equal(newBasisAtoms.begin(), newBasisAtoms.end(), tmpMolPtr->basisAtoms().begin())
         || ! std::equal(newOriginList.begin(), newOriginList.end(), tmpMolPtr->originList().begin())
         || ! std::equal(newBasisList.begin(), newBasisList.end(), tmpMolPtr->basisList().begin())
            )
    {
        wasModified();
    }

    tmpMolPtr->setOrigin(newOriginCode, newOriginList, size_t(newOriginAtoms[0]), size_t(newOriginAtoms[1]), newAtomLineScale);
    tmpMolPtr->setBasis(newBasisCode, newBasisList, newBasisAtoms[0], newBasisAtoms[1], newBasisAtoms[2]);

    d->m_MoleculeSettings->setMolecule(d->m_activeMolID);
    updateAxes();
}

void MolconvWindow::updateAxes()
{
    molconv::moleculePtr tmpMolPtr = getMol(d->m_activeMolID);

    d->m_GraphicsAxisMap.at(d->m_activeMolID)->setPosition(tmpMolPtr->originPosition());
    d->m_GraphicsAxisMap.at(d->m_activeMolID)->setVectors(tmpMolPtr->basisVectors());

    ui->molconv_graphicsview->update();
}

void MolconvWindow::useNavigateTool()
{
    ui->actionSelect->setChecked(false);
    ui->actionNavigate->setChecked(true);
    ui->molconv_graphicsview->setTool(d->m_navigatetool);
}

void MolconvWindow::useSelectTool()
{
    ui->actionNavigate->setChecked(false);
    ui->actionSelect->setChecked(true);
    ui->molconv_graphicsview->setTool(d->m_selecttool);
}

void MolconvWindow::wasModified()
{
    setWindowModified(true);
    ui->actionSave->setEnabled(true);
}

void MolconvWindow::moveActiveMoleculeTo(const double x, const double y, const double z,
                                         const double phi, const double theta, const double psi)
{
    getMol(d->m_activeMolID)->moveFromParas(x, y, z, phi, theta, psi);
    updateAxes();
    updateSelection();
    d->m_MoleculeInfo->updateLive();
    wasModified();
}

void MolconvWindow::resetCoords()
{
    std::array<double,6> oB = getMol(d->m_activeMolID)->originalBasis();
    moveActiveMoleculeTo(oB[0], oB[1], oB[2], oB[3], oB[4], oB[5]);
    d->m_MoleculeSettings->setMolecule(d->m_activeMolID);
    d->m_MoleculeInfo->updateMan();
}

void MolconvWindow::zeroCoords()
{
    moveActiveMoleculeTo(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    d->m_MoleculeSettings->setMolecule(d->m_activeMolID);
    d->m_MoleculeInfo->updateMan();
}

void MolconvWindow::alignMolecules(std::vector<unsigned long> &molecules)
{
    unsigned long refMolID = d->m_activeMolID;

    for (auto i : molecules)
    {
        if (i != refMolID)
            minimizeRMSD(refMolID, i);
    }
}

void MolconvWindow::calculateRMSD(const unsigned long refMolID, const unsigned long otherMolID)
{
    molconv::moleculePtr refMol = getMol(refMolID);
    molconv::moleculePtr otherMol = getMol(otherMolID);

    if (refMol->size() != otherMol->size())
    {
        QMessageBox::warning(this, tr("RMSD"), tr("The RMSD can only be calculated for molecules with equal number of atoms."));
        return;
    }

    double rmsd = molconv::System::get().calculateRMSDbetween(refMolID, otherMolID);

    if (rmsd >= 0.0)
    {
        QString message = tr("The RMSD between\n'")
                + QString::fromStdString(refMol->name())
                + tr("'\nand\n'")
                + QString::fromStdString(otherMol->name())
                + tr("'\nis ")
                + QString::number(rmsd) + QString::fromUtf8(" \u00C5");

        QMessageBox::information(this, tr("RMSD"), message);
    }
    else
    {
        QMessageBox::warning(this, tr("RMSD"), tr("Something went terribly wrong during the RMSD calculation."));
    }
}

void MolconvWindow::minimizeRMSD(const unsigned long refMolID, const unsigned long otherMolID)
{
    molconv::moleculePtr refMol = getMol(refMolID);
    molconv::moleculePtr otherMol = getMol(otherMolID);

    bool success = molconv::System::get().alignMolecules(refMolID, otherMolID);
    double rmsd = molconv::System::get().calculateRMSDbetween(refMolID, otherMolID);

    if (success)
    {
        QString message = tr("The molecules\n'")
                + QString::fromStdString(refMol->name())
                + tr("'\nand\n'")
                + QString::fromStdString(otherMol->name())
                + tr("'\nwere aligned with a residual\nRMSD of ")
                + QString::number(rmsd) + QString::fromUtf8(" \u00C5");

        QMessageBox::information(this, tr("RMSD"), message);
    }
    else
    {
        QMessageBox::critical(this, tr("Alignment impossible"), tr("Only molecules with equal number of atoms can be aligned."));
        return;
    }

    updateAxes();
    updateSelection();
    d->m_MoleculeInfo->updateLive();
    wasModified();
}

void MolconvWindow::writeMolconvFile(const QString &fileName)
{
    MolconvFile file = MolconvFile();
    file.write(fileName);
    d->m_currentFile = fileName;
    setWindowTitle(fileName.split("/").last() + "[*] - molconv");
    setWindowModified(false);
    ui->actionSave->setEnabled(false);
}

bool MolconvWindow::readMolconvFile(const QString &fileName)
{
    MolconvFile file = MolconvFile();
    file.read(fileName);
    for (auto mol : file.molecules())
    {
        add_molecule(mol);
    }
    d->m_currentFile = fileName;
    setWindowTitle(fileName.split("/").last() + "[*] - molconv");
    return true;
}

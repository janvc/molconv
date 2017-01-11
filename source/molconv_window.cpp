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

#include<iostream>
#include<QMessageBox>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<chemkit/graphicscamera.h>
    #include<boost/make_shared.hpp>
#endif

#include "types.h"
#include "molconv_window.h"
#include "ui_molconv_window.h"
#include "listofmolecules.h"
#include "moleculesettings.h"
#include "import_dialog.h"
#include "export_dialog.h"
#include "setbasisdialog.h"
#include "graphicsaxisitem.h"
#include "graphicsselectionitem.h"
#include "aboutdialog.h"
#include "multimoldialog.h"
#include "navigatetool.h"
#include "selecttool.h"


class MolconvWindowPrivate
{
public:
    MolconvWindowPrivate()
        : m_system(new molconv::System)
    {
    }

    ImportDialog *m_ImportDialog;
    ExportDialog *m_ExportDialog;
    NewGroupDialog *m_NewGroupDialog;
    setBasisDialog *m_setBasisDialog;
    MultiMolDialog *m_MultiMolDialog;

    ListOfMolecules *m_ListOfMolecules;
    MoleculeSettings *m_MoleculeSettings;

    molconv::sysPtr m_system;

    std::vector<molconv::MoleculeGroup *> m_MoleculeGroups;
    std::vector<chemkit::GraphicsMoleculeItem *> m_GraphicsItemVector;
    std::vector<GraphicsAxisItem *> m_GraphicsAxisVector;
    std::vector<chemkit::Atom *> m_SelectedAtoms;

    GraphicsSelectionItem *m_Selection;
    molconv::moleculePtr activeMolecule;

    boost::shared_ptr<NavigateTool> m_navigatetool;
    boost::shared_ptr<SelectTool> m_selecttool;
};


MolconvWindow::MolconvWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , d(new MolconvWindowPrivate)
    , ui(new Ui::MolconvWindow)
{
    ui->setupUi(this);

    d->m_ImportDialog = new ImportDialog(this);
    d->m_ExportDialog = new ExportDialog(this);
    d->m_NewGroupDialog = new NewGroupDialog(this);
    d->m_setBasisDialog = new setBasisDialog(this);

    connect(d->m_ImportDialog, SIGNAL(accepted()), SLOT(openFile()));
    connect(d->m_NewGroupDialog, SIGNAL(accepted()), this, SLOT(newGroup()));
    connect(d->m_setBasisDialog, SIGNAL(ready()), SLOT(changeOriginBasis()));

    connect(qApp, SIGNAL(aboutToQuit()), SLOT(quit()));

    connect(ui->actionImport_Molecule, SIGNAL(triggered()), SLOT(startImportDialog()));
    connect(ui->actionExport_Molecule, SIGNAL(triggered()), SLOT(startExportDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionNew_Molecule_Group, SIGNAL(triggered()), SLOT(startNewGroupDialog()));
    connect(ui->actionSet_internal_basis, SIGNAL(triggered()), SLOT(startBasisDialog()));
    connect(ui->actionDuplicate, SIGNAL(triggered()), SLOT(DuplicateActiveMolecule()));
    connect(ui->actionRemove, SIGNAL(triggered()), SLOT(removeActiveMolecule()));
    connect(ui->actionReset, SIGNAL(triggered()), SLOT(ResetView()));
    connect(ui->actionZero_Coordinates, SIGNAL(triggered()), SLOT(zeroCoords()));
    connect(ui->actionReset_Coordinates, SIGNAL(triggered()), SLOT(resetCoords()));
    connect(ui->actionNavigate, SIGNAL(triggered()), SLOT(useNavigateTool()));
    connect(ui->actionSelect, SIGNAL(triggered()), SLOT(useSelectTool()));

    ui->actionSet_internal_basis->setEnabled(false);
    ui->actionDuplicate->setEnabled(false);
    ui->actionRemove->setEnabled(false);
    ui->actionZero_Coordinates->setEnabled(false);
    ui->actionReset_Coordinates->setEnabled(false);
    ui->actionProperties->setEnabled(false);
    ui->actionAdd_To_Group->setEnabled(false);

    d->m_ListOfMolecules = new ListOfMolecules(this);
    d->m_MoleculeSettings = new MoleculeSettings(this);
    addDockWidget(Qt::BottomDockWidgetArea, d->m_ListOfMolecules);
    addDockWidget(Qt::LeftDockWidgetArea, d->m_MoleculeSettings);

    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(molconv::moleculePtr&)), d->m_MoleculeSettings, SLOT(setMolecule(molconv::moleculePtr&)));
    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(molconv::moleculePtr&)), SLOT(updateActiveMolecule(molconv::moleculePtr&)));
    connect(d->m_ListOfMolecules, SIGNAL(newGroupSelected(molconv::MoleculeGroup*)), d->m_MoleculeSettings, SLOT(setGroup(molconv::MoleculeGroup*)));

    connect(d->m_MoleculeSettings, SIGNAL(basisChanged()), SLOT(updateAxes()));
    connect(d->m_MoleculeSettings, SIGNAL(basisChanged()), SLOT(updateSelection()));

    GraphicsAxisItem *axes = new GraphicsAxisItem;
    ui->molconv_graphicsview->addItem(axes);

    d->m_Selection = new GraphicsSelectionItem;
    ui->molconv_graphicsview->addItem(d->m_Selection);

    d->m_navigatetool = boost::make_shared<NavigateTool>();
    d->m_selecttool = boost::make_shared<SelectTool>(this);
    useNavigateTool();

    ui->molconv_graphicsview->update();
}

MolconvWindow::~MolconvWindow()
{
    delete ui->molconv_graphicsview;
    delete ui;
}

void MolconvWindow::add_molecule(molconv::moleculePtr temp_mol)
{
    d->m_system->addMolecule(temp_mol);

    d->m_GraphicsItemVector.push_back(new chemkit::GraphicsMoleculeItem(d->m_system->getMolecule(d->m_system->nMolecules() - 1).get()));
    ui->molconv_graphicsview->addItem(d->m_GraphicsItemVector.back());

    d->m_GraphicsAxisVector.push_back(new GraphicsAxisItem(temp_mol->internalOriginPosition(), temp_mol->internalBasisVectors()));
    ui->molconv_graphicsview->addItem(d->m_GraphicsAxisVector.back());

    ui->molconv_graphicsview->update();

    d->m_ListOfMolecules->insertMolecule(temp_mol);
    d->activeMolecule = temp_mol;

    ui->actionSet_internal_basis->setEnabled(true);
    ui->actionDuplicate->setEnabled(true);
    ui->actionRemove->setEnabled(true);
    ui->actionZero_Coordinates->setEnabled(true);
    ui->actionReset_Coordinates->setEnabled(true);
    ui->actionProperties->setEnabled(true);
    ui->actionAdd_To_Group->setEnabled(true);

    emit new_molecule(temp_mol);
}

void MolconvWindow::removeActiveMolecule()
{
    molconv::Molecule *activeMolecule = d->activeMolecule.get();
    chemkit::GraphicsMoleculeItem *activeItem = 0;
    int index;

    d->m_ListOfMolecules->removeCurrentMolecule();

    for (int i = 0; i < int(d->m_GraphicsItemVector.size()); i++)
        if (d->m_GraphicsItemVector.at(i)->molecule() == activeMolecule)
        {
            index = i;
            activeItem = d->m_GraphicsItemVector.at(i);
        }

    ui->molconv_graphicsview->deleteItem(activeItem);
    ui->molconv_graphicsview->deleteItem(d->m_GraphicsAxisVector.at(index));

    d->m_GraphicsItemVector.erase(d->m_GraphicsItemVector.begin() + index);
    d->m_GraphicsAxisVector.erase(d->m_GraphicsAxisVector.begin() + index);

    d->m_system->removeMolecule(d->m_system->MoleculeIndex(d->activeMolecule));

    ui->molconv_graphicsview->update();

    if (d->m_system->nMolecules() > 0)
        d->activeMolecule = d->m_ListOfMolecules->currentMolecule();
    else
    {
        d->activeMolecule = 0;

        // disable all the actions that need a molecule to operate on:
        ui->actionSet_internal_basis->setEnabled(false);
        ui->actionDuplicate->setEnabled(false);
        ui->actionRemove->setEnabled(false);
        ui->actionZero_Coordinates->setEnabled(false);
        ui->actionReset_Coordinates->setEnabled(false);
        ui->actionProperties->setEnabled(false);
        ui->actionAdd_To_Group->setEnabled(false);
    }

    if (d->activeMolecule)
        d->m_MoleculeSettings->setMolecule(d->activeMolecule);
}

int MolconvWindow::nMolecules()
{
    return d->m_system->nMolecules();
}

molconv::moleculePtr MolconvWindow::getMol(int index)
{
    return d->m_system->getMolecule(index);
}

void MolconvWindow::toggle_molecule(molconv::moleculePtr theMolecule, bool state)
{
    size_t moleculeIndex = d->m_system->MoleculeIndex(theMolecule);
    if (state)
    {
        d->m_GraphicsItemVector.at(moleculeIndex)->show();
        d->m_GraphicsAxisVector.at(moleculeIndex)->show();
        ui->molconv_graphicsview->update();
    }
    else
    {
        d->m_GraphicsItemVector.at(moleculeIndex)->hide();
        d->m_GraphicsAxisVector.at(moleculeIndex)->hide();
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
    AboutDialog *ad = new AboutDialog;
    ad->exec();
}

void MolconvWindow::quit()
{
    QSettings settings;
    settings.setValue("startMaximized", QVariant(isMaximized()));
    settings.setValue("winW", width());
    settings.setValue("winH", height());
    settings.setValue("xPos", x());
    settings.setValue("yPos", y());

    qApp->quit();
}

void MolconvWindow::saveFile()
{
}

void MolconvWindow::openFile()
{
    openFile(d->m_ImportDialog->getFileName(), true);
}

void MolconvWindow::openFile(const QString &fileName, const bool showList)
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
                tempMol->setOrigin(d->m_ImportDialog->getOrigin(), d->m_ImportDialog->getOriginAtom());
                tempMol->setBasis(d->m_ImportDialog->getBasis(),
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
    d->m_ExportDialog->createMoleculeList();
    d->m_ExportDialog->setModal(true);
    d->m_ExportDialog->exec();
}

void MolconvWindow::startBasisDialog()
{
    d->m_setBasisDialog->prepare(d->activeMolecule);
    d->m_setBasisDialog->show();
}

void MolconvWindow::DuplicateActiveMolecule()
{
    DuplicateMolecule(d->activeMolecule);
}

void MolconvWindow::DuplicateMolecule(const molconv::moleculePtr oldMolecule)
{
    molconv::moleculePtr newMol = boost::make_shared<molconv::Molecule>(oldMolecule);

    add_molecule(newMol);
}

void MolconvWindow::newGroup()
{
    std::string newGroupName = d->m_NewGroupDialog->groupName();
    std::vector<bool> members = d->m_NewGroupDialog->molecules();

    if (d->m_NewGroupDialog->isStack())
    {
        d->m_MoleculeGroups.push_back(new molconv::MoleculeStack(newGroupName));
        for (int i = 0; i < nMolecules(); i++)
            if (members.at(i))
            {
                static_cast<molconv::MoleculeStack*>(d->m_MoleculeGroups.back())->addMolecule(getMol(i), molconv::zVec);
                getMol(i)->addToGroup(d->m_MoleculeGroups.back());
            }
    }
    else
    {
        d->m_MoleculeGroups.push_back(new molconv::MoleculeGroup(newGroupName));
        for (int i = 0; i < nMolecules(); i++)
            if (members.at(i))
            {
                d->m_MoleculeGroups.back()->addMolecule(getMol(i));
                getMol(i)->addToGroup(d->m_MoleculeGroups.back());
            }
    }

    d->m_ListOfMolecules->insertGroup(d->m_MoleculeGroups.back());
}

void MolconvWindow::startNewGroupDialog()
{
    d->m_NewGroupDialog->createMoleculeList();
    d->m_NewGroupDialog->setModal(true);
    d->m_NewGroupDialog->exec();
}

void MolconvWindow::addMoleculeToGroup()
{
}

void MolconvWindow::ResetView()
{
    // determine largest distance from origin:
    double maxLength = 0;
    for (int i = 0; i < int(d->m_system->nMolecules()); i++)
        for (int j = 0; j < int(d->m_system->getMolecule(i)->size()); j++)
        {
            double length = d->m_system->getMolecule(i)->atom(j)->position().norm();
            if (length > maxLength)
                maxLength = length;
        }

    // d = r / tan(22.5 degrees)
    double dist = maxLength / 0.4142135624 > 10.0 ? maxLength / 0.4142135624 : 10.0;

    ui->molconv_graphicsview->setCamera(boost::make_shared<chemkit::GraphicsCamera>(0,0,dist));
}

void MolconvWindow::updateActiveMolecule(molconv::moleculePtr &newActive)
{
    d->activeMolecule = newActive;
}

void MolconvWindow::changeOriginBasis()
{
    molconv::origin newOrigin = d->m_setBasisDialog->origin();
    molconv::basis newBasis = d->m_setBasisDialog->basis();

    std::array<int,2> newOriginAtoms = d->m_setBasisDialog->originAtoms();
    std::array<int,3> newBasisAtoms = d->m_setBasisDialog->basisAtoms();

    double newAtomLineScale = d->m_setBasisDialog->atomLineScale();

    std::vector<bool> newOriginList = d->m_setBasisDialog->selectedOriginAtoms();
    std::vector<bool> newBasisList = d->m_setBasisDialog->selectedBasisAtoms();

    d->activeMolecule->setOriginList(newOriginList);
    d->activeMolecule->setBasisList(newBasisList);
    d->activeMolecule->setOrigin(newOrigin, size_t(newOriginAtoms[0]), size_t(newOriginAtoms[1]), newAtomLineScale);
    d->activeMolecule->setBasis(newBasis, newBasisAtoms[0], newBasisAtoms[1], newBasisAtoms[2]);


    d->m_MoleculeSettings->setMolecule(d->activeMolecule);
    updateAxes();
}

void MolconvWindow::updateAxes()
{
    int index = d->m_system->MoleculeIndex(d->activeMolecule);

    d->m_GraphicsAxisVector.at(index)->setPosition(d->activeMolecule->internalOriginPosition());
    d->m_GraphicsAxisVector.at(index)->setVectors(d->activeMolecule->internalBasisVectors());

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

void MolconvWindow::resetCoords()
{
    std::array<double,6> oB = d->activeMolecule->origBasis();
    d->m_MoleculeSettings->moveMolecule(oB[0], oB[1], oB[2], oB[3], oB[4], oB[5]);
}

void MolconvWindow::zeroCoords()
{
    d->m_MoleculeSettings->moveMolecule(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

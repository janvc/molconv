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
#include<iomanip>
#include<cmath>
#include<map>
#include<algorithm>
#include<QMessageBox>
#include<Eigen/Eigenvalues>
#include<QDomDocument>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<chemkit/graphicscamera.h>
    #include<chemkit/bondpredictor.h>
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
#include "moleculeinfo.h"


class MolconvWindowPrivate
{
public:
    MolconvWindowPrivate()
        : m_system(new molconv::System)
    {
    }

    ImportDialog *m_ImportDialog;
    NewGroupDialog *m_NewGroupDialog;
    setBasisDialog *m_setBasisDialog;
    MultiMolDialog *m_MultiMolDialog;

    ListOfMolecules *m_ListOfMolecules;
    MoleculeSettings *m_MoleculeSettings;
    MoleculeInfo *m_MoleculeInfo;

    boost::scoped_ptr<molconv::System> m_system;

    std::vector<molconv::MoleculeGroup *> m_MoleculeGroups;
    std::map<unsigned long, chemkit::GraphicsMoleculeItem *> m_GraphicsItemMap;
    std::map<unsigned long, GraphicsAxisItem *> m_GraphicsAxisMap;
    std::vector<chemkit::Atom *> m_SelectedAtoms;

    GraphicsSelectionItem *m_Selection;
    unsigned long m_activeMolID;

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
    d->m_NewGroupDialog = new NewGroupDialog(this);
    d->m_setBasisDialog = new setBasisDialog(this);
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

    connect(qApp, SIGNAL(aboutToQuit()), SLOT(quit()));

    connect(this, SIGNAL(new_molecule(unsigned long)), d->m_MoleculeInfo, SLOT(setMolecule(unsigned long)));
    connect(this, SIGNAL(new_molecule(unsigned long)), d->m_MoleculeSettings, SLOT(setMolecule(unsigned long)));

    connect(ui->actionSave, SIGNAL(triggered()), SLOT(saveFile()));
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
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
    connect(ui->actionAlign, SIGNAL(triggered()), d->m_ListOfMolecules, SLOT(alignMolecules()));
    connect(ui->actionNavigate, SIGNAL(triggered()), SLOT(useNavigateTool()));
    connect(ui->actionSelect, SIGNAL(triggered()), SLOT(useSelectTool()));

    connect(d->m_ImportDialog, SIGNAL(accepted()), SLOT(importFile()));

    connect(d->m_NewGroupDialog, SIGNAL(accepted()), this, SLOT(newGroup()));

    connect(d->m_setBasisDialog, SIGNAL(ready()), SLOT(changeOriginBasis()));

    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(unsigned long)), d->m_MoleculeSettings, SLOT(setMolecule(unsigned long)));
    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(unsigned long)), SLOT(updateActiveMolecule(unsigned long)));
    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(unsigned long)), d->m_MoleculeInfo, SLOT(setMolecule(unsigned long)));
    connect(d->m_ListOfMolecules, SIGNAL(newGroupSelected(molconv::MoleculeGroup*)), d->m_MoleculeSettings, SLOT(setGroup(molconv::MoleculeGroup*)));

    connect(d->m_MoleculeSettings, SIGNAL(basisChanged()), SLOT(updateAxes()));
    connect(d->m_MoleculeSettings, SIGNAL(basisChanged()), SLOT(updateSelection()));
    connect(d->m_MoleculeSettings, SIGNAL(basisChanged()), d->m_MoleculeInfo, SLOT(updateLive()));
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

    molconv::Molecule::initRand();
}

MolconvWindow::~MolconvWindow()
{
    delete ui->molconv_graphicsview;
    delete ui;
    delete d;
}

void MolconvWindow::add_molecule(molconv::moleculePtr temp_mol)
{
    d->m_system->addMolecule(temp_mol);
    d->m_activeMolID = temp_mol->molId();

    chemkit::GraphicsMoleculeItem *item = new chemkit::GraphicsMoleculeItem(temp_mol.get());
    d->m_GraphicsItemMap.insert(std::make_pair(d->m_activeMolID, item));
    ui->molconv_graphicsview->addItem(item);

    GraphicsAxisItem *axis = new GraphicsAxisItem(temp_mol->internalOriginPosition(), temp_mol->internalBasisVectors());
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

void MolconvWindow::removeActiveMolecule()
{
    unsigned long molToRemove = d->m_activeMolID;

    // remove active molecule from the list
    d->m_ListOfMolecules->removeCurrentMolecule();

    // remove active molecule's graphics item
    ui->molconv_graphicsview->deleteItem(d->m_GraphicsItemMap.at(molToRemove));
    d->m_GraphicsItemMap.erase(molToRemove);

    // remove active molecule's axis item
    ui->molconv_graphicsview->deleteItem(d->m_GraphicsAxisMap.at(molToRemove));
    d->m_GraphicsAxisMap.erase(molToRemove);

    d->m_system->removeMolecule(molToRemove);

    ui->molconv_graphicsview->update();

    if (d->m_system->nMolecules() > 0)
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
    return d->m_system->nMolecules();
}

molconv::moleculePtr MolconvWindow::getMol(const unsigned long key)
{
    return d->m_system->getMolecule(key);
}

std::vector<unsigned long> MolconvWindow::getMolIDs()
{
    return d->m_system->getMolIDs();
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
    if (fileName.split(".").last() == "mcv")
        readMolconvFile(fileName);
    else
        importFile(fileName);
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
        delete molFile;
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
    ed->createMoleculeList(d->m_system->getMolIDs());
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
    molconv::moleculePtr newMol = boost::make_shared<molconv::Molecule>(d->m_system->getMolecule(oldMolID));

    add_molecule(newMol);
}

void MolconvWindow::newGroup()
{
    std::string newGroupName = d->m_NewGroupDialog->groupName();
    std::vector<bool> members = d->m_NewGroupDialog->molecules();

    // check if we have nested groups i.e. the molecules we are adding to this group are
    // already members of THE SAME existing group. That group will then be the parent of
    // this group.
    std::vector<molconv::groupPtr> groups;
    for (int i = 0; i < nMolecules(); i++)
        if (members.at(i))
            groups.push_back(getMol(i)->group());

    for (int i = 0; i < groups.size(); i++)
        if (groups[i] != groups[0])
        {
            QMessageBox::critical(this, "Error", "All molecules must be part of the same group");
            return;
        }

    if (d->m_NewGroupDialog->isStack())
    {
        molconv::groupPtr newStack(new molconv::MoleculeStack(newGroupName));

        for (int i = 0; i < nMolecules(); i++)
            if (members.at(i))
            {
                static_cast<molconv::MoleculeStack*>(newStack.get())->addMolecule(getMol(i), molconv::zVec);
                getMol(i)->addToGroup(newStack);
            }
        newStack->addToGroup(groups[0]);
        d->m_system->addGroup(newStack);
        d->m_ListOfMolecules->insertGroup(newStack.get());
    }
    else
    {
        molconv::groupPtr newGroup(new molconv::MoleculeGroup(newGroupName));

        for (int i = 0; i < nMolecules(); i++)
            if (members.at(i))
            {
                newGroup->addMolecule(getMol(i));
                getMol(i)->addToGroup(newGroup);
            }
        newGroup->addToGroup(groups[0]);
        d->m_system->addGroup(newGroup);
        d->m_ListOfMolecules->insertGroup(newGroup.get());
    }
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
    // determine largest distance of any atom from the global origin:
    double maxLength = 0;
    for (unsigned long id : d->m_system->getMolIDs())
    {
        molconv::moleculePtr mol =  d->m_system->getMolecule(id);

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
    molconv::origin newOrigin = d->m_setBasisDialog->origin();
    molconv::basis newBasis = d->m_setBasisDialog->basis();

    std::array<int,2> newOriginAtoms = d->m_setBasisDialog->originAtoms();
    std::array<int,3> newBasisAtoms = d->m_setBasisDialog->basisAtoms();

    double newAtomLineScale = d->m_setBasisDialog->atomLineScale();

    std::vector<bool> newOriginList = d->m_setBasisDialog->selectedOriginAtoms();
    std::vector<bool> newBasisList = d->m_setBasisDialog->selectedBasisAtoms();

    molconv::moleculePtr tmpMolPtr = d->m_system->getMolecule(d->m_activeMolID);
    tmpMolPtr->setOriginList(newOriginList);
    tmpMolPtr->setBasisList(newBasisList);
    tmpMolPtr->setOrigin(newOrigin, size_t(newOriginAtoms[0]), size_t(newOriginAtoms[1]), newAtomLineScale);
    tmpMolPtr->setBasis(newBasis, newBasisAtoms[0], newBasisAtoms[1], newBasisAtoms[2]);

    d->m_MoleculeSettings->setMolecule(d->m_activeMolID);
    updateAxes();
}

void MolconvWindow::updateAxes()
{
    molconv::moleculePtr tmpMolPtr = d->m_system->getMolecule(d->m_activeMolID);

    d->m_GraphicsAxisMap.at(d->m_activeMolID)->setPosition(tmpMolPtr->internalOriginPosition());
    d->m_GraphicsAxisMap.at(d->m_activeMolID)->setVectors(tmpMolPtr->internalBasisVectors());

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
    std::array<double,6> oB = d->m_system->getMolecule(d->m_activeMolID)->origBasis();
    d->m_MoleculeSettings->moveMolecule(oB[0], oB[1], oB[2], oB[3], oB[4], oB[5]);
}

void MolconvWindow::zeroCoords()
{
    d->m_MoleculeSettings->moveMolecule(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
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

void MolconvWindow::minimizeRMSD(const unsigned long refMolID, const unsigned long otherMolID)
{
    molconv::moleculePtr refMol = d->m_system->getMolecule(refMolID);
    molconv::moleculePtr otherMol = d->m_system->getMolecule(otherMolID);

    if (refMol->size() != otherMol->size())
    {
        QMessageBox::critical(this, tr("Alignment impossible"), tr("Only molecules with equal number of atoms can be aligned."));
        return;
    }

    int Natoms = refMol->size();

    Eigen::Vector3d center = refMol->center();
    Eigen::Vector3d shift = center - otherMol->center();
    Eigen::Vector3d newOrigin = otherMol->internalOriginPosition() + shift;

    updateActiveMolecule(otherMolID);
    d->m_MoleculeSettings->setMolecule(otherMolID);
    d->m_MoleculeSettings->moveMolecule(double(newOrigin(0)), double(newOrigin(1)), double(newOrigin(2)), 0.0, 0.0, 0.0);

    Eigen::MatrixXd Xr = Eigen::MatrixXd::Zero(3,Natoms);
    Eigen::MatrixXd Xo = Eigen::MatrixXd::Zero(3,Natoms);
    for (int i = 0; i < Natoms; i++)
    {
        Xr.col(i) = refMol->atom(i)->position() - center;
        Xo.col(i) = otherMol->atom(i)->position() - center;
    }

    Eigen::Matrix3d corr = Xo * Xr.transpose();

    // construct the quaternion matrix
    Eigen::Matrix4d F = Eigen::Matrix4d::Zero();
    F(0,0) =  corr(0,0) + corr(1,1) + corr(2,2);
    F(1,1) =  corr(0,0) - corr(1,1) - corr(2,2);
    F(2,2) = -corr(0,0) + corr(1,1) - corr(2,2);
    F(3,3) = -corr(0,0) - corr(1,1) + corr(2,2);
    F(0,1) =  corr(1,2) - corr(2,1);
    F(0,2) =  corr(2,0) - corr(0,2);
    F(0,3) =  corr(0,1) - corr(1,0);
    F(1,2) =  corr(0,1) + corr(1,0);
    F(1,3) =  corr(0,2) + corr(2,0);
    F(2,3) =  corr(1,2) + corr(2,1);
    F(1,0) = F(0,1);
    F(2,0) = F(0,2);
    F(3,0) = F(0,3);
    F(2,1) = F(1,2);
    F(3,1) = F(1,3);
    F(3,2) = F(2,3);

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix4d> Feig(F);
    Eigen::Vector4d Feval = Feig.eigenvalues();
    Eigen::Matrix4d Fevec = Feig.eigenvectors();

    // the optimal rotation corresponds to either the first or the last eigenvector, depending on which eigenvalue is larger
    Eigen::Vector4d lQuart = std::abs(double(Feval(0))) > std::abs(double(Feval(3))) ? Fevec.block(0, 0, 4, 1) : Fevec.block(0, 3, 4, 1);

    Eigen::Matrix3d rotmat = Eigen::Matrix3d::Zero();
    rotmat(0,0) = lQuart(0) * lQuart(0) + lQuart(1) * lQuart(1) - lQuart(2) * lQuart(2) - lQuart(3) * lQuart(3);
    rotmat(1,1) = lQuart(0) * lQuart(0) - lQuart(1) * lQuart(1) + lQuart(2) * lQuart(2) - lQuart(3) * lQuart(3);
    rotmat(2,2) = lQuart(0) * lQuart(0) - lQuart(1) * lQuart(1) - lQuart(2) * lQuart(2) + lQuart(3) * lQuart(3);
    rotmat(0,1) = 2.0 * (lQuart(1) * lQuart(2) - lQuart(0) * lQuart(3));
    rotmat(0,2) = 2.0 * (lQuart(1) * lQuart(3) + lQuart(0) * lQuart(2));
    rotmat(1,2) = 2.0 * (lQuart(2) * lQuart(3) - lQuart(0) * lQuart(1));
    rotmat(1,0) = 2.0 * (lQuart(1) * lQuart(2) + lQuart(0) * lQuart(3));
    rotmat(2,0) = 2.0 * (lQuart(1) * lQuart(3) - lQuart(0) * lQuart(2));
    rotmat(2,1) = 2.0 * (lQuart(2) * lQuart(3) + lQuart(0) * lQuart(1));

    std::array<double,3> newEulers = molconv::Molecule::rot2euler(rotmat);
    double newPhi = newEulers[2];
    double newTheta = newEulers[1];
    double newPsi = newEulers[0];

    d->m_MoleculeSettings->moveMolecule(double(newOrigin(0)), double(newOrigin(1)), double(newOrigin(2)), newPhi, newTheta, newPsi);

    updateAxes();
}

void MolconvWindow::writeMolconvFile(const QString &fileName)
{
    QDomDocument testDoc;
    QDomElement system = testDoc.createElement("System");
    testDoc.appendChild(system);

    for (unsigned long id : d->m_system->getMolIDs())
    {
        QDomElement molecule = testDoc.createElement("Molecule");
        molecule.setAttribute("Name", QString::fromStdString(d->m_system->getMolecule(id)->name()));

        QDomElement origin = testDoc.createElement("Origin");
        origin.setAttribute("Type", QString::number(d->m_system->getMolecule(id)->internalOrigin()));
        origin.setAttribute("Factor", QString::number(d->m_system->getMolecule(id)->internalOriginFactor()));

        QString atomString = QString::number(d->m_system->getMolecule(id)->internalOriginAtoms()[0]);
        atomString += ",";
        atomString += QString::number(d->m_system->getMolecule(id)->internalOriginAtoms()[1]);
        origin.setAttribute("Atoms", atomString);

        QString originList;
        for (int k = 0; k < int(d->m_system->getMolecule(id)->size() - 1); k++)
            if (d->m_system->getMolecule(id)->originList()[k])
                originList += "T,";
            else
                originList += "F,";
        if (d->m_system->getMolecule(id)->originList()[d->m_system->getMolecule(id)->size() - 1])
            originList += "T";
        else
            originList += "F";
        origin.setAttribute("originList", originList);

        origin.setAttribute("vecX", QString::number(d->m_system->getMolecule(id)->internalOriginPosition()(0), 'e', 16));
        origin.setAttribute("vecY", QString::number(d->m_system->getMolecule(id)->internalOriginPosition()(1), 'e', 16));
        origin.setAttribute("vecZ", QString::number(d->m_system->getMolecule(id)->internalOriginPosition()(2), 'e', 16));
        molecule.appendChild(origin);

        QDomElement basis = testDoc.createElement("Basis");
        basis.setAttribute("Type", QString::number(d->m_system->getMolecule(id)->internalBasis()));

        atomString = QString::number(d->m_system->getMolecule(id)->internalBasisAtoms()[0]);
        atomString += ",";
        atomString += QString::number(d->m_system->getMolecule(id)->internalBasisAtoms()[1]);
        atomString += ",";
        atomString += QString::number(d->m_system->getMolecule(id)->internalBasisAtoms()[2]);
        basis.setAttribute("Atoms", atomString);

        QString basisList;
        for (int k = 0; k < int(d->m_system->getMolecule(id)->size() - 1); k++)
            if (d->m_system->getMolecule(id)->basisList()[k])
                basisList += "T,";
            else
                basisList += "F,";
        if (d->m_system->getMolecule(id)->basisList()[d->m_system->getMolecule(id)->size() - 1])
            basisList += "T";
        else
            basisList += "F";
        basis.setAttribute("basisList", basisList);

        basis.setAttribute("phi", QString::number(d->m_system->getMolecule(id)->phi(), 'e', 16));
        basis.setAttribute("theta", QString::number(d->m_system->getMolecule(id)->theta(), 'e', 16));
        basis.setAttribute("psi", QString::number(d->m_system->getMolecule(id)->psi(), 'e', 16));
        molecule.appendChild(basis);

        for (int j = 0; j < int(d->m_system->getMolecule(id)->size()); j++)
        {
            QDomElement atom = testDoc.createElement("Atom");
            atom.setAttribute("Ele", QString::fromStdString(d->m_system->getMolecule(id)->atom(j)->element().symbol()));
            atom.setAttribute("X", QString::number(d->m_system->getMolecule(id)->internalPositions()[j](0), 'e', 16));
            atom.setAttribute("Y", QString::number(d->m_system->getMolecule(id)->internalPositions()[j](1), 'e', 16));
            atom.setAttribute("Z", QString::number(d->m_system->getMolecule(id)->internalPositions()[j](2), 'e', 16));
            molecule.appendChild(atom);
        }
        system.appendChild(molecule);
    }

    for (int i = 0; i < int(d->m_system->nGroups()); i++)
    {
        QDomElement group = testDoc.createElement("Group");
        group.setAttribute("Name", QString::fromStdString(d->m_system->getGroup(i)->name()));
        group.setAttribute("Parent", QString::number(d->m_system->GroupIndex(d->m_system->getGroup(i)->parent())));

        QString memberString;
        for (int j = 0; j < int(d->m_system->getGroup(i)->nMolecules() - 1); j++)
        {
            memberString += QString::number(d->m_system->MoleculeIndex(d->m_system->getGroup(i)->getMol(j)));
            memberString += ",";
        }
        memberString += QString::number(d->m_system->MoleculeIndex(d->m_system->getGroup(i)->getMol(d->m_system->getGroup(i)->nMolecules() - 1)));
        group.setAttribute("Members", memberString);

        system.appendChild(group);
    }

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << testDoc.toString();
    file.close();
}

void MolconvWindow::readMolconvFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QDomDocument testDoc;
    testDoc.setContent(&file);
    file.close();

    QDomElement systemElement = testDoc.documentElement();

    QDomNode moleculeNode = systemElement.firstChild();

    while (! moleculeNode.isNull())
    {
        QDomElement moleculeElement = moleculeNode.toElement();
        if (! moleculeElement.isNull())
        {
            molconv::moleculePtr currentMolecule(new molconv::Molecule);

            currentMolecule->setName(moleculeElement.attribute("Name").toStdString());

            // create the origin and the basis to transform the atoms to their global positions
            // since only the internal positions are saved in the molconv file:
            QDomElement originElement = moleculeElement.elementsByTagName("Origin").at(0).toElement();

            molconv::origin Origin = static_cast<molconv::origin>(originElement.attribute("Type").toInt());
            double vecx = originElement.attribute("vecX").toDouble();
            double vecy = originElement.attribute("vecY").toDouble();
            double vecz = originElement.attribute("vecZ").toDouble();
            QString originAtomString = originElement.attribute("Atoms");
            QString originListString = originElement.attribute("originList");
            double originFactor = originElement.attribute("Factor").toDouble();

            Eigen::Vector3d originVec(vecx, vecy, vecz);
            int oA1, oA2;
            oA1 = originAtomString.split(",").at(0).toInt();
            oA2 = originAtomString.split(",").at(1).toInt();
            std::vector<bool> originList;
            for (int i = 0; i < originListString.split(",").length(); i++)
                if (originListString.split(",").at(i) == "T")
                    originList.push_back(true);
                else
                    originList.push_back(false);

            QDomElement basisElement = moleculeElement.elementsByTagName("Basis").at(0).toElement();

            molconv::basis Basis = static_cast<molconv::basis>(basisElement.attribute("Type").toInt());
            double phi = basisElement.attribute("phi").toDouble();
            double theta = basisElement.attribute("theta").toDouble();
            double psi = basisElement.attribute("psi").toDouble();
            QString basisAtomString = basisElement.attribute("Atoms");
            QString basisListString = basisElement.attribute("basisList");

            int bA1, bA2, bA3;
            bA1 = basisAtomString.split(",").at(0).toInt();
            bA2 = basisAtomString.split(",").at(1).toInt();
            bA3 = basisAtomString.split(",").at(2).toInt();
            std::vector<bool> basisList;
            for (int i = 0; i < basisListString.split(",").length(); i++)
                if (basisListString.split(",").at(i) == "T")
                    basisList.push_back(true);
                else
                    basisList.push_back(false);

            Eigen::MatrixXd trafo(molconv::Molecule::euler2rot(psi, theta, phi));

            QDomNode atomNode = basisElement.nextSibling();

            while (! atomNode.isNull())
            {
                QDomElement atomElement = atomNode.toElement();
                if (! atomElement.isNull())
                {
                    double x = atomElement.attribute("X").toDouble();
                    double y = atomElement.attribute("Y").toDouble();
                    double z = atomElement.attribute("Z").toDouble();
                    std::string element = atomElement.attribute("Ele").toStdString();

                    Eigen::Vector3d intPos(x, y, z);
                    Eigen::Vector3d globalPos(originVec + trafo * intPos);

                    chemkit::Atom *currentAtom = currentMolecule->addAtom(chemkit::Element(element));


                    currentAtom->setPosition(globalPos);
                }
                atomNode = atomNode.nextSibling();
            }

            currentMolecule->setOrigin(Origin, oA1, oA2, originFactor);
            currentMolecule->setOriginList(originList);
            currentMolecule->setBasis(Basis, bA1, bA2, bA3);
            currentMolecule->setBasisList(basisList);

            chemkit::BondPredictor::predictBonds(currentMolecule.get());
            add_molecule(currentMolecule);
        }
        moleculeNode = moleculeNode.nextSibling();
    }
}

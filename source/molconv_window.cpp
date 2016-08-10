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
#include<QDebug>
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
#include "open_dialog.h"
#include "export_dialog.h"
#include "graphicsaxisitem.h"
#include "aboutdialog.h"


class MolconvWindowPrivate
{
public:
    MolconvWindowPrivate()
        : m_system(new molconv::System)
    {
    }

    OpenDialog *m_OpenDialog;
    ExportDialog *m_ExportDialog;
    NewGroupDialog *m_NewGroupDialog;

    ListOfMolecules *m_ListOfMolecules;
    MoleculeSettings *m_MoleculeSettings;

    molconv::sysPtr m_system;

    std::vector<molconv::MoleculeGroup *> m_MoleculeGroups;
    std::vector<molconv::MoleculeStack *> m_MoleculeStacks;
    std::vector<chemkit::GraphicsMoleculeItem *> m_GraphicsItemVector;

    molconv::moleculePtr activeMolecule;
};


MolconvWindow::MolconvWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , d(new MolconvWindowPrivate)
    , ui(new Ui::MolconvWindow)
{
    qDebug("this is the constructor of MolconvWindow");

    ui->setupUi(this);

    d->m_OpenDialog = new OpenDialog(this);
    d->m_ExportDialog = new ExportDialog(this);
    d->m_NewGroupDialog = new NewGroupDialog(this);


    connect(d->m_OpenDialog, SIGNAL(accepted()), this, SLOT(getMoleculeDialog()));
    connect(d->m_NewGroupDialog, SIGNAL(accepted()), this, SLOT(newGroup()));

    connect(ui->actionImport_Molecule, SIGNAL(triggered()), SLOT(startOpenDialog()));
    connect(ui->actionExport_Molecule, SIGNAL(triggered()), SLOT(startExportDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionNew_Molecule_Group, SIGNAL(triggered()), SLOT(startNewGroupDialog()));

    connect(ui->actionRemove, SIGNAL(triggered()), SLOT(removeActiveMolecule()));

    connect(ui->actionReset, SIGNAL(triggered()), SLOT(ResetView()));

    d->m_ListOfMolecules = new ListOfMolecules(this);
    d->m_MoleculeSettings = new MoleculeSettings(this);
    addDockWidget(Qt::BottomDockWidgetArea, d->m_ListOfMolecules);
    addDockWidget(Qt::LeftDockWidgetArea, d->m_MoleculeSettings);

    connect(d->m_ListOfMolecules, SIGNAL(newMoleculeSelected(molconv::moleculePtr&)), d->m_MoleculeSettings, SLOT(setMolecule(molconv::moleculePtr&)));

    GraphicsAxisItem *axes = new GraphicsAxisItem;
    ui->molconv_graphicsview->addItem(axes);

    ui->molconv_graphicsview->update();
}

MolconvWindow::~MolconvWindow()
{
    qDebug("this is the destructor of MolconvWindow");
    delete ui->molconv_graphicsview;
    delete ui;
}

void MolconvWindow::add_molecule(molconv::moleculePtr temp_mol)
{
    qDebug("entering MolconvWindow::add_molecule(temp_mol)");

    d->m_system->addMolecule(temp_mol);
    d->m_GraphicsItemVector.push_back(new chemkit::GraphicsMoleculeItem(d->m_system->getMolecule(d->m_system->nMolecules() - 1).get()));
    ui->molconv_graphicsview->addItem(d->m_GraphicsItemVector.back());
    ui->molconv_graphicsview->update();

    d->m_ListOfMolecules->insertMolecule(temp_mol);
    d->activeMolecule = temp_mol;

    emit new_molecule(temp_mol);
}

void MolconvWindow::removeActiveMolecule()
{
    d->activeMolecule = d->m_ListOfMolecules->currentMolecule();

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

    d->m_GraphicsItemVector.erase(d->m_GraphicsItemVector.begin() + index);

    d->m_system->removeMolecule(d->m_system->MoleculeIndex(d->activeMolecule));

    ui->molconv_graphicsview->update();

    d->activeMolecule = d->m_ListOfMolecules->currentMolecule();
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
    qDebug("entering MolconvWindow::toggle_molecule()");

    size_t moleculeIndex = d->m_system->MoleculeIndex(theMolecule);
    if (state)
    {
        d->m_GraphicsItemVector.at(moleculeIndex)->show();
        ui->molconv_graphicsview->update();
    }
    else
    {
        d->m_GraphicsItemVector.at(moleculeIndex)->hide();
        ui->molconv_graphicsview->update();
    }
}

void MolconvWindow::about()
{
    AboutDialog *ad = new AboutDialog;
    ad->exec();
}

void MolconvWindow::quit()
{
    qDebug("entering MolconvWindow::quit()");
    qApp->quit();
}

void MolconvWindow::saveFile()
{
    qDebug("entering MolconvWindow::saveFile()");
}

void MolconvWindow::startOpenDialog()
{
    qDebug("entering MolconvWindow::startOpenDialog()");

    d->m_OpenDialog->setModal(true);
    d->m_OpenDialog->exec();
}

void MolconvWindow::startExportDialog()
{
    qDebug("entering MolconvWindow::startExportDialog()");

    d->m_ExportDialog->createMoleculeList();
    d->m_ExportDialog->setModal(true);
    d->m_ExportDialog->exec();
}

void MolconvWindow::getMoleculeDialog()
{
    qDebug("entering MolconvWindow::get_molecule_Dialog()");

    molconv::moleculePtr temp_mol = d->m_OpenDialog->getMol();
    add_molecule(temp_mol);
}

void MolconvWindow::DuplicateMolecule(const molconv::moleculePtr oldMolecule)
{
    qDebug("entering MolconvWindow::DuplicateMolecule()");

    molconv::moleculePtr newMol = boost::make_shared<molconv::Molecule>(oldMolecule);

    add_molecule(newMol);
}

void MolconvWindow::newGroup()
{
    qDebug("entering MolconvWindow::newGroup()");

    std::string newGroupName = d->m_NewGroupDialog->groupName();

    if (d->m_NewGroupDialog->isStack())
        d->m_MoleculeStacks.push_back(new molconv::MoleculeStack(newGroupName));
    else
        d->m_MoleculeGroups.push_back(new molconv::MoleculeGroup(newGroupName));
}

void MolconvWindow::startNewGroupDialog()
{
    qDebug("entering MolconvWindow::startNewGroupDialog()");

    d->m_NewGroupDialog->setModal(true);
    d->m_NewGroupDialog->exec();
}

void MolconvWindow::addMoleculeToGroup()
{
    qDebug("entering MolconvWindow::addMoleculeToGroup()");
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

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

#include<iostream>
#include<QDebug>
#include<QMessageBox>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<boost/make_shared.hpp>
#endif

#include "include/types.h"
#include "molconv_window.h"
#include "ui_molconv_window.h"
#include "listofmolecules.h"
#include "moleculesettings.h"
#include "open_dialog.h"


class MolconvWindowPrivate
{
public:
    OpenDialog *m_OpenDialog;
    NewGroupDialog *m_NewGroupDialog;

    ListOfMolecules *m_ListOfMolecules;
    QDockWidget *m_MoleculeSettings;

    molconv::System m_system;

    std::vector<molconv::MoleculeGroup *> m_MoleculeGroups;
    std::vector<molconv::MoleculeStack *> m_MoleculeStacks;
    std::vector<chemkit::GraphicsMoleculeItem *> m_GraphicsItemVector;
};


MolconvWindow::MolconvWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , d(new MolconvWindowPrivate)
    , ui(new Ui::MolconvWindow)
{
    qDebug("this is the constructor of MolconvWindow");

    ui->setupUi(this);
    d->m_OpenDialog = new OpenDialog(this);
    d->m_NewGroupDialog = new NewGroupDialog(this);

    connect(d->m_OpenDialog, SIGNAL(accepted()), this, SLOT(getMoleculeDialog()));
    connect(d->m_NewGroupDialog, SIGNAL(accepted()), this, SLOT(newGroup()));

    connect(ui->actionImport_Molecule, SIGNAL(triggered()), SLOT(startOpenDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionDuplicate, SIGNAL(triggered()), SLOT(DuplicateSelectedMolecule()));
    connect(ui->actionNew_Molecule_Group, SIGNAL(triggered()), SLOT(startNewGroupDialog()));

    d->m_ListOfMolecules = new ListOfMolecules(this);
    d->m_MoleculeSettings = new MoleculeSettings(this);
    addDockWidget(Qt::BottomDockWidgetArea, d->m_ListOfMolecules);
    addDockWidget(Qt::LeftDockWidgetArea, d->m_MoleculeSettings);

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

    d->m_system.addMolecule(temp_mol);
    d->m_GraphicsItemVector.push_back(new chemkit::GraphicsMoleculeItem(d->m_system.getMolecule(d->m_system.size() - 1).get()));
    ui->molconv_graphicsview->addItem(d->m_GraphicsItemVector.back());
    ui->molconv_graphicsview->update();

    d->m_ListOfMolecules->list_new_molecule(temp_mol);
}

void MolconvWindow::toggle_molecule(molconv::moleculePtr theMolecule, bool state)
{
    qDebug("entering MolconvWindow::toggle_molecule()");

    size_t moleculeIndex = d->m_system.MoleculeIndex(theMolecule);
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
    QMessageBox::about(this, tr("About molconv"),
                tr("<b>molconv</b> is a programm to manipulate molecules."));
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

void MolconvWindow::saveFile(const size_t index, const QString &filename)
{
    qDebug("entering MolconvWindow::saveFile(filename)");
}

void MolconvWindow::startOpenDialog()
{
    qDebug("entering MolconvWindow::startOpenDialog()");

    d->m_OpenDialog->setModal(true);
    d->m_OpenDialog->exec();
}

void MolconvWindow::getMoleculeDialog()
{
    qDebug("entering MolconvWindow::get_molecule_Dialog()");

    molconv::moleculePtr temp_mol = d->m_OpenDialog->getMol();
    temp_mol->cleanUp();
    add_molecule(temp_mol);
}

void MolconvWindow::DuplicateMolecule(const molconv::moleculePtr oldMolecule)
{
    qDebug("entering MolconvWindow::DuplicateMolecule()");

    molconv::moleculePtr newMol = boost::make_shared<molconv::Molecule>(oldMolecule);

    add_molecule(newMol);
}

void MolconvWindow::DuplicateSelectedMolecule()
{
    qDebug("entering MolconvWindow::DuplicateSelectedMolecule()");

    DuplicateMolecule(d->m_ListOfMolecules->getSelectedMolecule());
}

void MolconvWindow::newGroup()
{
    qDebug("entering MolconvWindow::newGroup()");

    std::string newGroupName = d->m_NewGroupDialog->groupName();
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

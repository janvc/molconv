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
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<boost/make_shared.hpp>
#endif

#include"molconv_window.h"
#include"ui_molconv_window.h"
#include"listofmolecules.h"
#include"open_molecule_dialog.h"


molconv_window::molconv_window(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::molconv_window)
{
    this->the_molfile = 0;

    ui->setupUi(this);
    this->open_dialog = new open_molecule_dialog(this);
    connect(this->open_dialog, SIGNAL(accepted()), this, SLOT(get_molecule_Dialog()));
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));

    QDockWidget *the_dockwidget;

    the_dockwidget = new ListOfMolecules(this);
    addDockWidget(Qt::LeftDockWidgetArea, the_dockwidget);
}

molconv_window::~molconv_window()
{
    delete ui->molconv_graphicsview;
    delete ui;
}

void molconv_window::openFile(const QString &filename)
{
    std::cout << "Opening file " << filename.toStdString() << std::endl;
    this->the_molfile = new chemkit::MoleculeFile(filename.toStdString());
    if (! this->the_molfile->read())
    {
        std::cerr << "Could not read molecule file " << filename.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(this->the_molfile->errorString().c_str()));
        delete this->the_molfile;
        return;
    }
    if (this->the_molfile->moleculeCount() > 0)
    {
        chemkit::Molecule temp_mol = *this->the_molfile->molecule();
        this->the_molecule_objects.push_back(temp_mol);
    }
}

void molconv_window::add_molecule()
{
    this->the_molecule_pointers.push_back(boost::make_shared<molconv::Molecule>(this->the_molecule_objects.back()));
    this->the_graph_item = new chemkit::GraphicsMoleculeItem(this->the_molecule_pointers.back().get());
    this->ui->molconv_graphicsview->addItem(this->the_graph_item);
    ui->molconv_graphicsview->update();
    emit new_molecule(this->the_molecule_pointers.back().get());
}

void molconv_window::add_molecule(chemkit::Molecule temp_mol)
{
    this->the_molecule_objects.push_back(temp_mol);
    this->the_molecule_pointers.push_back(boost::make_shared<molconv::Molecule>(this->the_molecule_objects.back()));
    this->the_graph_item = new chemkit::GraphicsMoleculeItem(this->the_molecule_pointers.back().get());
    this->ui->molconv_graphicsview->addItem(this->the_graph_item);
    ui->molconv_graphicsview->update();
    emit new_molecule(this->the_molecule_pointers.back().get());
}

void molconv_window::quit()
{
    qApp->quit();
}

void molconv_window::closeFile()
{
}

void molconv_window::clean_up(const int mol_nr, const molconv::configuration &config)
{
    this->the_molecule_objects.at(mol_nr).clean_up(config);
}

void molconv_window::set_intbasis(const int mol_nr, const molconv::configuration &config)
{
    this->the_molecule_objects.at(mol_nr).set_intbasis(config);
}

void molconv_window::saveFile()
{
}

void molconv_window::saveFile(const QString &filename)
{
    std::cout << "Saving file " << filename.toStdString() << std::endl;

    if (this->the_molfile)
        delete this->the_molfile;

    this->the_molfile = new chemkit::MoleculeFile(filename.toStdString());
    this->the_molfile->addMolecule(this->the_molecule_pointers.front());

    if (! this->the_molfile->write())
    {
        std::cerr << "Could not write molecule file " << filename.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("Error writing to file: %1").arg(this->the_molfile->errorString().c_str()));
        delete this->the_molfile;
        return;
    }
}

void molconv_window::openDialog()
{
    //if(!this->open_dialog)
    //{
    //    this->open_dialog = new open_molecule_dialog(this);
    //    connect(this->open_dialog, SIGNAL(accepted()), this, SLOT(get_molecule_Dialog()));
    //}
    this->open_dialog->setModal(true);
    this->open_dialog->exec();
}

void molconv_window::get_molecule_Dialog()
{
    molconv::Molecule temp_mol = this->open_dialog->getMol();
    temp_mol.clean_up();
    add_molecule(temp_mol);
}

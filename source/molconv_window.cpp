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

//#include<chemkit/moleculefile.h>
//#include<chemkit/graphicsmoleculeitem.h>
//#include<boost/make_shared.hpp>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<boost/make_shared.hpp>
#endif
#include"molconv_window.h"
#include"ui_molconv_window.h"


molconv_window::molconv_window(QMainWindow *parent)
	: QMainWindow(parent)
	, ui(new Ui::molconv_window)
{
	this->the_molfile = 0;

	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
	connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));
}

molconv_window::~molconv_window()
{
    for (int i = 0; i < this->the_graph_mol_items.size(); i++)
    {
        delete this->the_graph_mol_items.at(i);
    }
	delete ui->molconv_graphicsview;
	delete ui;
}

void molconv_window::openFile(const QString &filename)
{
	std::cout << "Opening file " << filename.toStdString() << std::endl;

	this->the_molfile = new chemkit::MoleculeFile(filename.toStdString());
	if (! this->the_molfile->read())
	{
		QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(this->the_molfile->errorString().c_str()));
		delete this->the_molfile;
		return;
	}

	if (this->the_molfile->moleculeCount() > 0)
	{
		//molconv::Molecule my_molecule(*(the_file->molecule().get()));
		//boost::shared_ptr<molconv::Molecule> mol_point = boost::dynamic_pointer_cast<molconv::Molecule>(the_file->molecule());
		//boost::shared_ptr<molconv::Molecule> mol_point = boost::make_shared<molconv::Molecule>(my_molecule);
		//this->add_molecule(boost::dynamic_pointer_cast<molconv::Molecule>(this->the_molfile->molecule()));
		this->the_molecules.push_back(*(this->the_molfile->molecule().get()));
        this->add_molecule(boost::make_shared<molconv::Molecule>(this->the_molecules.back()));
	}
}

void molconv_window::openFile()
{
	std::vector<std::string> formats = chemkit::MoleculeFile::formats();
	std::sort(formats.begin(), formats.end());

	QString formatsString;
	foreach(const std::string &format, formats)
		formatsString += QString("*.%1 ").arg(format.c_str());

	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), 0, QString("Molecule Files (%1);;All Files (*.*)").arg(formatsString));

	if (! filename.isEmpty())
		this->openFile(filename);
}

void molconv_window::add_molecule(const boost::shared_ptr<molconv::Molecule> &molecule)
{
	std::cout << "Adding molecule" << std::endl;
	boost::shared_ptr<molconv::Molecule> temp_molecule = molecule;
	//this->the_molecules.push_back(temp_molecule);
	//molecule.get()->show_inertia();
	//molecule.get()->show_covar();
	//chemkit::GraphicsMoleculeItem *graphitem = new chemkit::GraphicsMoleculeItem(molecule.get());
    this->the_graph_mol_items.push_back(new chemkit::GraphicsMoleculeItem(temp_molecule.get()));
    this->ui->molconv_graphicsview->addItem(this->the_graph_mol_items.back());
}

void molconv_window::quit()
{
	qApp->quit();
}

void molconv_window::closeFile()
{
}

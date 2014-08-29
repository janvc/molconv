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


#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<boost/make_shared.hpp>
#endif

#include"molconv_window.h"
#include"ui_molconv_window.h"
#include"molecule_list.h"


molconv_window::molconv_window(QMainWindow *parent)
	: QMainWindow(parent)
	, ui(new Ui::molconv_window)
{
	this->the_molfile = 0;

	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
	connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));

	QDockWidget *the_dockwidget;

	the_dockwidget = new molecule_list(this);
	addDockWidget(Qt::BottomDockWidgetArea, the_dockwidget);
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
		std::cerr << "Could not read molecule file " << filename.toStdString() << std::endl;
		QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(this->the_molfile->errorString().c_str()));
		delete this->the_molfile;
		return;
	}

	if (this->the_molfile->moleculeCount() > 0)
	{
		//this->add_molecule();
		chemkit::Molecule temp_mol = *this->the_molfile->molecule();
		this->the_molecule_objects.push_back(temp_mol);
		//this->the_molecule_pointers.push_back(boost::make_shared<molconv::Molecule>(this->the_molecule_objects.back()));
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
	{
		this->openFile(filename);
		this->add_molecule();
	}
}

void molconv_window::add_molecule()
{
	//std::cout << "Adding molecule" << std::endl;
	//chemkit::Molecule temp_mol = *this->the_molfile->molecule();
	//this->the_molecule_objects.push_back(temp_mol);
	//this->the_molecule_pointers.push_back(boost::make_shared<molconv::Molecule>(this->the_molecule_objects.back()));
	//boost::shared_ptr<molconv::Molecule> temp_pointer(boost::make_shared<molconv::Molecule>(this->the_molecule_objects.back()));
	this->the_molecule_pointers.push_back(boost::make_shared<molconv::Molecule>(this->the_molecule_objects.back()));
	this->the_graph_item = new chemkit::GraphicsMoleculeItem(this->the_molecule_pointers.back().get());
	this->ui->molconv_graphicsview->addItem(this->the_graph_item);
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

void molconv_window::saveFile()
{
}

void molconv_window::saveFile(const QString &filename)
{
	std::cout << "Saving file " << filename.toStdString() << std::endl;

	if (this->the_molfile)
		delete this->the_molfile;

	this->the_molfile = new chemkit::MoleculeFile(filename.toStdString());
	//this->the_molfile->addMolecule(boost::make_shared<chemkit::Molecule>(this->the_molecule_objects.front()));
	this->the_molfile->addMolecule(this->the_molecule_pointers.front());

	if (! this->the_molfile->write())
	{
		std::cerr << "Could not write molecule file " << filename.toStdString() << std::endl;
		QMessageBox::critical(this, "Error", QString("Error writing to file: %1").arg(this->the_molfile->errorString().c_str()));
		delete this->the_molfile;
		return;
	}
}

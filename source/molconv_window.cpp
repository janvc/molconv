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

#include<chemkit/moleculefile.h>
#include<chemkit/graphicsmoleculeitem.h>
#include<boost/make_shared.hpp>
#include"molconv_window.h"
#include"ui_molconv_window.h"


molconv_window::molconv_window(QMainWindow *parent)
	: QMainWindow(parent)
	, ui(new Ui::molconv_window)
{
	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
}

molconv_window::~molconv_window()
{
	delete this->ui;
}

void molconv_window::openFile(const QString &filename)
{
	chemkit::MoleculeFile *the_file = new chemkit::MoleculeFile(filename.toStdString());
	if (! the_file->read())
	{
		QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(the_file->errorString().c_str()));
		delete the_file;
		return;
	}

	if (the_file->moleculeCount() > 0)
	{
		molconv::Molecule my_molecule(*(the_file->molecule().get()));
		boost::shared_ptr<molconv::Molecule> mol_point = boost::make_shared<molconv::Molecule>(my_molecule);
		this->add_molecule(mol_point);
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

void molconv_window::add_molecule(boost::shared_ptr<molconv::Molecule> &molecule)
{
	chemkit::GraphicsMoleculeItem *graphitem = new chemkit::GraphicsMoleculeItem(molecule.get());
	this->ui->molconv_graphicsview->addItem(graphitem);
}

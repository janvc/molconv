/*
 * Copyright 2014 Jan von Cosel
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


#ifndef OPEN_MOLECULE_DIALOG_H
#define OPEN_MOLECULE_DIALOG_H

#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
#endif
#include<QDialog>
#include<QtGui>
#include"molecule.h"

namespace Ui
{
    class open_molecule_dialog;
}

class open_molecule_dialog : public QDialog
{
    Q_OBJECT

public:
    open_molecule_dialog(QWidget *parent = 0);
    ~open_molecule_dialog();
    void openFile(const QString &filename);
    chemkit::Molecule getMol();
    molconv::origin getOrigin();
    molconv::basis getBasis();

private slots:
    void on_filedialog_clicked();

    void on_coa_toggled(bool checked);

    void on_atoms_toggled(bool checked);

private:
    Ui::open_molecule_dialog *ui;
    chemkit::Molecule the_molecule;
};

#endif // OPEN_MOLECULE_DIALOG_H

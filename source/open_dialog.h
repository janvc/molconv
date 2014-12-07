/*
 * Copyright 2014 Sebastian Lenz
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


#ifndef OPEN_DIALOG_H
#define OPEN_DIALOG_H

#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<boost/shared_ptr.hpp>
#endif
#include<QDialog>
#include<QtWidgets>
#include "molconv-molecule/molecule.h"

namespace Ui
{
    class OpenDialog;
}

class OpenDialog : public QDialog
{
    Q_OBJECT

public:
    OpenDialog(QWidget *parent = 0);
    ~OpenDialog();
    void openFile(const QString &filename);
    molconv::moleculePtr getMol();
    molconv::origin getOrigin();
    molconv::basis getBasis();
    std::string getMoleculeName();

private slots:
    void on_filedialog_clicked();
    void on_coa_toggled(bool checked);
    void on_atoms_toggled(bool checked);
    void atoms_changed(int useless);

private:
    Ui::OpenDialog *ui;
    molconv::moleculePtr the_molecule;
};

#endif // OPEN_DIALOG_H

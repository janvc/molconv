/*
 * Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz
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
#include "molecule.h"

class ImportDialogPrivate;

namespace Ui
{
    class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    ImportDialog(QWidget *parent = 0);
    ~ImportDialog();
    void openFile();
    molconv::OriginCode getOrigin() const;
    molconv::BasisCode getBasis() const;
    int getOriginAtom() const;
    std::array<int,3> getBasisAtoms() const;
    QString getMoleculeName() const;
    QString getFileName() const;

private slots:
    void on_filedialog_clicked();
    void on_coa_toggled(bool checked);
    void on_atoms_toggled(bool checked);
    void atoms_changed();
    void on_coc_toggled(bool checked);
    void on_com_toggled(bool checked);
    void on_cog_toggled(bool checked);
    void on_stdori_toggled(bool checked);
    void on_inert_toggled(bool checked);
    void on_covar_toggled(bool checked);
    void on_an_valueChanged(int arg1);

private:
    ImportDialogPrivate *d;
    Ui::ImportDialog *ui;
};

#endif // OPEN_DIALOG_H

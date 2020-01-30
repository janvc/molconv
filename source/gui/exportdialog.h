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

#ifndef EXPORT_DIALOG_H
#define EXPORT_DIALOG_H

#include <QDialog>
#include "molecule.h"
#include "molconvwindow.h"

namespace Ui
{
    class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

    void createMoleculeList(const std::vector<unsigned long> &ids);
    void writeFile(const QString &filename, const molconv::moleculePtr &theMolecule) const;

private slots:
    void on_buttonBox_accepted();
    void on_selectAllBox_clicked(bool checked);

private:
    Ui::ExportDialog *ui;
    MolconvWindow *theWindow;
};

#endif // EXPORT_DIALOG_H

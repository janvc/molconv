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


#ifndef MULTIMOL_DIALOG_H
#define MULTIMOL_DIALOG_H

#include <QDialog>
#include "molecule.h"

namespace Ui
{
    class MultiMolDialog;
}

class MultiMolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiMolDialog(QWidget *parent = 0);
    ~MultiMolDialog();

    void createMoleculeList(chemkit::MoleculeFile *file);
    std::vector<bool> molecules() const;

private slots:
    void on_MultiMolDialog_rejected();
    void on_selectAllBox_clicked(bool checked);

private:
    Ui::MultiMolDialog *ui;
};

#endif // MULTIMOL_DIALOG_H

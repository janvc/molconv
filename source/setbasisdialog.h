/*
 * Copyright 2014 - 2016 Jan von Cosel & Sebastian Lenz
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


#ifndef SETBASISDIALOG_H
#define SETBASISDIALOG_H

#include <QDialog>

#include "molconv_window.h"

namespace Ui
{
    class setBasisDialog;
}

class setBasisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setBasisDialog(QWidget *parent = 0);
    ~setBasisDialog();

    void prepare(molconv::moleculePtr &molecule);

    molconv::origin origin() const;
    molconv::basis basis() const;
    std::array<int,2> originAtoms() const;
    std::array<int,3> basisAtoms() const;

    std::vector<bool> selectedOriginAtoms() const;
    std::vector<bool> selectedBasisAtoms() const;

private slots:
    void on_originAllCheckBox_stateChanged();
    void on_basisAllCheckBox_stateChanged();
    void on_originAtomPos_toggled(bool checked);
    void on_originAtomLine_toggled(bool checked);
    void on_basisAtoms_toggled(bool checked);
    void on_originALineScaleSlider_valueChanged(int value);
    void on_originALineScaleBox_valueChanged(double value);
    void on_buttonBox_accepted();
    void on_originCOM_toggled(bool checked);
    void on_originCOG_toggled(bool checked);
    void on_originCOC_toggled(bool checked);
    void on_basisInert_toggled(bool checked);
    void on_basisCovar_toggled(bool checked);
    void on_basisCharge_toggled(bool checked);

private:
    molconv::moleculePtr m_molecule;
    molconv::origin m_origin;
    molconv::basis m_basis;

    Ui::setBasisDialog *ui;
    MolconvWindow *m_window;

    std::array<int,2> m_originAtoms;
    std::array<int,3> m_basisAtoms;

    std::vector<bool> m_originList;
    std::vector<bool> m_basisList;
};

#endif // SETBASISDIALOG_H

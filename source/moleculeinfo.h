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

#ifndef MOLECULEINFO_H
#define MOLECULEINFO_H

#include <QDockWidget>
#include "molecule.h"

class MolconvWindow;

namespace Ui {
class MoleculeInfo;
}

class MoleculeInfo : public QDockWidget
{
    Q_OBJECT

public:
    explicit MoleculeInfo(MolconvWindow *window = 0);
    ~MoleculeInfo();

public slots:
    void setMolecule(const unsigned long newMol);
    void updateLive();
    void updateMan();

private slots:
    void on_nDigits_valueChanged(int nDigits);
    void on_doLiveUpdate_toggled(bool isChecked);

private:
    void update();
    Ui::MoleculeInfo *ui;
    MolconvWindow *m_window;
    unsigned long m_molID;
    int m_aP_prec;
    bool m_updateLive;
};

#endif // MOLECULEINFO_H

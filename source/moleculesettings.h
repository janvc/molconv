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

#ifndef MOLECULESETTINGS_H
#define MOLECULESETTINGS_H

#include <QDockWidget>

#include "molconv_window.h"

namespace Ui
{
    class MoleculeSettings;
}

class MolconvWindow;

class MoleculeSettings : public QDockWidget
{
    Q_OBJECT

public:
    //explicit MoleculeSettings(MolconvWindow *window);
    MoleculeSettings(MolconvWindow *window);
    ~MoleculeSettings();

    molconv::moleculePtr molecule() const;

public slots:
    void setMolecule(molconv::moleculePtr &newMolecule);

private slots:
    void on_xSlider_valueChanged(int value);
    void on_ySlider_valueChanged(int value);
    void on_zSlider_valueChanged(int value);
    void on_xSpinBox_valueChanged(double value);
    void on_ySpinBox_valueChanged(double value);
    void on_zSpinBox_valueChanged(double value);

    void on_phiSlider_valueChanged(int value);

    void on_phiSpinBox_valueChanged(double value);

private:
    void setValues();

    Ui::MoleculeSettings *ui;
    MolconvWindow *main_window;
    molconv::moleculePtr m_molecule;
};

#endif // MOLECULESETTINGS_H

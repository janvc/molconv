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


#include "moleculeorigingeometriccenter.h"

namespace molconv {

MoleculeOriginGeometricCenter::MoleculeOriginGeometricCenter(moleculePtr molecule)
    : MoleculeOrigin(molecule)
{
    // initialize the list of atoms contributing to the geometric center
    for (int i = 0; i < int(molecule->size()); i++)
    {
        m_originList.push_back(true);
    }

    calculatePosition();
}

void MoleculeOriginGeometricCenter::calculatePosition()
{
    Eigen::Vector3d cog = Eigen::Vector3d::Zero();
    int Nactive = 0;
    for (int i = 0; i < int(molecule->size()); i++)
    {
        if (m_originList.at(i))
        {
            cog += molecule->atom(i)->position();
            Nactive++;
        }
    }

    m_position = cog / double(Nactive);
}

Eigen::Vector3d MoleculeOriginGeometricCenter::position()
{
    return m_position;
}

std::vector<bool> MoleculeOriginGeometricCenter::originList() const
{
    return m_originList;
}

}


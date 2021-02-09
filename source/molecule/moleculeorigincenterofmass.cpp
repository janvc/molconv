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


#include "moleculeorigincenterofmass.h"

namespace molconv {

MoleculeOriginCenterOfMass::MoleculeOriginCenterOfMass(moleculePtr molecule, std::vector<bool> originList)
    : MoleculeOriginGlobal(molecule, originList)
{
    Eigen::Vector3d centerOfMass = Eigen::Vector3d::Zero();
    double totalMass = 0.0;

    for (int i = 0; i < int(m_molecule->size()); i++)
    {
        if (m_originList.at(i))
        {
            centerOfMass += m_molecule->atom(i)->position() * m_molecule->atom(i)->mass();
            totalMass += m_molecule->atom(i)->mass();
        }
    }

    m_position = centerOfMass / totalMass;
}

MoleculeOriginCenterOfMass::MoleculeOriginCenterOfMass(const MoleculeOriginCenterOfMass &origin)
    : MoleculeOriginGlobal(origin)
{
    m_molecule = origin.molecule();
    m_position = origin.position();
    m_originList = origin.originList();
}

MoleculeOrigin *MoleculeOriginCenterOfMass::clone()
{
    return new MoleculeOriginCenterOfMass(*this);
}

OriginCode MoleculeOriginCenterOfMass::code() const
{
    return kCenterOfMass;
}

}

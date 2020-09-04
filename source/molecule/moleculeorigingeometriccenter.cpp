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

MoleculeOriginGeometricCenter::MoleculeOriginGeometricCenter(moleculePtr molecule, std::vector<bool> originList)
    : MoleculeOriginGlobal(molecule, originList)
{
    Eigen::Vector3d center = Eigen::Vector3d::Zero();
    int Nactive = 0;

    for (int i = 0; i < int(m_molecule->size()); i++)
    {
        if (m_originList.at(i))
        {
            center += m_molecule->atom(i)->position();
            Nactive++;
        }
    }

    m_position = center / double(Nactive);
}

MoleculeOriginGeometricCenter::MoleculeOriginGeometricCenter(const MoleculeOriginGeometricCenter &origin)
{
    m_molecule = origin.molecule();
    m_position = origin.position();
    m_originList = origin.originList();
}

MoleculeOrigin *MoleculeOriginGeometricCenter::clone()
{
    return new MoleculeOriginGeometricCenter(*this);
}

OriginCode MoleculeOriginGeometricCenter::code() const
{
    return kCenterOfGeometry;
}

}


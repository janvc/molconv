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


#include "moleculeorigin.h"

namespace molconv {

MoleculeOrigin::MoleculeOrigin()
{
}

MoleculeOrigin::MoleculeOrigin(moleculePtr molecule)
{
    m_molecule = molecule;
    m_position = Eigen::Vector3d::Zero();
}

moleculePtr MoleculeOrigin::molecule() const
{
    return m_molecule;
}

Eigen::Vector3d MoleculeOrigin::position() const
{
    return m_position;
}

void MoleculeOrigin::setPosition(const Eigen::Vector3d newPosition)
{
    m_position = newPosition;
}

}


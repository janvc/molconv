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


#include "moleculeoriginglobal.h"

namespace molconv {

MoleculeOriginGlobal::MoleculeOriginGlobal()
{
}

MoleculeOriginGlobal::MoleculeOriginGlobal(moleculePtr molecule, std::vector<bool> originList)
    : MoleculeOrigin(molecule)
{
    m_originList = originList;
}

std::vector<bool> MoleculeOriginGlobal::originList() const
{
    return m_originList;
}

std::array<int,2> MoleculeOriginGlobal::atoms() const
{
    return std::array<int,2>();
}

double MoleculeOriginGlobal::factor() const
{
    return 0.0;
}

}

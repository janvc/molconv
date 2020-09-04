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


#include "moleculeoriginonatom.h"

namespace molconv {

MoleculeOriginOnAtom::MoleculeOriginOnAtom(moleculePtr molecule, const int atom1)
    : MoleculeOrigin(molecule)
{
    m_atom1 = atom1;

    m_position = m_molecule->atom(m_atom1)->position();
}

MoleculeOriginOnAtom::MoleculeOriginOnAtom(const MoleculeOriginOnAtom &origin)
{
    m_molecule = origin.molecule();
    m_position = origin.position();
    m_atom1 = origin.atoms()[0];
}

MoleculeOrigin *MoleculeOriginOnAtom::clone()
{
    return new MoleculeOriginOnAtom(*this);
}

std::vector<bool> MoleculeOriginOnAtom::originList() const
{
    return std::vector<bool>();
}

std::array<int,2> MoleculeOriginOnAtom::atoms() const
{
    std::array<int,2> array;
    array[0] = m_atom1;
    array[1] = -1;

    return array;
}

double MoleculeOriginOnAtom::factor() const
{
    return 0.0;
}

}

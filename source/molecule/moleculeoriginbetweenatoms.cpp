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


#include "moleculeoriginbetweenatoms.h"

namespace molconv {

MoleculeOriginBetweenAtoms::MoleculeOriginBetweenAtoms(moleculePtr molecule, const int atom1, const int atom2, const double factor)
    : MoleculeOriginOnAtom (molecule, atom1)
{
    m_atom2 = atom2;
    m_factor = factor;

    m_position = m_factor * m_molecule->atom(m_atom1)->position()
               + (1.0 - m_factor) * m_molecule->atom(m_atom2)->position();
}

}

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


#ifndef MOLECULEORIGINBETWEENATOMS_H
#define MOLECULEORIGINBETWEENATOMS_H

#include "moleculeoriginonatom.h"

namespace molconv {

class MoleculeOriginBetweenAtoms : public MoleculeOriginOnAtom
{
public:
    MoleculeOriginBetweenAtoms(moleculePtr molecule, const int atom1, const int atom2, const double factor);
    MoleculeOriginBetweenAtoms(const MoleculeOriginBetweenAtoms &origin);
    ~MoleculeOriginBetweenAtoms() {}
    MoleculeOrigin *clone();

    std::array<int,2> atoms() const;
    double factor() const;
    OriginCode code() const;

private:
    int m_atom2;
    double m_factor;
};

}

#endif // MOLECULEORIGINBETWEENATOMS_H

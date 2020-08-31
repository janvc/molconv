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


#ifndef MOLECULEBASISONATOMS_H
#define MOLECULEBASISONATOMS_H

#include "moleculebasis.h"

namespace molconv {

class MoleculeBasisOnAtoms : public MoleculeBasis
{
public:
    MoleculeBasisOnAtoms(moleculePtr molecule, const int atom1, const int atom2, const int atom3);

    std::array<int,3> atoms() const;

private:
    int m_atom1;
    int m_atom2;
    int m_atom3;
};

}

#endif // MOLECULEBASISONATOMS_H

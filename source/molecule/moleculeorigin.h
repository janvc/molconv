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


#ifndef MOLECULEORIGIN_H
#define MOLECULEORIGIN_H

#include <Eigen/Core>
#include "types.h"
#include "molecule.h"

namespace molconv {

class MoleculeOrigin
{
public:
    MoleculeOrigin(moleculePtr molecule);

    moleculePtr molecule();
    Eigen::Vector3d position() const;
    void setPosition(const Eigen::Vector3d newPosition);

    virtual std::vector<bool> originList() = 0;
    virtual std::array<int,2> atoms() = 0;
    virtual double factor() = 0;

protected:
    moleculePtr m_molecule;
    Eigen::Vector3d m_position;
};

}


#endif // MOLECULEORIGIN_H


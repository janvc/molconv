/*
 * Copyright 2014 Jan von Cosel & Sebastian Lenz
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


#ifndef MOLECULE_H_
#define MOLECULE_H_

#ifndef Q_MOC_RUN
    #include<chemkit/molecule.h>
    #include<chemkit/atom.h>
#endif
#include<Eigen/Core>
#include "types.h"

namespace molconv
{
    class MoleculePrivate;

    class Molecule : public chemkit::Molecule
    {
    public:
        // constructors and destructor(s):
        Molecule();
        Molecule(const chemkit::Molecule &BaseMolecule);
        Molecule(const Molecule &originalMolecule);

        // molecular properties:
        Eigen::Vector3d internalOrigin() const;
        Eigen::Matrix3d internalBasis() const;

        Eigen::Matrix3d inertiaTensor() const;
        Eigen::Matrix3d covarianceMatrix() const;

        // moving the molecule:
        void translate(const Eigen::Vector3d &shiftVector);
        void rotate(const Eigen::Matrix3d &rotationMatrix);
        void rotate(const Eigen::Vector3d &axis, const double &angle);

    private:
        MoleculePrivate *d;
    };


} // namespace molconv

#endif /* MOLECULE_H_ */

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


#ifndef ABSTRACTMOLECULEGROUP_H
#define ABSTRACTMOLECULEGROUP_H

#include<boost/shared_ptr.hpp>
#include"molecule.h"

namespace molconv
{
    class abstractMoleculeGroupPrivate;

    class abstractMoleculeGroup
    {
    public:
        abstractMoleculeGroup();

        size_t size() const;

        double EulerTheta(const size_t index) const;
        double EulerPhi(const size_t index) const;
        double EulerPsi(const size_t index) const;

        double Distance(const size_t firstMolecule, const size_t secondMolecule) const;
        Eigen::Vector3d DistanceVector(const size_t firstMolecule, const size_t secondMolecule) const;

        void checkIndex(const size_t index) const;

        boost::shared_ptr<Molecule> getMolecule(const size_t index) const;

        void addMolecule(const boost::shared_ptr<Molecule> newMolecule);
        void removeMolecule(const size_t index);

    private:
        abstractMoleculeGroupPrivate *d;
    };

} // namespace molconv

#endif // ABSTRACTMOLECULEGROUP_H

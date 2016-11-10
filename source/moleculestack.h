/*
 * Copyright 2014 - 2016 Jan von Cosel & Sebastian Lenz
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


#ifndef MOLECULESTACK_H
#define MOLECULESTACK_H

#include<boost/scoped_ptr.hpp>
#include"moleculegroup.h"
#include"types.h"

namespace molconv
{
    class MoleculeStackPrivate;

    class MoleculeStack : public MoleculeGroup
    {
    public:
        MoleculeStack();
        MoleculeStack(const std::string &name);
        ~MoleculeStack();

        void addMolecule(const moleculePtr &newMolecule,
                         const parallelVector newVector = zVec,
                         const size_t atom1 = 0,
                         const size_t atom2 = 0);

        size_t RefMol() const;
        void setReferenceMolecule(const size_t newRef);

        parallelVector getParallelVector(const size_t index) const;
        Eigen::Vector3d getParallelVectorDirection(const size_t index) const;
        void setParallelVector(const size_t index, const parallelVector newVector,
                               const size_t atom1 = 0, const size_t atom2 = 0);

        double PlaneDistance(const size_t index) const;
        double LineDistance(const size_t index) const;
        double RotationAngle(const size_t index) const;
        double LateralX(const size_t index) const;
        double LateralY(const size_t index) const;

        void setPlaneDistance(const size_t index, const double newDistance);
        void setRotationAngle(const size_t index, const double newAngle);
        void setLateralX(const size_t index, const double newX);
        void setLateralY(const size_t index, const double newY);

    private:
        boost::scoped_ptr<MoleculeStackPrivate> d;
    };

} // namespace molconv

#endif // MOLECULESTACK_H

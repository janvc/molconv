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


#ifndef MOLECULESTACK_H
#define MOLECULESTACK_H

#include"abstractmoleculegroup.h"
#include"types.h"

namespace molconv
{
    class MoleculeStackPrivate;

    class MoleculeStack : public abstractMoleculeGroup
    {
    public:
        MoleculeStack();

        void addMolecule(const boost::shared_ptr<Molecule> newMolecule, const parallelVector newVector);

        size_t ReferenceMolecule() const;
        void setReferenceMolecule(const size_t newRef);

        parallelVector getParallelVector(const size_t index) const;
        void setParallelVector(const size_t index, const parallelVector newVector);

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
        MoleculeStackPrivate *d;
    };

} // namespace molconv

#endif // MOLECULESTACK_H

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


#include<stdexcept>
#include<cmath>
#include "moleculestack.h"


namespace molconv
{
    class MoleculeStackPrivate
    {
    public:
        MoleculeStackPrivate()
            : referenceMolecule(0)
        {
        }

        size_t referenceMolecule;
        std::vector<parallelVector> vectors;
    };

    ///
    /// \brief MoleculeStack::MoleculeStack
    ///
    /// the constructor of MoleculeStack
    ///
    MoleculeStack::MoleculeStack()
        : abstractMoleculeGroup()
        , d(new MoleculeStackPrivate)
    {
    }

    ///
    /// \brief MoleculeStack::~MoleculeStack
    ///
    /// the default destructor of MoleculeStack
    ///
    MoleculeStack::~MoleculeStack() {}

    ///
    /// \brief MoleculeStack::ReferenceMolecule
    /// \return
    ///
    /// return the index of the reference molecule of the stack
    ///
    size_t MoleculeStack::ReferenceMolecule() const
    {
        return d->referenceMolecule;
    }

    ///
    /// \brief MoleculeStack::setReferenceMolecule
    /// \param newRef
    ///
    /// set the reference molecule to molecule  \p index
    ///
    void MoleculeStack::setReferenceMolecule(const size_t newRef)
    {
        checkIndex(newRef);

        d->referenceMolecule = newRef;
    }

    ///
    /// \brief MoleculeStack::PlaneDistance
    /// \return
    ///
    /// return the distance between the least-squares-plane of the molecule
    /// \p index and that of the reference molecule
    ///
    double MoleculeStack::PlaneDistance(const size_t index) const
    {
        checkIndex(index);

        Eigen::Vector3d distance = DistanceVector(ReferenceMolecule(), index);

        Eigen::Vector3d zBasis = getMolecule(ReferenceMolecule())->internalBasisVectors().col(2);

        return distance.dot(zBasis);
    }

    ///
    /// \brief MoleculeStack::RotationAngle
    /// \return
    ///
    /// return the rotation angle between the x-axis of the molecule
    /// \p index and that of the reference molecule
    ///
    double MoleculeStack::RotationAngle(const size_t index) const
    {
        Eigen::Vector3d xBasis = getMolecule(ReferenceMolecule())->internalBasisVectors().col(0);
        Eigen::Vector3d indexVector = getMolecule(index)->internalBasisVectors().col(0);

        xBasis.normalize();
        indexVector.normalize();

        double scalarProduct = xBasis.dot(indexVector);

        return acos(scalarProduct);
    }

    ///
    /// \brief MoleculeStack::LateralX
    /// \param index
    /// \return
    ///
    /// return the lateral displacement of the molecule \p index with respect to the
    /// reference molecule in X direction (of the reference molecule)
    ///
    double MoleculeStack::LateralX(const size_t index) const
    {
        Eigen::Vector3d distance = DistanceVector(ReferenceMolecule(), index);

        Eigen::Vector3d xBasis = getMolecule(ReferenceMolecule())->internalBasisVectors().col(0);

        return distance.dot(xBasis);
    }

    ///
    /// \brief MoleculeStack::LateralY
    /// \param index
    /// \return
    ///
    /// return the lateral displacement of the molecule \p index with respect to the
    /// reference molecule in Y direction (of the reference molecule)
    ///
    double MoleculeStack::LateralY(const size_t index) const
    {
        Eigen::Vector3d distance = DistanceVector(ReferenceMolecule(), index);

        Eigen::Vector3d yBasis = getMolecule(ReferenceMolecule())->internalBasisVectors().col(1);

        return distance.dot(yBasis);
    }

    ///
    /// \brief MoleculeStack::setPlaneDistance
    /// \param index
    /// \param newDistance
    ///
    /// set the distance between the molecule \p index and the reference molecule
    /// to \p newDistance
    ///
    void MoleculeStack::setPlaneDistance(const size_t index, const double newDistance)
    {
        double currentDistance = PlaneDistance(index);

        Eigen::Vector3d shiftVector = getMolecule(ReferenceMolecule())->internalBasisVectors().col(2)
                                    * (newDistance - currentDistance);

        getMolecule(index)->translate(shiftVector);
    }

    ///
    /// \brief MoleculeStack::setRotationAngle
    /// \param index
    /// \param newAngle
    ///
    /// set the rotation angle of the molecule \p index with respect to the reference
    /// molecule to \p newAngle
    ///
    void MoleculeStack::setRotationAngle(const size_t index, const double newAngle)
    {
        double shiftAngle = newAngle - RotationAngle(index);

        Eigen::Vector3d rotationAxis = getMolecule(index)->internalBasisVectors().col(2);

        getMolecule(index)->rotate(rotationAxis, shiftAngle);
    }

    ///
    /// \brief MoleculeStack::setLateralX
    /// \param index
    /// \param newX
    ///
    /// set the X component of the lateral displacement of molecule \p index with
    /// respect to the reference molecule to \p newX
    ///
    void MoleculeStack::setLateralX(const size_t index, const double newX)
    {
        double shiftX = newX - LateralX(index);

        Eigen::Vector3d shiftVector = getMolecule(ReferenceMolecule())->internalBasisVectors().col(0) * shiftX;

        getMolecule(index)->translate(shiftVector);
    }

    ///
    /// \brief MoleculeStack::setLateralY
    /// \param index
    /// \param newY
    ///
    /// set the Y component of the lateral displacement of molecule \p index with
    /// respect to the reference molecule to \p newY
    ///
    void MoleculeStack::setLateralY(const size_t index, const double newY)
    {
        double shiftY = newY - LateralY(index);

        Eigen::Vector3d shiftVector = getMolecule(ReferenceMolecule())->internalBasisVectors().col(1) * shiftY;

        getMolecule(index)->translate(shiftVector);
    }


} // namespace molconv

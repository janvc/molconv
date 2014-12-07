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
#include<array>
#include<QDebug>
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
        std::vector<std::array<int,2> > parallelAtoms;
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
        qDebug("this is the constructor of MoleculeStack");
    }

    ///
    /// \brief MoleculeStack::~MoleculeStack
    ///
    /// the default destructor of MoleculeStack
    ///
    MoleculeStack::~MoleculeStack() {}

    ///
    /// \brief MoleculeStack::addMolecule
    /// \param newMolecule
    /// \param newVector
    /// \param atom1
    /// \param atom2
    ///
    /// add a new molecule to the stack
    ///
    void MoleculeStack::addMolecule(const boost::shared_ptr<Molecule> newMolecule, const parallelVector newVector, const size_t atom1, const size_t atom2)
    {
        qDebug("entering MoleculeStack::addMolecule()");
        abstractMoleculeGroup::addMolecule(newMolecule);

        std::array<int,2> theAtoms;
        theAtoms[0] = atom1;
        theAtoms[1] = atom2;

        d->vectors.push_back(newVector);
        d->parallelAtoms.push_back(theAtoms);

        // if this is NOT the first molecule, align the new molecule to the reference molecule:
        if (size() != 1)
        {
            double distance = Distance(ReferenceMolecule(), size() - 1);
            Eigen::Vector3d newPosition = getMolecule(ReferenceMolecule())->internalOriginPosition() + getParallelVectorDirection(ReferenceMolecule()) * distance;
            Eigen::Vector3d translationVector = newPosition - getMolecule(size() - 1)->internalOriginPosition();

            getMolecule(size() - 1)->translate(translationVector);

            Eigen::Vector3d rotationAxis = getParallelVectorDirection(ReferenceMolecule()).cross(getParallelVectorDirection(size() - 1));
            double rotAngle = acos(getParallelVectorDirection(ReferenceMolecule()).dot(getParallelVectorDirection(size() - 1)));

            getMolecule(size() - 1)->rotate(rotationAxis, rotAngle);
        }
    }

    ///
    /// \brief MoleculeStack::ReferenceMolecule
    /// \return
    ///
    /// return the index of the reference molecule of the stack
    ///
    size_t MoleculeStack::ReferenceMolecule() const
    {
        qDebug("entering MoleculeStack::ReferenceMolecule()");
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
        qDebug("entering MoleculeStack::setReferenceMolecule()");
        checkIndex(newRef);

        d->referenceMolecule = newRef;
    }

    ///
    /// \brief MoleculeStack::getParallelVector
    /// \param index
    /// \return
    ///
    /// return the type of parallel vector of molecule \p index
    ///
    parallelVector MoleculeStack::getParallelVector(const size_t index) const
    {
        qDebug("entering MoleculeStack::getParallelVector()");
        checkIndex(index);

        return d->vectors.at(index);
    }

    ///
    /// \brief MoleculeStack::getParallelVectorDirection
    /// \param index
    /// \return
    ///
    /// return the actual parallel vector of molecule \p index in terms of its components
    ///
    Eigen::Vector3d MoleculeStack::getParallelVectorDirection(const size_t index) const
    {
        qDebug("entering MoleculeStack::getParallelVectorDirection");
        Eigen::Vector3d vector;

        switch (getParallelVector(index))
        {
        case kInertVSmall:
            vector = getMolecule(index)->inertiaEigenvectors().col(0);
            break;
        case kInertVMedium:
            vector = getMolecule(index)->inertiaEigenvectors().col(1);
            break;
        case kInertVLarge:
            vector = getMolecule(index)->inertiaEigenvectors().col(2);
            break;
        case kCovarVSmall:
            vector = getMolecule(index)->covarianceEigenvectors().col(0);
            break;
        case kCovarVMedium:
            vector = getMolecule(index)->covarianceEigenvectors().col(1);
            break;
        case kCovarVLarge:
            vector = getMolecule(index)->covarianceEigenvectors().col(2);
            break;
        case kAtoms:
            Eigen::Vector3d firstAtom = getMolecule(index)->atom(d->parallelAtoms[index][0])->position();
            Eigen::Vector3d secondAtom = getMolecule(index)->atom(d->parallelAtoms[index][1])->position();
            vector = secondAtom - firstAtom;
            vector.normalize();
            break;
        }

        return vector;
    }

    ///
    /// \brief MoleculeStack::setParallelVector
    /// \param index
    /// \param newVector
    /// \param atom1
    /// \param atom2
    ///
    /// set the parallel vector of an already existing molecule
    ///
    void MoleculeStack::setParallelVector(const size_t index, const parallelVector newVector, const size_t atom1, const size_t atom2)
    {
        qDebug("entering MoleculeStack::setParallelVector()");
        checkIndex(index);
        checkIndex(atom1);
        checkIndex(atom2);

        std::array<int,2> theAtoms;
        theAtoms[0] = atom1;
        theAtoms[1] = atom2;

        d->vectors.at(index) = newVector;
        d->parallelAtoms.at(index) = theAtoms;
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
        qDebug("entering MoleculeStack::PlaneDistance()");
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
        qDebug("entering MoleculeStack::RotationAngle()");
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
        qDebug("entering MoleculeStack::LateralX()");
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
        qDebug("entering MoleculeStack::LateralY()");
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
        qDebug("entering MoleculeStack::setPlaneDistance()");
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
        qDebug("entering MoleculeStack::setRotationAngle()");
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
        qDebug("entering MoleculeStack::setLateralX()");
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
        qDebug("entering MoleculeStack::setLateralY()");
        double shiftY = newY - LateralY(index);

        Eigen::Vector3d shiftVector = getMolecule(ReferenceMolecule())->internalBasisVectors().col(1) * shiftY;

        getMolecule(index)->translate(shiftVector);
    }


} // namespace molconv

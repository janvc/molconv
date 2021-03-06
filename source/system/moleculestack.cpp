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


#include<stdexcept>
#include<cmath>
#include<array>
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
    /// the default constructor of MoleculeStack
    ///
    MoleculeStack::MoleculeStack()
        : MoleculeGroup()
        , d(new MoleculeStackPrivate)
    {
    }

    ///
    /// \brief MoleculeStack::MoleculeStack
    /// \param name
    ///
    /// this constructor takes the name of the stack as an argument
    ///
    MoleculeStack::MoleculeStack(const std::string &name)
        : MoleculeGroup(name)
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
    /// \brief MoleculeStack::addMolecule
    /// \param newMolecule
    /// \param newVector
    /// \param atom1
    /// \param atom2
    ///
    /// add a new molecule to the stack
    ///
    void MoleculeStack::addMolecule(const moleculePtr &newMolecule, const parallelVector newVector,
                                    const size_t atom1, const size_t atom2)
    {
        MoleculeGroup::addMolecule(newMolecule);

        std::array<int,2> theAtoms;
        theAtoms[0] = atom1;
        theAtoms[1] = atom2;

        d->vectors.push_back(newVector);
        d->parallelAtoms.push_back(theAtoms);

        // if this is NOT the first molecule, align the new molecule to the reference molecule:
        if (nMolecules() > 1)
        {
            double distance = Distance(RefMol(), nMolecules() - 1);
            Eigen::Vector3d newPos = getMol(RefMol())->originPosition()
                                   + getParallelVectorDirection(RefMol()) * distance;
            getMol(nMolecules() - 1)->moveFromParas(newPos(0), newPos(1), newPos(2),
                                                    getMol(RefMol())->phi(), getMol(RefMol())->theta(), getMol(RefMol())->psi());
        }
    }

    ///
    /// \brief MoleculeStack::ReferenceMolecule
    /// \return
    ///
    /// return the index of the reference molecule of the stack
    ///
    size_t MoleculeStack::RefMol() const
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
    /// \brief MoleculeStack::getParallelVector
    /// \param index
    /// \return
    ///
    /// return the type of parallel vector of molecule \p index
    ///
    parallelVector MoleculeStack::getParallelVector(const size_t index) const
    {
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
        Eigen::Vector3d vector;

        switch (getParallelVector(index))
        {
        case xVec:
            vector = getMol(index)->basisVectors().col(0);
            break;
        case yVec:
            vector = getMol(index)->basisVectors().col(1);
            break;
        case zVec:
            vector = getMol(index)->basisVectors().col(2);
            break;
        case Atoms:
            Eigen::Vector3d firstAtom = getMol(index)->atom(d->parallelAtoms[index][0])->position();
            Eigen::Vector3d secondAtom = getMol(index)->atom(d->parallelAtoms[index][1])->position();
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
        checkIndex(index);

        Eigen::Vector3d distance = DistanceVector(RefMol(), index);

        Eigen::Vector3d zBasis = getMol(RefMol())->basisVectors().col(2);

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
        Eigen::Vector3d xBasis = getMol(RefMol())->basisVectors().col(0);
        Eigen::Vector3d indexVector = getMol(index)->basisVectors().col(0);

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
        Eigen::Vector3d distance = DistanceVector(RefMol(), index);

        Eigen::Vector3d xBasis = getMol(RefMol())->basisVectors().col(0);

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
        Eigen::Vector3d distance = DistanceVector(RefMol(), index);

        Eigen::Vector3d yBasis = getMol(RefMol())->basisVectors().col(1);

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

        Eigen::Vector3d shiftVector = getMol(RefMol())->basisVectors().col(2)
                                    * (newDistance - currentDistance);

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

        Eigen::Vector3d rotationAxis = getMol(index)->basisVectors().col(2);

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

        Eigen::Vector3d shiftVector = getMol(RefMol())->basisVectors().col(0) * shiftX;

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

        Eigen::Vector3d shiftVector = getMol(RefMol())->basisVectors().col(1) * shiftY;

    }


} // namespace molconv

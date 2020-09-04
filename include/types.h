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
 *
 * This file is containing some types used for molconv.
 *
 */

#ifndef TYPES_H
#define TYPES_H

#include<boost/shared_ptr.hpp>

namespace molconv {
    /*
     * Center of a molecule
     */
    enum OriginCode {
        kCenterOfMass = 0,
        kCenterOfGeometry = 1,
        kCenterOfCharge = 2,
        kCenterOnAtom = 3,
        kCenterBetweenAtoms = 4
    };

    /*
     * Which vectors to use for new internal basis
     */
    enum BasisCode {
        kCovarianceVectors = 0,
        kInertiaVectors = 1,
        kChargeVectors = 2,
        kStandardOrientation = 3,
        kVectorsFromAtoms = 4
    };

    /*
     * Which molecule-specific vector is parallel in a stack
     */
    enum parallelVector {
        xVec = 0,
        yVec = 1,
        zVec = 2,
        Atoms = 3
    };

    // pointer to a molecule
    class Molecule;
    class MoleculeGroup;
    class System;
    typedef boost::shared_ptr<Molecule> moleculePtr;
    typedef boost::shared_ptr<MoleculeGroup> groupPtr;
//    typedef boost::shared_ptr<System> sysPtr;
} // namespace molconv

#endif // TYPES_H

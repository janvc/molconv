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
    enum origin {
        kCenterOnZero,
        kCenterOfMass,
        kCenterOfGeometry,
        kCenterOnAtom,
        kCenterBetweenAtoms
    };

    /*
     * Which vectors to use for new internal basis
     */
    enum basis {
        kIdentityVectors,
        kCovarianceVectors,
        kInertiaVectors,
        kVectorsFromAtoms
    };

    /*
     * Which molecule-specific vector is parallel in a stack
     */
    enum parallelVector {
        kInertVSmall,
        kInertVMedium,
        kInertVLarge,
        kCovarVSmall,
        kCovarVMedium,
        kCovarVLarge,
        kAtoms
    };

    // pointer to a molecule
    class Molecule;
    class abstractMoleculeGroup;
    class System;
    typedef boost::shared_ptr<Molecule> moleculePtr;
    typedef boost::shared_ptr<abstractMoleculeGroup> groupPtr;
    typedef boost::shared_ptr<System> sysPtr;
} // namespace molconv

#endif // TYPES_H

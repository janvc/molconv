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


#ifndef SYSTEM_H
#define SYSTEM_H

#include<vector>
#include<boost/shared_ptr.hpp>
#include "molecule.h"

namespace molconv
{
    class SystemPrivate;

    class System
    {
    public:
        System();
        size_t size() const;
        boost::shared_ptr<Molecule> getMolecule(const size_t index) const;

        void addMolecule(const boost::shared_ptr<Molecule> newMolecule);
        void removeMolecule(const size_t index);

    private:
        SystemPrivate *d;
    };

} // namespace molconv

#endif // SYSTEM_H

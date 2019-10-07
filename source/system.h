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


#ifndef SYSTEM_H
#define SYSTEM_H

#include<vector>
#include<QAbstractItemModel>
#include<boost/shared_ptr.hpp>
#include<boost/scoped_ptr.hpp>
#include "molecule.h"

namespace molconv
{
    class SystemPrivate;

    class System
    {
    public:
        System();
        ~System();
        size_t nMolecules() const;
        size_t nGroups() const;
        moleculePtr getMolecule(const unsigned long index) const;
        groupPtr getGroup(const size_t index) const;
        size_t MoleculeIndex(const moleculePtr theMolecule);
        size_t GroupIndex(const groupPtr &theGroup) const;
        std::vector<unsigned long> getMolIDs() const;

        void addMolecule(const moleculePtr newMolecule);
        void removeMolecule(const unsigned long key);
        void addGroup(const groupPtr &newGroup);
        void removeGroup(const size_t index);

    private:
        boost::scoped_ptr<SystemPrivate> d;
    };

} // namespace molconv

#endif // SYSTEM_H

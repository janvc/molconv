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


#ifndef MOLECULEGROUP_H
#define MOLECULEGROUP_H

#include<boost/shared_ptr.hpp>
#include<boost/scoped_ptr.hpp>
#include"molecule.h"

namespace molconv
{
    class MoleculeGroupPrivate;

    class MoleculeGroup
    {
    public:
        MoleculeGroup();
        MoleculeGroup(const std::string &name);
        ~MoleculeGroup();

        size_t nMolecules() const;
        size_t nAtoms() const;
        size_t nGroups() const;
        std::string name() const;
        double mass() const;

        double Distance(const size_t firstMolecule, const size_t secondMolecule) const;
        Eigen::Vector3d DistanceVector(const size_t firstMolecule, const size_t secondMolecule) const;

        void checkIndex(const size_t index) const;

        moleculePtr getMol(const size_t index) const;
        groupPtr getGroup(const size_t index) const;
        groupPtr parent() const;

        void setName(const std::string &newName);

        virtual void addMolecule(const moleculePtr &newMolecule);
        void removeMolecule(const size_t index);

        void addGroup(const groupPtr &newGroup);
        void removeGroup(const size_t index);
        void addToGroup(const groupPtr &newParentGroup);

    private:
        boost::scoped_ptr<MoleculeGroupPrivate> d;
    };

} // namespace molconv

#endif // MOLECULEGROUP_H

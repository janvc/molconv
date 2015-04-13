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


#ifndef ABSTRACTMOLECULEGROUP_H
#define ABSTRACTMOLECULEGROUP_H

#include<boost/shared_ptr.hpp>
#include<boost/scoped_ptr.hpp>
#include"../molconv-molecule/molecule.h"

namespace molconv
{
    class abstractMoleculeGroupPrivate;

    class abstractMoleculeGroup
    {
    public:
        abstractMoleculeGroup();
        abstractMoleculeGroup(const std::string &name);
        ~abstractMoleculeGroup();

        size_t nMolecules() const;
        size_t nGroups() const;
        std::string name() const;

        double EulerTheta(const size_t index) const;
        double EulerPhi(const size_t index) const;
        double EulerPsi(const size_t index) const;

        double Distance(const size_t firstMolecule, const size_t secondMolecule) const;
        Eigen::Vector3d DistanceVector(const size_t firstMolecule, const size_t secondMolecule) const;

        void checkIndex(const size_t index) const;

        moleculePtr getMolecule(const size_t index) const;
        groupPtr getGroup(const size_t index) const;
        groupPtr parent() const;

        void setName(const std::string &newName);

        void addMolecule(const moleculePtr newMolecule);
        void removeMolecule(const size_t index);

        void addGroup(const groupPtr &newGroup);
        void removeGroup(const size_t index);
        void addToGroup(const groupPtr &newParentGroup);

    private:
        boost::scoped_ptr<abstractMoleculeGroupPrivate> d;
    };

} // namespace molconv

#endif // ABSTRACTMOLECULEGROUP_H

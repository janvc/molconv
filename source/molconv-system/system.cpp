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
#include<algorithm>
#include<boost/make_shared.hpp>
#include<QDebug>
#include "system.h"


namespace molconv
{
    class SystemPrivate
    {
    public:
        std::vector<moleculePtr> m_molecules;
        std::vector<groupPtr> m_groups;
    };

    ///
    /// \brief System::System
    ///
    /// The default constructor of the System class
    ///
    System::System()
        : d(new SystemPrivate)
    {
        qDebug() << "this is the first constructor of molconv::System";
    }

    ///
    /// \brief System::~System
    ///
    /// The default destructor of the System class
    ///
    System::~System() {}

    ///
    /// \brief System::size
    /// \return size_t
    ///
    /// returns the number of molecules in the system
    ///
    size_t System::nMolecules() const
    {
        qDebug() << "entering System::nMolecules()";
        return d->m_molecules.size();
    }

    ///
    /// \brief System::nGroups
    /// \return
    ///
    /// returns the number of groups in the system
    ///
    size_t System::nGroups() const
    {
        qDebug("entering System::nGroups()");
        return d->m_groups.size();
    }

    ///
    /// \brief System::Molecule
    /// \param index
    /// \return moleculePtr
    ///
    /// returns a shared pointer to the molecule at \p index
    ///
    moleculePtr System::getMolecule(const size_t index) const
    {
        qDebug() << "entering System::getMolecule()";
        if (index >= nMolecules())
            throw std::invalid_argument("index out of range.\n");

        return d->m_molecules.at(index);
    }

    ///
    /// \brief System::getGroup
    /// \param index
    /// \return
    ///
    /// returns a shared pointer to the group at \p index
    ///
    groupPtr System::getGroup(const size_t index) const
    {
        qDebug("entering System::getGroup()");
        if (index >= nGroups())
            throw std::invalid_argument("index out of range.\n");

        return d->m_groups.at(index);
    }

    ///
    /// \brief System::MoleculeIndex
    /// \param theMolecule
    /// \return
    ///
    /// returns the position of the molecule in the vector
    ///
    size_t System::MoleculeIndex(const moleculePtr theMolecule)
    {
        qDebug("entering System::MoleculeIndex()");

        size_t index;

        for (size_t i = 0; i < nMolecules(); i++)
        {
            if (getMolecule(i) == theMolecule)
                index = i;
        }

        return index;
    }

    ///
    /// \brief System::GroupIndex
    /// \param theGroup
    /// \return
    ///
    /// returns the position of the group in the vector
    ///
    size_t System::GroupIndex(const groupPtr &theGroup) const
    {
        qDebug("entering System::GroupIndex()");

        size_t index = 0;

        for (size_t i = 0; i < nGroups(); i++)
        {
            if (getGroup(i) == theGroup)
                index = i;
        }

        return index;
    }

    ///
    /// \brief System::addMolecule
    /// \param newMolecule
    ///
    /// adds a new molecule \p newMolecule to the system
    ///
    void System::addMolecule(const moleculePtr newMolecule)
    {
        qDebug() << "entering System::addMolecule()";
        d->m_molecules.push_back(newMolecule);

        //QModelIndex index = createIndex(nMolecules(), 0, newMolecule.get());

        //emit dataChanged(index, index);
    }

    ///
    /// \brief System::removeMolecule
    /// \param index
    ///
    /// removes the molecule at index \p index
    ///
    void System::removeMolecule(const size_t index)
    {
        qDebug() << "entering System::removeMolecule()";
        if (index >= nMolecules())
            throw std::invalid_argument("index out of range.\n");

        d->m_molecules.erase(d->m_molecules.begin() + index);
    }

    ///
    /// \brief System::addGroup
    /// \param newGroup
    ///
    /// adds a new group to the system
    ///
    void System::addGroup(const groupPtr &newGroup)
    {
        qDebug("entering System::addGroup()");
        d->m_groups.push_back(newGroup);
    }

    ///
    /// \brief System::removeGroup
    /// \param index
    ///
    /// removes the group at index \p index
    ///
    void System::removeGroup(const size_t index)
    {
        qDebug("entering System::removeGroup()");

        if (index >= nGroups())
            throw std::invalid_argument("index out of range.\n");

        d->m_groups.erase(d->m_groups.begin() + index);
    }

    ///
    /// \brief System::data
    /// \param index
    /// \param role
    /// \return
    ///
    /// get the data from the model to the tree view
    /// 1st column: name of molecule/group
    /// 2nd column: number of atoms/molecules of molecule/group
    ///
    QVariant System::data(const QModelIndex &index, int role) const
    {
        qDebug("entering System::data()");

        if (!index.isValid())
            return QVariant();

        if (role != Qt::DisplayRole && role != Qt::EditRole)
            return QVariant();

        // find out if index points to a molecule or a group
        Molecule *testMol = getMolecule(index);
        abstractMoleculeGroup *testGroup = getGroup(index);

        if (testMol)    // it is a molecule!!
        {
            switch (index.column())
            {
            case 0:
            {
                return QVariant(QString::fromStdString(testMol->name()));
                break;
            }
            case 1:
            {
                int molSize = testMol->size();
                return QVariant(molSize);
                break;
            }
            default:
            {
                return QVariant();
                break;
            }
            }
        }
        else if (testGroup) // no it's not!!
        {
            switch (index.column())
            {
            case 0:
            {
                return QVariant(QString::fromStdString(testGroup->name()));
                break;
            }
            case 1:
            {
                int groupSize = testGroup->nMolecules();
                return QVariant(groupSize);
                break;
            }
            default:
            {
                return QVariant();
                break;
            }
            }
        }
        else    // WTF!?
            return QVariant();
    }


    QVariant System::headerData(int section, Qt::Orientation orientation, int role) const
    {
        qDebug("entering System::headerData()");

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            switch (section)
            {
            case 0:
                return QVariant("Name");
                break;
            case 1:
                return QVariant("Number of Atoms/Molecules");
                break;
            }
        }

        return QVariant();
    }


    QModelIndex System::index(int row, int column, const QModelIndex &parent) const
    {
        qDebug("entering System::index()");

        if (parent.isValid() && parent.column() != 0)
            return QModelIndex();

        abstractMoleculeGroup *parentGroup = getGroup(parent);
        int groupsInParent = parentGroup->nGroups();

        if (row >= groupsInParent)
        {
            // it must be a molecule, because the groups always come first
            Molecule *childMol = parentGroup->getMolecule(row - groupsInParent).get();
            if (childMol)
                return createIndex(row, column, childMol);
            else
                return QModelIndex();
        }
        else
        {
            abstractMoleculeGroup *childGroup = parentGroup->getGroup(row).get();
            if (childGroup)
                return createIndex(row, column, childGroup);
            else
                return QModelIndex();
        }
    }


    QModelIndex System::parent(const QModelIndex &child) const
    {
        qDebug("entering System::parent()");

        if (!child.isValid())
            return QModelIndex();

        Molecule *testMol = getMolecule(child);
        if (testMol)    // "this" is a molecule
        {
            abstractMoleculeGroup *parentGroup = testMol->group().get();
            if (parentGroup == 0)
                return QModelIndex();

            int index = 0;
            while (parentGroup->getMolecule(index).get() != child.internalPointer())
                index++;

            return createIndex(index, 0, parentGroup);
        }
        else    // it is a group
        {
            abstractMoleculeGroup *testGroup = getGroup(child);
            abstractMoleculeGroup *parentGroup = testGroup->parent().get();
            if (parentGroup == 0)
                return QModelIndex();

            int index = 0;
            while (parentGroup->getGroup(index).get() != child.internalPointer())
                index++;

            return createIndex(index, 0, parentGroup);
        }
    }


    bool System::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        qDebug("entering System::setData()");

        if (role != Qt::EditRole)
            return false;

        switch (index.column())
        {
            case 0:
        {
                Molecule *testMol = getMolecule(index);
                if (testMol)
                    testMol->setName(value.toString().toStdString());
                else
                {
                    abstractMoleculeGroup *testGroup = getGroup(index);
                    testGroup->setName(value.toString().toStdString());
                }
                return true;
                break;
        }
        default:
            return false;
            break;
        }
    }


    bool System::insertRows(int row, int count, const QModelIndex &parent)
    {
        qDebug("entering System::insertRows()");
        //return false;
        beginInsertRows(parent, row, row + count -1);
        endInsertRows();
        return true;
    }


//    bool System::removeRows(int row, int count, const QModelIndex &parent)
//    {
//        qDebug("entering System::removeRows()");
//        return false;
//    }


    int System::rowCount(const QModelIndex &parent) const
    {
        qDebug("entering System::rowCount()");

        abstractMoleculeGroup *testGroup = getGroup(parent);
        if (testGroup)
            return testGroup->nGroups() + testGroup->nMolecules();
        else
            return 0;
    }


    int System::columnCount(const QModelIndex & /* parent */) const
    {
        qDebug("entering System::columnCount()");
        return 2;
    }


    Molecule *System::getMolecule(const QModelIndex &index) const
    {
        qDebug("entering System::getMolecule()");

        if (index.isValid())
        {
            Molecule *theMol = static_cast<Molecule*>(index.internalPointer());
            if (theMol)
                return theMol;
        }
        return 0;
    }


    abstractMoleculeGroup *System::getGroup(const QModelIndex &index) const
    {
        qDebug("entering System::getGroup()");

        if (index.isValid())
        {
            abstractMoleculeGroup *theGroup = static_cast<abstractMoleculeGroup*>(index.internalPointer());
            if (theGroup)
                return theGroup;
        }
        return 0;
    }

} // namespace molconv

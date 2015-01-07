#include<QDebug>
#include "moleculegroup.h"

namespace molconv
{
    class MoleculeGroupPrivate
    {
    public:
        MoleculeGroupPrivate()
        {
        }
    };

    ///
    /// \brief MoleculeGroup::MoleculeGroup
    ///
    /// this is the default constructor
    ///
    MoleculeGroup::MoleculeGroup()
        : abstractMoleculeGroup()
        , d(new MoleculeGroupPrivate)
    {
        qDebug("this is the default constructor of MoleculeGroup");
    }

    ///
    /// \brief MoleculeGroup::MoleculeGroup
    /// \param name
    ///
    /// this constructor takes the group name as an argument
    ///
    MoleculeGroup::MoleculeGroup(const std::string &name)
        : abstractMoleculeGroup(name)
        , d(new MoleculeGroupPrivate)
    {
        qDebug("this is the second constructor of MoleculeGroup");
    }

    ///
    /// \brief MoleculeGroup::~MoleculeGroup
    ///
    /// this is the default destructor
    ///
    MoleculeGroup::~MoleculeGroup() {}



} // namespace molconv

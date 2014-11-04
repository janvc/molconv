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
    }

    ///
    /// \brief MoleculeGroup::~MoleculeGroup
    ///
    /// this is the default destructor
    ///
    MoleculeGroup::~MoleculeGroup() {}



} // namespace molconv

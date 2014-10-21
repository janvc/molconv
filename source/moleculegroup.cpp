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

    MoleculeGroup::MoleculeGroup()
        : abstractMoleculeGroup()
        , d(new MoleculeGroupPrivate)
    {
    }

} // namespace molconv

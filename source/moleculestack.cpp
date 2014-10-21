#include "moleculestack.h"

namespace molconv
{
    class MoleculeStackPrivate
    {
    public:
        MoleculeStackPrivate()
        {
        }
    };

    MoleculeStack::MoleculeStack()
        : abstractMoleculeGroup()
        , d(new MoleculeStackPrivate)
    {
    }

} // namespace molconv

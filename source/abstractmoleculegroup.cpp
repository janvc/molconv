#include "abstractmoleculegroup.h"

namespace molconv
{
    class abstractMoleculeGroupPrivate
    {
    public:
        std::vector<boost::shared_ptr<Molecule> > m_molecules;
    };

    abstractMoleculeGroup::abstractMoleculeGroup()
    {
    }

} // namespace molconv

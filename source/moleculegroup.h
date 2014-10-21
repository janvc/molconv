#ifndef MOLECULEGROUP_H
#define MOLECULEGROUP_H

#include"abstractmoleculegroup.h"

namespace molconv
{
    class MoleculeGroupPrivate;

    class MoleculeGroup : public abstractMoleculeGroup
    {
    public:
        MoleculeGroup();

    private:
        MoleculeGroupPrivate *d;
    };

} // namespace molconv

#endif // MOLECULEGROUP_H

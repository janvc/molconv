#ifndef ABSTRACTMOLECULEGROUP_H
#define ABSTRACTMOLECULEGROUP_H

#include"molecule.h"

namespace molconv
{
    class abstractMoleculeGroupPrivate;

    class abstractMoleculeGroup
    {
    public:
        abstractMoleculeGroup();


    private:
        abstractMoleculeGroupPrivate *d;
    };

} // namespace molconv

#endif // ABSTRACTMOLECULEGROUP_H

#ifndef MOLECULESTACK_H
#define MOLECULESTACK_H

#include"abstractmoleculegroup.h"

namespace molconv
{
    class MoleculeStackPrivate;

    class MoleculeStack : public abstractMoleculeGroup
    {
    public:
        MoleculeStack();

    private:
        MoleculeStackPrivate *d;
    };

} // namespace molconv

#endif // MOLECULESTACK_H

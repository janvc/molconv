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

        double Distance(const size_t index) const;
        double RotationAngle(const size_t index) const;
        double LateralX(const size_t index) const;
        double LateralY(const size_t index) const;

        void setDistance(const size_t index, const double newDistance);
        void setRotationAngle(const size_t index, const double newAngle);
        void setLateralX(const size_t index, const double newX);
        void setLateralY(const size_t index, const double newY);

    private:
        MoleculeStackPrivate *d;
    };

} // namespace molconv

#endif // MOLECULESTACK_H

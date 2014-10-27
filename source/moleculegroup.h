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

        void setPosition(const size_t index, const Eigen::Vector3d &newPosition);
        void setEulerTheta(const size_t index, const double newTheta);
        void setEulerPhi(const size_t index, const double newPhi);
        void setEulerPsi(const size_t index, const double newPsi);

    private:
        MoleculeGroupPrivate *d;
    };

} // namespace molconv

#endif // MOLECULEGROUP_H

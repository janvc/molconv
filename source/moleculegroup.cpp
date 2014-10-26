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
    /// \brief MoleculeGroup::Position
    /// \param index
    /// \return
    ///
    /// return the position of the molecule's internal origin in the global
    /// coordinate system.
    ///
    Eigen::Vector3d MoleculeGroup::Position(const size_t index) const
    {
    }

    ///
    /// \brief MoleculeGroup::EulerTheta
    /// \param index
    /// \return
    ///
    /// return the Theta Euler angle of the molecule \p index with respect to
    /// the global coordinate system
    ///
    double MoleculeGroup::EulerTheta(const size_t index) const
    {
    }

    ///
    /// \brief MoleculeGroup::EulerPhi
    /// \param index
    /// \return
    ///
    /// return the Phi Euler angle of the molecule \p index with respect to
    /// the global coordinate system
    ///
    double MoleculeGroup::EulerPhi(const size_t index) const
    {
    }

    ///
    /// \brief MoleculeGroup::EulerPsi
    /// \param index
    /// \return
    ///
    /// return the
    double MoleculeGroup::EulerPsi(const size_t index) const
    {
    }

} // namespace molconv

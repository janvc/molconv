#include "moleculestack.h"

namespace molconv
{
    class MoleculeStackPrivate
    {
    public:
        MoleculeStackPrivate()
            : referenceMolecule(0)
        {
        }

        size_t referenceMolecule;
    };

    ///
    /// \brief MoleculeStack::MoleculeStack
    ///
    /// the constructor of MoleculeStack
    ///
    MoleculeStack::MoleculeStack()
        : abstractMoleculeGroup()
        , d(new MoleculeStackPrivate)
    {
    }

    ///
    /// \brief MoleculeStack::Distance
    /// \return
    ///
    /// return the distance between the least-squares-plane of the molecule
    /// \p index and that of the reference molecule
    ///
    double MoleculeStack::Distance(const size_t index) const
    {
    }

    ///
    /// \brief MoleculeStack::RotationAngle
    /// \return
    ///
    /// return the rotation angle between the x-axis of the molecule
    /// \p index and that of the reference molecule
    ///
    double MoleculeStack::RotationAngle(const size_t index) const
    {
    }

    ///
    /// \brief MoleculeStack::LateralX
    /// \param index
    /// \return
    ///
    /// return the lateral displacement of the molecule \p index with respect to the
    /// reference molecule in X direction (of the reference molecule)
    ///
    double MoleculeStack::LateralX(const size_t index) const
    {
    }

    ///
    /// \brief MoleculeStack::LateralY
    /// \param index
    /// \return
    ///
    /// return the lateral displacement of the molecule \p index with respect to the
    /// reference molecule in Y direction (of the reference molecule)
    ///
    double MoleculeStack::LateralY(const size_t index) const
    {
    }

    ///
    /// \brief MoleculeStack::setDistance
    /// \param index
    /// \param newDistance
    ///
    /// set the distance between the molecule \p index and the reference molecule
    /// to \p newDistance
    ///
    void MoleculeStack::setDistance(const size_t index, const double newDistance)
    {
    }

    ///
    /// \brief MoleculeStack::setRotationAngle
    /// \param index
    /// \param newAngle
    ///
    /// set the rotation angle of the molecule \p index with respect to the reference
    /// molecule to \p newAngle
    ///
    void MoleculeStack::setRotationAngle(const size_t index, const double newAngle)
    {
    }

    ///
    /// \brief MoleculeStack::setLateralX
    /// \param index
    /// \param newX
    ///
    /// set the X component of the lateral displacement of molecule \p index with
    /// respect to the reference molecule to \p newX
    ///
    void MoleculeStack::setLateralX(const size_t index, const double newX)
    {
    }

    ///
    /// \brief MoleculeStack::setLateralY
    /// \param index
    /// \param newY
    ///
    /// set the Y component of the lateral displacement of molecule \p index with
    /// respect to the reference molecule to \p newY
    ///
    void MoleculeStack::setLateralY(const size_t index, const double newY)
    {
    }




} // namespace molconv

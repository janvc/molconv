/*
 * Copyright 2014 Jan von Cosel & Sebastian Lenz
 *
 * This file is part of molconv.
 *
 * molconv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * molconv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have recieved a copy of the GNU General Public License
 * along with molconv. If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef MOLECULE_H_
#define MOLECULE_H_

#ifndef Q_MOC_RUN
    #include<chemkit/molecule.h>
    #include<chemkit/atom.h>
#endif
#include<Eigen/Core>
#include"types.h"

namespace molconv {
    class Molecule : public chemkit::Molecule {
     public:
         Molecule();                                                          // the default constructor
         Molecule(const chemkit::Molecule &base_molecule);                    // take a base molecule
         Molecule(const boost::shared_ptr<chemkit::Molecule> &base_mol_ptr);  // same as above, but with pointer

         void show_inertia();                                                 // show the inertia tensor
         void show_covariance();                                              // show the covariance matrix
         void rotate(Eigen::Matrix3d rotation_matrix);                        // rotate the molecule about the origin with a rotation matrix
         void clean_up();                                                     // clean up the coordinates of the molecule
         void set_basis(const origin origin, const basis axes, const int origin_atom=0, const int basis_atom1=0, const int basis_atom2=0, const int basis_atom3=0);
     private:
         void trans2euler();       // calculate the eulerian angles from the transformation matrix
         void euler2trans();       // calculate the transformation matrix from the eulerian angles

         void calc_inertia_tensor();           // calculate the tensor of moments of inertia
         void diag_inertia_tensor();           // diagonalize the inertia tensor
         void calc_covariance_matrix();        // calculate the covariance matrix of the molecule
         void diag_covariance_matrix();        // diagonalize the covariance matrix
         void update_geometrical_props();      // update the geometrical properties (i.e. after an atomic coordinate change)

         size_t number_of_atoms_;              // the number of atoms in the molecule
         double total_mass_;                   // total mass of the molecule
         Eigen::Vector3d center_of_mass_;      // the center of mass of the molecule
         Eigen::Vector3d center_of_geometry_;  // the center of geometry of the molecule
         Eigen::Matrix3d inertia_tensor_;      // tensor of the moments of inertia
         Eigen::Vector3d inertia_eigvals_;     // the eigenvalues of the inertia tensor
         Eigen::Matrix3d inertia_eigvecs_;     // the eigenvectors of the inertia tensor (the principal axes)
         Eigen::Matrix3d covariance_matrix_;        // the covariance matrix of the molecule
         Eigen::Vector3d covariance_eigvals_;       // eigenvalues of the covariance matrix
         Eigen::Matrix3d covariance_eigvecs_;       // eigenvectors of the covariance matrix

         double euler_phi_;                    // |
         double euler_theta_;                  // | the eulerian angles specifying the orientation of the molecule
         double euler_psi_;                    // |
         Eigen::Vector3d origin_;              // origin of the internal coordinate system
         Eigen::Matrix3d basis_;               // the basis vectors of the internal coordinate system in terms
         size_t origin_type_;                  // internal origin
         size_t origin_atom_;                  // atom that defines the internal origin (if set)
         size_t basis_type_;                   // internal basis
         std::vector<size_t> basis_atoms_;     // atoms that define the internal basis (if set)
                                               // of the global coordinates
    };
} // namespace molconv

#endif /* MOLECULE_H_ */

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


#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<cmath>
#include<Eigen/Eigenvalues>
#include<chemkit/bondpredictor.h>
#include"molecule.h"
#include"utilities.h"


namespace molconv {
    /*
     * This is just the default constructor that creates an empty molecule.
     */
    Molecule::Molecule()
        : chemkit::Molecule() {
    }


    /*
     * This constructor takes a chemkit::Molecule as argument and calculates the additional
     * stuff of the molconv::Molecule class / sets it to default values.
     */
    Molecule::Molecule(const chemkit::Molecule &base_molecule)
        : chemkit::Molecule(base_molecule)
        , number_of_atoms_(this->size())
        , total_mass_(this->mass())
        , center_of_mass_(this->centerOfMass())
        , center_of_geometry_(this->center())
        , origin_(Eigen::Vector3d::Zero())
        , basis_(Eigen::Matrix3d::Identity()) {

        std::cout << "This is the first constructor of molconv::Molecule." << std::endl;

        chemkit::BondPredictor::predictBonds(this);

        this->calc_inertia_tensor();
        this->calc_covariance_matrix();

        this->diag_inertia_tensor();
        this->diag_covariance_matrix();
    }


    /*
     * Same as above, but with a pointer.
     */
    Molecule::Molecule(const boost::shared_ptr<chemkit::Molecule> &base_mol_ptr)
        : chemkit::Molecule(*(base_mol_ptr.get()))
        , number_of_atoms_(this->size())
        , total_mass_(this->mass())
        , center_of_mass_(this->centerOfMass())
        , center_of_geometry_(this->center()) {

        std::cout << "This is the second constructor of molconv::Molecule." << std::endl;
        this->calc_inertia_tensor();
        this->calc_covariance_matrix();

        this->diag_inertia_tensor();
        this->diag_covariance_matrix();
    }

    /*
     * This function prints the inertia tensor as well as its eigenvalues and eigenvectors
     * to the standard output
     */
    void Molecule::show_inertia() {
        std::cout << "The inertia tensor:" << std::endl
                  << this->inertia_tensor_ << std::endl;

        std::cout << "The eigenvalues of the inertia tensor:" << std::endl
                  << this->inertia_eigvals_ << std::endl;

        std::cout << "The eigenvectors of the inertia tensor:" << std::endl
                  << this->inertia_eigvecs_ << std::endl;
    }

    /*
     * This function prints the covariance matrix as well as its eigenvalues and eigenvectors
     * to the standard output
     */
    void Molecule::show_covariance() {
        std::cout << "The covariance_matrix:" << std::endl
                  << this->covariance_matrix_ << std::endl;

        std::cout << "The eigenvalues of the covariance matrix:" << std::endl
                  << this->covariance_eigvals_ << std::endl;

        std::cout << "The eigenvectors of the covariance matrix:" << std::endl
                  << this->covariance_eigvecs_ << std::endl;
    }

    /*
     * This function will rotate the molecule using a rotation matrix
     */
    void Molecule::rotate(Eigen::Matrix3d rotation_matrix) {

        std::cout << "using the matrix\n" << rotation_matrix << "\nto rotate the molecule.\n";

        for (size_t atiter = 0; atiter < this->number_of_atoms_; atiter++)
        {
            Eigen::Vector3d new_pos = rotation_matrix * this->atom(atiter)->position();
            this->atom(atiter)->setPosition(new_pos);
        }
    }

    /*
     * This function will clean up the structure of the molecule, i.e. shift it
     * so that the origin of the internal basis equals the origin, and rotate
     * it so that the coordiante axes match the internal basis
     */
    void Molecule::clean_up() {
        std::cout << "Cleaning up the molecule" << std::endl;
        std::cout << "shift vector:" << std::endl << this->center_of_geometry_ - this->origin_ << std::endl;

        this->setCenter(this->center_of_geometry_ - this->origin_);

        this->rotate(this->basis_.transpose());
    }

    /*
     * This function sets the internal basis of the molecule based on the information given
     * in the configuration
     */
    void Molecule::set_basis(const origin origin, const basis axes, const int origin_atom, 
       const int basis_atom1, const int basis_atom2, const int basis_atom3) {

        switch (origin)
        {
            case kCenterOnZero:
                this->origin_type_ = kCenterOnZero;
                this->origin_atom_ = 0;
                this->origin_ = Eigen::Vector3d::Zero();
                break;
            case kCenterOfMass:
                this->origin_type_ = kCenterOfMass;
                this->origin_atom_ = 0;
                this->origin_ = this->center_of_mass_;
                break;
            case kCenterOfGeometry:
                this->origin_type_ = kCenterOfGeometry;
                this->origin_atom_ = 0;
                this->origin_ = this->center_of_geometry_;
                break;
            case kCenterOnAtom:
                this->origin_type_ = kCenterOnAtom;
                this->origin_atom_ = origin_atom;
                this->origin_ = this->atom(origin_atom)->position();
                break;
            default:
                std::cerr << "Serious ERROR in Molecule::set_intbasis while setting the internal origin" << std::endl;
                break;
        }

        switch (axes)
        {
            case kIdentityVectors:
                this->basis_type_ = kIdentityVectors;
                this->basis_atoms_.push_back(0);
                this->basis_atoms_.push_back(0);
                this->basis_atoms_.push_back(0);
                this->basis_ = Eigen::Matrix3d::Identity();
                break;
            case kInertiaVectors:
                this->basis_type_ = kInertiaVectors;
                this->basis_atoms_.push_back(0);
                this->basis_atoms_.push_back(0);
                this->basis_atoms_.push_back(0);
                this->basis_ = this->inertia_eigvecs_;
                break;
            case kCovarianceVectors:
                this->basis_type_ = kCovarianceVectors;
                this->basis_atoms_.push_back(0);
                this->basis_atoms_.push_back(0);
                this->basis_atoms_.push_back(0);
                this->basis_ = this->covariance_eigvecs_;
                break;
            case kVectorsFromAtoms:
            {
                Eigen::Vector3d column1 = this->atom(basis_atom2)->position()
                                        - this->atom(basis_atom1)->position();
                column1 /= column1.norm();

                Eigen::Vector3d column2 = this->atom(basis_atom3)->position()
                                        - this->atom(basis_atom1)->position();

                column2 -= (column1.dot(column2) * column1);
                column2 /= column2.norm();

                Eigen::Vector3d column3 = column1.cross(column2);
                column3 /= column3.norm();

                this->basis_.col(0) = column1;
                this->basis_.col(1) = column2;
                this->basis_.col(2) = column3;
                break;
            }
            default:
                std::cerr << "Serious ERROR in Molecule::set_intbasis while setting the internal origin" << std::endl;
                break;
        }

        std::cout << "internal origin:\n" << this->origin_ << "\ninternal basis:\n" << this->basis_ << std::endl;
        std::cout << "product of internal basis with itself:\n" << this->basis_.transpose() * this->basis_ << std::endl;
    }

    /*
     * This function (re)calculates the geometric properties of the molecule.
     * It should be called e.g. after the coordinates have been changed.
     */
    void Molecule::update_geometrical_props() {
        this->center_of_mass_ = this->centerOfMass();
        this->center_of_geometry_ = this->center();

        this->calc_inertia_tensor();
        this->calc_covariance_matrix();

        this->diag_inertia_tensor();
        this->diag_covariance_matrix();
    }

    /*
     * This function calculates the inertia tensor of the molecule. The elements of the inertia tensor
     * are given by:
     *                     J_ij = sum_k=1^N  m_k * ((r_k)^2 * delta_ij) - x_i * x_j
     *
     * This results in:
     *                          / y^2 + z^2    - x * y     - x * z  \
     *                     J = |   - x * y    x^2 + z^2    - y * z   |
     *                          \  - x * z     - y * z    x^2 + y^2 /
     *
     * where the sum is implicitly included in the matrix elements.
     */
    void Molecule::calc_inertia_tensor() {
        for (size_t alpha = 0; alpha < 3; alpha++)
        {
            for (size_t beta = 0; beta < 3; beta++)
            {
                this->inertia_tensor_(alpha,beta) = 0.0;

                for (size_t atiter = 0; atiter < this->number_of_atoms_; atiter++)
                {
                    double factor = 0.0;

                    if (alpha == beta)
                        factor = (this->atom(atiter)->position() - this->center_of_mass_)
                                 .dot(this->atom(atiter)->position() - this->center_of_mass_);

                    factor -= (this->atom(atiter)->position() - this->center_of_mass_)(alpha)
                            * (this->atom(atiter)->position() - this->center_of_mass_)(beta);

                    this->inertia_tensor_(alpha,beta) += this->atom(atiter)->mass() * factor;
                }
            }
        }
    }


    /*
     * This function diagonalizes the inertia tensor using a routine from the EIGEN library.
     * The eigenvectors of the inertia tensor are the principal axes of the molecule and these are
     * used as the coordinate axes for the molecule fixed coordinate system. The eigenvalues of
     * the inertia tensor are the moments of inertia along those axes.
     */
    void Molecule::diag_inertia_tensor() {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(this->inertia_tensor_);

        if (solver.info() != Eigen::Success)
        {
            std::cerr << "The inertia tensor could not be diagonalized" << std::endl;
        }
        else
        {
            this->inertia_eigvals_ = solver.eigenvalues();
            this->inertia_eigvecs_ = solver.eigenvectors();
        }
    }


    /*
     * This function calculates the covariance matrix of the molecule. The variance is a measure of the
     * spread of the atoms in the respective direction. The covariance matrix is calculated as:
     *
     *                 C_ij = 1/N * sum_k=1^N  (x_i - u_i) * (x_j - u_j)
     *
     * or in matrix form:
     *                             /     (x - u_x)^2      (x - u_x)*(y - u_y)  (x - u_x)*(y - u_y) \
     *                 C = 1/N *  |  (x - u_x)*(y - u_y)      (y - u_y)^2      (y - u_y)*(z - u_z)  |
     *                             \ (x - u_x)*(y - u_y)  (y - u_y)*(z - u_z)      (z - u_z)^2     /
     *
     * where u_x, u_y, u_z are the positions of the GEOMETRIC center of the molecule (i.e. the center
     * of geometry, see respective function above).
     */
    void Molecule::calc_covariance_matrix() {
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                this->covariance_matrix_(i,j) = 0.0;

                for (size_t atiter = 0; atiter < this->number_of_atoms_; atiter++)
                {
                    this->covariance_matrix_(i,j) += ((this->atom(atiter)->position()(i) - this->center_of_geometry_(i))
                                           * (this->atom(atiter)->position()(j) - this->center_of_geometry_(j)));
                }
                this->covariance_matrix_(i,j) /= this->number_of_atoms_;
            }
        }
    }

    /*
     * This function diagonalizes the covariance matrix. The eigenvectors of the covariance matrix are
     * the directions with the minimum and maximum variance, respectively, while the corresponding
     * eigenvalues are the variances along these directions. For example, the eigenvector corresponding
     * to the lowest variance stands perpendicular to the least squares plane of the molecule.
     */
    void Molecule::diag_covariance_matrix() {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(this->covariance_matrix_);

        if (solver.info() != Eigen::Success)
        {
            std::cerr << "The covariance matrix could not be diagonalized" << std::endl;
        }
        else
        {
            this->covariance_eigvals_ = solver.eigenvalues();
            this->covariance_eigvecs_ = solver.eigenvectors();
        }
    }

    /*
     * This function calculates the elements of the transformation matrix from the eulerian angles
     * describing the rotations from the external coordinate system to the internal coordinate system.
     * the definition of the eulerian angles is adapted from: H. Goldstein, "classical mechanics".
     */
    void Molecule::euler2trans() {
        this->basis_(0,0) =  cos(this->euler_psi_) * cos(this->euler_phi_)
                                      - cos(this->euler_theta_) * cos(this->euler_phi_) * cos(this->euler_psi_);
        this->basis_(0,1) =  cos(this->euler_psi_) * sin(this->euler_phi_)
                                      + cos(this->euler_theta_) * cos(this->euler_phi_) * sin(this->euler_psi_);
        this->basis_(0,2) =  sin(this->euler_psi_) * sin(this->euler_theta_);
        this->basis_(1,0) = -sin(this->euler_psi_) * cos(this->euler_phi_)
                                      - cos(this->euler_theta_) * sin(this->euler_phi_) * cos(this->euler_psi_);
        this->basis_(1,1) = -sin(this->euler_psi_) * sin(this->euler_phi_)
                                      + cos(this->euler_theta_) * cos(this->euler_phi_) * cos(this->euler_psi_);
        this->basis_(1,2) =  cos(this->euler_psi_) * sin(this->euler_theta_);
        this->basis_(2,0) =  sin(this->euler_theta_) * sin(this->euler_phi_);
        this->basis_(2,1) = -sin(this->euler_theta_) * cos(this->euler_phi_);
        this->basis_(2,2) =  cos(this->euler_theta_);

        this->euler_theta_ = 1.0;
    }

    /*
     * This function calculates the eulerian angles describing the rotations from the external (global) coordinate
     * system to the internal coordinate system from a given transformation matrix performing these rotations.
     */
    void Molecule::trans2euler() {
        double abs_l = sqrt(pow(this->basis_(2,0), 2) + pow(this->basis_(2,1), 2));
    
        this->euler_theta_ = acos(this->basis_(2,2));
        this->euler_phi_ = acos(-this->basis_(2,1)/abs_l);
        this->euler_psi_ = acos((this->basis_(0,1)*this->basis_(2,0)-this->basis_(0,0)*this->basis_(2,1))/abs_l);
    }

}

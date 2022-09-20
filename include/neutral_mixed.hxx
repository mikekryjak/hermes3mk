
#pragma once
#ifndef NEUTRAL_MIXED_H
#define NEUTRAL_MIXED_H

#include <memory>
#include <string>

#include <invert_laplace.hxx>

#include "component.hxx"

/// Evolve density, parallel momentum and pressure
/// for a neutral gas species with cross-field diffusion
struct NeutralMixed : public Component {
  ///
  /// @param name     The name of the species e.g. "h"
  /// @param options  Top-level options. Settings will be taken from options[name]
  /// @param solver   Time-integration solver to be used
  NeutralMixed(const std::string& name, Options& options, Solver *solver);
  
  /// Modify the given simulation state
  void transform(Options &state) override;
  
  /// Use the final simulation state to update internal state
  /// (e.g. time derivatives)
  void finally(const Options &state) override;

  /// Add extra fields for output, or set attributes e.g docstrings
  void annotate(Options &state) override;

  /// Preconditioner
  void precon(const Options &state, BoutReal gamma) override;
private:
  std::string name;  ///< Species name
  
  Field3D Nn, Pn, NVn; // Density, pressure and parallel momentum
  Field3D Vn; ///< Neutral parallel velocity
  Field3D Tn; ///< Neutral temperature
  Field3D Nnlim, Pnlim, Vnlim; // Limited in regions of low density

  BoutReal AA; ///< Atomic mass (proton = 1)

  Field3D Dnn; ///< Diffusion coefficient
  
  bool sheath_ydown, sheath_yup;
  
  BoutReal neutral_gamma; ///< Heat transmission for neutrals
  
  BoutReal nn_floor; ///< Minimum Nn used when dividing NVn by Nn to get Vn.
  
  bool precondition {true}; ///< Enable preconditioner?
  std::unique_ptr<Laplacian> inv; ///< Laplacian inversion used for preconditioning

  Field3D Sn, Sp, Snv; ///< Particle, pressure and momentum source
};

namespace {
RegisterComponent<NeutralMixed> registersolverneutralmixed("neutral_mixed");
}

#endif // NEUTRAL_MIXED_H

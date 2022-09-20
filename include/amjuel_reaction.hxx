#pragma once
#ifndef AMJUEL_REACTION_H
#define AMJUEL_REACTION_H

#include "component.hxx"
#include "integrate.hxx"

struct AmjuelReaction : public Component {
  AmjuelReaction(std::string, Options& alloptions, Solver*) {
    // Get the units
    const auto& units = alloptions["units"];
    Tnorm = get<BoutReal>(units["eV"]);
    Nnorm = get<BoutReal>(units["inv_meters_cubed"]);
    FreqNorm = 1. / get<BoutReal>(units["seconds"]);
  }

protected:
  BoutReal Tnorm, Nnorm, FreqNorm; // Normalisations

  BoutReal clip(BoutReal value, BoutReal min, BoutReal max) {
    if (value < min)
      return min;
    if (value > max)
      return max;
    return value;
  }

  /// Evaluate a double polynomial fit in n and T
  /// (page 20 of amjuel.pdf)
  ///
  ///  coefs[T][n]
  /// Input in units:
  ///     n in m^-3
  ///     T in eV
  ///
  /// Output in SI, units m^3/s, or eV m^3/s for energy loss
  template <size_t rows, size_t cols>
  BoutReal evaluate(const BoutReal (&coefs)[rows][cols], BoutReal T, BoutReal n) {

    // Enforce range of validity
    n = clip(n, 1e14, 1e22); // 1e8 - 1e16 cm^-3
    T = clip(T, 0.1, 1e4);

    BoutReal logntilde = log(n / 1e14); // Note: 1e8 cm^-3
    BoutReal logT = log(T);
    BoutReal result = 0.0;

    BoutReal logT_n = 1.0; // log(T) ** n
    for (size_t n = 0; n < cols; ++n) {
      BoutReal logn_m = 1.0; // log(ntilde) ** m
      for (size_t m = 0; m < rows; ++m) {
        result += coefs[n][m] * logn_m * logT_n;
        logn_m *= logntilde;
      }
      logT_n *= logT;
    }
    return exp(result) * 1e-6; // Note: convert cm^3 to m^3
  }

  /// Electron-driven reaction
  /// e + from_ion -> to_ion [ + e? + e?]
  ///
  /// Coefficients from Amjuel:
  ///  - rate_coefs        Double-polynomial log fit [T][n] for <σv>
  ///  - radiation_coefs   Double-polynomial log fit [T][n] for electron loss
  /// electron_heating  Energy added to electrons per reaction [eV]
  template <size_t rows, size_t cols>
  void electron_reaction(Options& electron, Options& from_ion, Options& to_ion,
                         const BoutReal (&rate_coefs)[rows][cols],
                         const BoutReal (&radiation_coefs)[rows][cols],
                         BoutReal electron_heating) {
    Field3D Ne = get<Field3D>(electron["density"]);
    Field3D Te = get<Field3D>(electron["temperature"]);

    Field3D N1 = get<Field3D>(from_ion["density"]);
    Field3D T1 = get<Field3D>(from_ion["temperature"]);
    Field3D V1 = get<Field3D>(from_ion["velocity"]);

    auto AA = get<BoutReal>(from_ion["AA"]);
    ASSERT1(AA == get<BoutReal>(to_ion["AA"]));

    const BoutReal from_charge =
        from_ion.isSet("charge") ? get<BoutReal>(from_ion["charge"]) : 0.0;
    const BoutReal to_charge =
        to_ion.isSet("charge") ? get<BoutReal>(to_ion["charge"]) : 0.0;

    Field3D reaction_rate = cellAverage(
        [&](BoutReal ne, BoutReal n1, BoutReal te) {
          return ne * n1 * evaluate(rate_coefs, te * Tnorm, ne * Nnorm) * Nnorm
                 / FreqNorm;
        },
        Ne.getRegion("RGN_NOBNDRY"))(Ne, N1, Te);

    // Particles
    subtract(from_ion["density_source"], reaction_rate);
    add(to_ion["density_source"], reaction_rate);

    if (from_charge != to_charge) {
      // To ensure quasineutrality, add electron density source
      add(electron["density_source"], (to_charge - from_charge) * reaction_rate);
    }

    // Momentum
    Field3D momentum_exchange = reaction_rate * AA * V1;

    subtract(from_ion["momentum_source"], momentum_exchange);
    add(to_ion["momentum_source"], momentum_exchange);

    // Ion energy
    Field3D energy_exchange = reaction_rate * (3. / 2) * T1;
    subtract(from_ion["energy_source"], energy_exchange);
    add(to_ion["energy_source"], energy_exchange);

    // Electron energy loss (radiation, ionisation potential)
    Field3D energy_loss = cellAverage(
        [&](BoutReal ne, BoutReal n1, BoutReal te) {
          return ne * n1 * evaluate(radiation_coefs, te * Tnorm, ne * Nnorm) * Nnorm
                 / (Tnorm * FreqNorm);
        },
        Ne.getRegion("RGN_NOBNDRY"))(Ne, N1, Te);

    // Loss is reduced by heating
    energy_loss -= (electron_heating / Tnorm) * reaction_rate;

    subtract(electron["energy_source"], energy_loss);
  }
};

#endif // AMJUEL_REACTION_H

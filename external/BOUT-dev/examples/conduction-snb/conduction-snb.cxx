// SNB heat conduction model

#include <bout/snb.hxx>
#include <bout.hxx>

int main(int argc, char** argv) {
  using bout::HeatFluxSNB;

  BoutInitialise(argc, argv);

  // Read the density and temperature profiles
  Options& opt = Options::root();

  Field3D Ne = opt["Ne"].doc("Electron density in m^-3").as<Field3D>();
  Field3D Te = opt["Te"].doc("Electron temperature in eV").as<Field3D>();

  bout::globals::mesh->communicate(Ne, Te);

  // Calculate divergence of heat flux
  HeatFluxSNB snb;
  Field3D Div_Q_SH;
  Field3D Div_Q = snb.divHeatFlux(Te, Ne, &Div_Q_SH);

  // Save to the output
  SAVE_ONCE(Ne, Te, Div_Q, Div_Q_SH);
  bout::globals::dump.write();

  BoutFinalise();
  return 0;
}

bout_add_integrated_test(test-options-netcdf
  SOURCES test-options-netcdf.cxx
  USE_RUNTEST
  USE_DATA_BOUT_INP
  REQUIRES BOUT_HAS_NETCDF
  CONFLICTS BOUT_HAS_LEGACY_NETCDF
  )

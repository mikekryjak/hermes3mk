/// Information about the version of Hermes
///
/// The build system will update this file on every commit, which may
/// result in files that include it getting rebuilt. Therefore it
/// should be included in as few places as possible

#ifndef HERMES_REVISION_H
#define HERMES_REVISION_H

namespace hermes {
namespace version {
/// The git commit hash
#ifndef HERMES_REVISION
constexpr auto revision = "a4e6f5ce17806f9159336b213d87e1d7e854ae7f";
#else
// Stringify value passed at compile time
#define BUILDFLAG1_(x) #x
#define BUILDFLAG(x) BUILDFLAG1_(x)
constexpr auto revision = BUILDFLAG(HERMES_REVISION);
#undef BUILDFLAG1
#undef BUILDFLAG
#endif
} // namespace version
} // namespace hermes

#endif // HERMES_REVISION_H

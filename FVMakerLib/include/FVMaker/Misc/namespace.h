#ifndef __FVMAKER_NAMESPACE_CONFIG_H__
#define __FVMAKER_NAMESPACE_CONFIG_H__

// Macro to open the main namespace for the FVM library
#define FVMAKER_NAMESPACE_OPEN namespace fvm {

// Macro to close the main namespace
#define FVMAKER_NAMESPACE_CLOSE }

// Macro to open the grid namespace within the FVM namespace
#define GRID_NAMESPACE_OPEN namespace fvm { namespace grid {

// Macro to close the grid namespace within the gripp namespace
#define GRID_NAMESPACE_CLOSE } }

#endif // __FVMAKER_NAMESPACE_CONFIG_H__

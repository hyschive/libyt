#include "yt_combo.h"
#include "libyt.h"




//-------------------------------------------------------------------------------------------------------
// Function    :  yt_set_parameter
// Description :  Set YT-specific parameters
//
// Note        :  1. Store the input "param_yt" to libyt.param_yt
//
// Parameter   :  param_yt : Structure storing all YT-specific parameters
//
// Return      :  YT_SUCCESS or YT_FAIL
//-------------------------------------------------------------------------------------------------------
int yt_set_parameter( yt_param_yt *param_yt )
{

// check if libyt has been initialized
   if ( g_param_libyt.libyt_initialized )
      log_info( "Setting YT parameters ...\n" );
   else
      YT_ABORT( "Please invoke yt_init() before calling %s()!\n", __FUNCTION__ );


// check if this function has been called previously
   if ( g_param_libyt.param_yt_set )
   {
      log_warning( "%s() has been called already!\n", __FUNCTION__ );
      log_warning( "==> Are you trying to overwrite existing parameters?\n" );
   }


// reset all cosmological parameters to zero for non-cosmological datasets
   if ( !param_yt->cosmological_simulation ) {
      param_yt->current_redshift = param_yt->omega_lambda = param_yt->omega_matter = param_yt->hubble_constant = 0.0; }


// check if all parameters have been set properly
   if ( param_yt->validate() )
      log_debug( "Validating YT parameters ... done\n" );
   else
      YT_ABORT(  "Validating YT parameters ... failed\n" );


// print out all parameters
   log_debug( "List of YT parameters:\n" );
   param_yt->show();


// store user-provided parameters to a libyt internal variable
// ==> must do this before calling allocate_hierarchy() since it will need "g_param_yt.num_grids"
// ==> must do this before setting the default figure base name since it will overwrite g_param_yt.fig_basename
   g_param_yt = *param_yt;


// set the default figure base name if it's not set by users
   if ( param_yt->fig_basename == NULL )
   {
      char fig_basename[15];
      sprintf( fig_basename, "Fig%09ld", g_param_libyt.counter );

      g_param_yt.fig_basename = fig_basename;
   }


// export data to libyt.param_yt
// strings
   add_dict_string(  g_py_param_yt, "frontend",                g_param_yt.frontend                );
   add_dict_string(  g_py_param_yt, "fig_basename",            g_param_yt.fig_basename            );

// scalars
   add_dict_scalar(  g_py_param_yt, "current_time",            g_param_yt.current_time            );
   add_dict_scalar(  g_py_param_yt, "current_redshift",        g_param_yt.current_redshift        );
   add_dict_scalar(  g_py_param_yt, "omega_lambda",            g_param_yt.omega_lambda            );
   add_dict_scalar(  g_py_param_yt, "omega_matter",            g_param_yt.omega_matter            );
   add_dict_scalar(  g_py_param_yt, "hubble_constant",         g_param_yt.hubble_constant         );
   add_dict_scalar(  g_py_param_yt, "length_unit",             g_param_yt.length_unit             );
   add_dict_scalar(  g_py_param_yt, "mass_unit",               g_param_yt.mass_unit               );
   add_dict_scalar(  g_py_param_yt, "time_unit",               g_param_yt.time_unit               );
   add_dict_scalar(  g_py_param_yt, "cosmological_simulation", g_param_yt.cosmological_simulation );
   add_dict_scalar(  g_py_param_yt, "dimensionality",          g_param_yt.dimensionality          );
   add_dict_scalar(  g_py_param_yt, "refine_by",               g_param_yt.refine_by               );
   add_dict_scalar(  g_py_param_yt, "num_grids",               g_param_yt.num_grids               );

// vectors (stored as Python tuples)
   add_dict_vector3( g_py_param_yt, "domain_left_edge",        g_param_yt.domain_left_edge        );
   add_dict_vector3( g_py_param_yt, "domain_right_edge",       g_param_yt.domain_right_edge       );
   add_dict_vector3( g_py_param_yt, "periodicity",             g_param_yt.periodicity             );
   add_dict_vector3( g_py_param_yt, "domain_dimensions",       g_param_yt.domain_dimensions       );

   log_debug( "Inserting YT parameters to libyt.param_yt ... done\n" );


// fill libyt.hierarchy with NumPy arrays allocated but uninitialized
   if ( allocate_hierarchy() )
      log_debug( "Allocating libyt.hierarchy ... done\n" );
   else
      YT_ABORT(  "Allocating libyt.hierarchy ... failed!\n" );


   g_param_libyt.param_yt_set = true;

   return YT_SUCCESS;

} // FUNCTION : yt_set_parameter

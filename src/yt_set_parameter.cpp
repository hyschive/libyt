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
   if ( g_initialized )
      log_info( "Setting YT parameters ...\n" );
   else
      YT_ABORT( "Please invoke yt_init before calling %s!\n", __FUNCTION__ );


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


// export data to libyt.param_yt
// scalars
   add_dict_scalar(  g_param_yt, "current_time",            param_yt->current_time            );
   add_dict_scalar(  g_param_yt, "current_redshift",        param_yt->current_redshift        );
   add_dict_scalar(  g_param_yt, "omega_lambda",            param_yt->omega_lambda            );
   add_dict_scalar(  g_param_yt, "omega_matter",            param_yt->omega_matter            );
   add_dict_scalar(  g_param_yt, "hubble_constant",         param_yt->hubble_constant         );
   add_dict_scalar(  g_param_yt, "length_unit",             param_yt->length_unit             );
   add_dict_scalar(  g_param_yt, "mass_unit",               param_yt->mass_unit               );
   add_dict_scalar(  g_param_yt, "time_unit",               param_yt->time_unit               );
   add_dict_scalar(  g_param_yt, "cosmological_simulation", param_yt->cosmological_simulation );
   add_dict_scalar(  g_param_yt, "dimensionality",          param_yt->dimensionality          );

// vectors (stored as Python tuples)
   add_dict_vector3( g_param_yt, "domain_left_edge",        param_yt->domain_left_edge        );
   add_dict_vector3( g_param_yt, "domain_right_edge",       param_yt->domain_right_edge       );
   add_dict_vector3( g_param_yt, "periodicity",             param_yt->periodicity             );
   add_dict_vector3( g_param_yt, "domain_dimensions",       param_yt->domain_dimensions       );


   log_debug( "Inserting YT parameters ... done\n" );

   return YT_SUCCESS;

} // FUNCTION : yt_set_parameter
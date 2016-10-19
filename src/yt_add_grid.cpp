#include "yt_combo.h"
#include "libyt.h"




//-------------------------------------------------------------------------------------------------------
// Function    :  yt_add_grid
// Description :  Add a single grid to the libyt Python module
//
// Note        :  1. Store the input "grid" to libyt.hierarchy and libyt.grid_data
//                2. Must call yt_set_parameter() in advance, which will set the total number of grids and
//                   preallocate memory for NumPy arrays
//
// Parameter   :  grid : Structure storing all information of a single grid
//
// Return      :  YT_SUCCESS or YT_FAIL
//-------------------------------------------------------------------------------------------------------
int yt_add_grid( yt_grid *grid )
{

// check if libyt has been initialized
   if ( !g_param_libyt.libyt_initialized )
      YT_ABORT( "Please invoke yt_init() before calling %s()!\n", __FUNCTION__ );


// check if YT parameters have been set
   if ( !g_param_libyt.param_yt_set )
      YT_ABORT( "Please invoke yt_set_parameter() before calling %s()!\n", __FUNCTION__ );


// check if all parameters have been set properly
   if ( grid->validate() )
      log_debug( "Validating input grid [%15ld] ... done\n", grid->id );
   else
      YT_ABORT(  "Validating input grid [%ld] ... failed\n", grid->id );


// additional checks that depend on input YT parameters
// grid ID
   if ( grid->id >= g_param_yt.num_grids )
      YT_ABORT( "Grid ID (%ld) >= total number of grids (%ld)!\n",
                grid->id, g_param_yt.num_grids );

   if ( grid->parent_id >= g_param_yt.num_grids )
      YT_ABORT( "Grid [%ld] parent ID (%ld) >= total number of grids (%ld)!\n",
                grid->id, grid->parent_id, g_param_yt.num_grids );

// edge
   for (int d=0; d<g_param_yt.dimensionality; d++)
   {
      if ( grid->left_edge[d] < g_param_yt.domain_left_edge[d] )
         YT_ABORT( "Grid [%ld] left edge (%13.7e) < domain left edge (%13.7e) along the dimension %d!\n",
                   grid->id, grid->left_edge[d], g_param_yt.domain_left_edge[d], d );

      if ( grid->right_edge[d] > g_param_yt.domain_right_edge[d] )
         YT_ABORT( "Grid [%ld] right edge (%13.7e) > domain right edge (%13.7e) along the dimension %d!\n",
                   grid->id, grid->right_edge[d], g_param_yt.domain_right_edge[d], d );
   }



// export data to libyt.hierarchy
   /*
// strings
   add_dict_string(  g_param_yt, "frontend",                param_yt->frontend                );

// scalars
   add_dict_scalar(  g_param_yt, "current_time",            param_yt->current_time            );

// vectors (stored as Python tuples)
   add_dict_vector3( g_param_yt, "domain_left_edge",        param_yt->domain_left_edge        );
   */


   return YT_SUCCESS;

} // FUNCTION : yt_add_grid

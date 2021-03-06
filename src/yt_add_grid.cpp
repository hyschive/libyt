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
   if ( !grid->validate() )
      YT_ABORT(  "Validating input grid [%ld] ... failed\n", grid->id );


// additional checks that depend on input YT parameters
// grid ID
   if ( grid->id >= g_param_yt.num_grids )
      YT_ABORT( "Grid ID [%ld] >= total number of grids [%ld]!\n",
                grid->id, g_param_yt.num_grids );

   if ( grid->parent_id >= g_param_yt.num_grids )
      YT_ABORT( "Grid [%ld] parent ID [%ld] >= total number of grids [%ld]!\n",
                grid->id, grid->parent_id, g_param_yt.num_grids );

   if ( grid->level > 0  &&  grid->parent_id < 0 )
      YT_ABORT( "Grid [%ld] parent ID [%ld] < 0 at level [%d]!\n",
                grid->id, grid->parent_id, grid->level );

// edge
   for (int d=0; d<g_param_yt.dimensionality; d++)
   {
      if ( grid->left_edge[d] < g_param_yt.domain_left_edge[d] )
         YT_ABORT( "Grid [%ld] left edge [%13.7e] < domain left edge [%13.7e] along the dimension [%d]!\n",
                   grid->id, grid->left_edge[d], g_param_yt.domain_left_edge[d], d );

      if ( grid->right_edge[d] > g_param_yt.domain_right_edge[d] )
         YT_ABORT( "Grid [%ld] right edge [%13.7e] > domain right edge [%13.7e] along the dimension [%d]!\n",
                   grid->id, grid->right_edge[d], g_param_yt.domain_right_edge[d], d );
   }


// check if this grid has been set previously
   if ( g_param_libyt.grid_set[ grid->id ] == true )
      YT_ABORT( "Grid [%ld] has been set already!\n", grid->id );


// export grid info to libyt.hierarchy
   PyArrayObject *py_array_obj;

// convenient macro
// note that PyDict_GetItemString() returns a **borrowed** reference ==> no need to call Py_DECREF
#  define FILL_ARRAY( KEY, ARRAY, DIM, TYPE )                                                            \
   {                                                                                                     \
      for (int t=0; t<DIM; t++)                                                                          \
      {                                                                                                  \
         if (  ( py_array_obj = (PyArrayObject*)PyDict_GetItemString( g_py_hierarchy, KEY ) ) == NULL )  \
            YT_ABORT( "Accessing the key \"%s\" from libyt.hierarchy ... failed!\n", KEY );              \
                                                                                                         \
         *(TYPE*)PyArray_GETPTR2( py_array_obj, grid->id, t ) = (TYPE)(ARRAY)[t];                        \
      }                                                                                                  \
   }

   FILL_ARRAY( "grid_left_edge",       grid->left_edge,      3, npy_double );
   FILL_ARRAY( "grid_right_edge",      grid->right_edge,     3, npy_double );
   FILL_ARRAY( "grid_dimensions",      grid->dimensions,     3, npy_long   );
   FILL_ARRAY( "grid_particle_count", &grid->particle_count, 1, npy_long   );
   FILL_ARRAY( "grid_parent_id",      &grid->parent_id,      1, npy_long   );
   FILL_ARRAY( "grid_levels",         &grid->level,          1, npy_long   );

   log_debug( "Inserting grid [%15ld] info to libyt.hierarchy ... done\n", grid->id );


// export grid data to libyt.grid_data as "libyt.grid_data[grid_id][field_label][field_data]"
   int      grid_ftype   = (grid->field_ftype == YT_FLOAT ) ? NPY_FLOAT : NPY_DOUBLE;
   npy_intp grid_dims[3] = { grid->dimensions[0], grid->dimensions[1], grid->dimensions[2] };
   PyObject *py_grid_id, *py_field_labels, *py_field_data;

// allocate [grid_id][field_label]
   py_grid_id      = PyLong_FromLong( grid->id );
   py_field_labels = PyDict_New();

   PyDict_SetItem( g_py_grid_data, py_grid_id, py_field_labels );

// fill [grid_id][field_label][field_data]
   for (int v=0; v<grid->num_fields; v++)
   {
//    PyArray_SimpleNewFromData simply creates an array wrapper and does note allocate and own the array
      py_field_data = PyArray_SimpleNewFromData( 3, grid_dims, grid_ftype, grid->field_data[v] );

//    add the field data to "libyt.grid_data[grid_id][field_label]"
      PyDict_SetItemString( py_field_labels, grid->field_labels[v], py_field_data );

//    call decref since PyDict_SetItemString() returns a new reference
      Py_DECREF( py_field_data );
   }

// call decref since both PyLong_FromLong() and PyDict_New() return a new reference
   Py_DECREF( py_grid_id );
   Py_DECREF( py_field_labels );

   log_debug( "Inserting grid [%15ld] data to libyt.hierarchy ... done\n", grid->id );


// record that the grid "grid->id" has been set successfully
   g_param_libyt.grid_set[ grid->id ] = true;


   return YT_SUCCESS;

} // FUNCTION : yt_add_grid

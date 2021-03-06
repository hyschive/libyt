#ifndef __YT_TYPE_GRID_H__
#define __YT_TYPE_GRID_H__



/*******************************************************************************
/
/  yt_grid structure
/
/  ==> included by yt_type.h
/
********************************************************************************/


//-------------------------------------------------------------------------------------------------------
// Structure   :  yt_grid
// Description :  Data structure to store a single grid
//
// Data Member :  dimensions     : Number of cells along each direction
//                left_edge      : Grid left  edge in code units
//                right_edge     : Grid right edge in code units
//                particle_count : Nunber of particles in this grid
//                level          : AMR level (0 for the root level)
//                id             : Grid ID (0-indexed ==> must be in the range 0 <= id < total number of grids)
//                parent_id      : Parent grid ID (0-indexed, -1 for grids on the root level)
//                num_fields     : Number of fields
//                field_labels   : Name of each field (e.g., density, temperature, ...)
//                field_data     : Pointer arrays pointing to the data of each field
//                field_ftype    : Floating-point type of "field_data" ==> YT_FLOAT or YT_DOUBLE
//
// Method      :  yt_grid  : Constructor
//               ~yt_grid  : Destructor
//                validate : Check if all data members have been set properly by users
//-------------------------------------------------------------------------------------------------------
struct yt_grid
{

// data members
// ===================================================================================
   double left_edge[3];
   double right_edge[3];

   long   particle_count;
   long   id;
   long   parent_id;

   int    dimensions[3];
   int    level;

   int          num_fields;
   const char **field_labels;
   void       **field_data;
   yt_ftype     field_ftype;


   //===================================================================================
   // Method      :  yt_grid
   // Description :  Constructor of the structure "yt_grid"
   //
   // Note        :  Initialize all data members
   //
   // Parameter   :  None
   //===================================================================================
   yt_grid()
   {

//    set defaults
      for (int d=0; d<3; d++) {
      left_edge [d]  = FLT_UNDEFINED;
      right_edge[d]  = FLT_UNDEFINED; }

      for (int d=0; d<3; d++) {
      dimensions[d]  = INT_UNDEFINED; }

      particle_count = INT_UNDEFINED;
      id             = INT_UNDEFINED;
      parent_id      = INT_UNDEFINED;
      level          = INT_UNDEFINED;

      num_fields     = INT_UNDEFINED;
      field_labels   = NULL;
      field_data     = NULL;
      field_ftype    = YT_FTYPE_UNKNOWN;

   } // METHOD : yt_grid


   //===================================================================================
   // Method      :  ~yt_grid
   // Description :  Destructor of the structure "yt_grid"
   //
   // Note        :  1. Not used currently
   //                2. We do not free the pointer arrays "field_labels" and "field_data" here
   //                   ==> They must be free'd by users
   //
   // Parameter   :  None
   //===================================================================================
   ~yt_grid()
   {

   } // METHOD : ~yt_grid


   //===================================================================================
   // Method      :  validate
   // Description :  Check if all data members have been set properly by users
   //
   // Note        :  1. This function does not perform checks that depend on the input
   //                   YT parameters (e.g., whether left_edge lies within the simulation domain)
   //                   ==> These checks are performed in yt_add_grid()
   //
   // Parameter   :  None
   //
   // Return      :  YT_SUCCESS or YT_FAIL
   //===================================================================================
   int validate() const
   {

      for (int d=0; d<3; d++) {
      if ( left_edge [d]  == FLT_UNDEFINED    )   YT_ABORT( "\"%s[%d]\" has not been set for grid [%ld]!\n", "left_edge",  d,  id );
      if ( right_edge[d]  == FLT_UNDEFINED    )   YT_ABORT( "\"%s[%d]\" has not been set for grid [%ld]!\n", "right_edge", d,  id ); }

      for (int d=0; d<3; d++) {
      if ( dimensions[d]  == INT_UNDEFINED    )   YT_ABORT( "\"%s[%d]\" has not been set for grid [%ld]!\n", "dimensions", d,  id ); }
      if ( particle_count == INT_UNDEFINED    )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "particle_count", id );
      if ( id             == INT_UNDEFINED    )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "id",             id );
      if ( parent_id      == INT_UNDEFINED    )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "parent_id",      id );
      if ( level          == INT_UNDEFINED    )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "level",          id );
      if ( num_fields     == INT_UNDEFINED    )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "num_fields",     id );
      if ( field_labels   == NULL             )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "field_labels",   id );
      if ( field_data     == NULL             )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "field_data",     id );
      if ( field_ftype    == YT_FTYPE_UNKNOWN )   YT_ABORT(     "\"%s\" has not been set for grid [%ld]!\n", "field_ftype",    id );

//    additional checks
      for (int d=0; d<3; d++) {
      if ( dimensions[d] <= 0 )   YT_ABORT( "\"%s[%d]\" == %d <= 0 for grid [%ld]!\n", "dimensions", d, dimensions[d], id ); }
      if ( particle_count < 0 )   YT_ABORT( "\"%s\" == %d < 0 for grid [%ld]!\n", "particle_count", particle_count, id );
      if ( id < 0 )               YT_ABORT( "\"%s\" == %d < 0!\n", "id", id );
      if ( level < 0 )            YT_ABORT( "\"%s\" == %d < 0 for grid [%ld]!\n", "level", level, id );
      if ( num_fields <= 0 )      YT_ABORT( "\"%s\" == %d <= 0 for grid [%ld]!\n", "num_fields", num_fields, id );
      if ( field_ftype != YT_FLOAT  &&  field_ftype != YT_DOUBLE )
         YT_ABORT( "Unknown \"%s\" == %d for grid [%ld]!\n", "field_ftype", field_ftype, id );

      return YT_SUCCESS;

   } // METHOD : validate

}; // struct yt_grid



#endif // #ifndef __YT_TYPE_GRID_H__

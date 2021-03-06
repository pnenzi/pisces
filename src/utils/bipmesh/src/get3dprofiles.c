/*----------------------------------------------------------------------
**  Copyright 1989 by
**  The Board of Trustees of the Leland Stanford Junior University
**  All rights reserved.
**
**  This routine may not be used without the prior written consent of
**  the Board of Trustees of the Leland Stanford University.
**----------------------------------------------------------------------
**/

/*	get3dprofiles.c		Version 1.2		*/
/*	Last Modification:	1/22/90 13:20:27		*/


/* get3dprofiles - gathers user information for profiles */

#include <stdio.h>
#include "struct.h"

get3dprofiles()

{
    int typeofprofile;
    char buf[3];


    switch ( dopingmode )  {
	case  0  : 
	
		typeofprofile = 0;
    		read_dop("Emitter", &(dop_data[0]), 
			vert_info.substrate_dop, typeofprofile,
			&(vert_info.emitter_depth) );
    		read_dop("Internal Base", &(dop_data[1]), 
		    vert_info.substrate_dop, typeofprofile,
		    &(vert_info.int_base_depth) );
		read_dop("External Base", &(dop_data[3]),
		    vert_info.substrate_dop, typeofprofile,
		    &(vert_info.ext_base_depth) );
		if ( vert_info.int_base_depth <= vert_info.emitter_depth )
		     panic( "base depth less than emitter depth. try again\n");
    		read_dop("Collector", &(dop_data[2]), 
			vert_info.substrate_dop, typeofprofile,
			&(vert_info.collector_depth) );
		if ( ( vert_info.collector_depth <= vert_info.int_base_depth )
		  || ( vert_info.collector_depth <= vert_info.ext_base_depth ) )
		     panic("collector depth less than base depth. try again\n");
		break;

  	case 1  :  	
	
		typeofprofile = 1;
    		read_dop("Emitter", &(dop_data[0]), 
			vert_info.substrate_dop, typeofprofile,
			&(vert_info.emitter_depth) );
    		read_dop("Internal Base", &(dop_data[1]), 
		    vert_info.substrate_dop, typeofprofile,
		    &(vert_info.int_base_depth) );
    		read_dop("External Base", &(dop_data[3]), 
		    vert_info.substrate_dop, typeofprofile,
		    &(vert_info.ext_base_depth) );
		if ( vert_info.int_base_depth <= vert_info.emitter_depth )
		     panic( "base depth less than emitter depth. try again\n");
    		read_dop("Collector", &(dop_data[2]), 
			vert_info.substrate_dop, typeofprofile,
			&(vert_info.collector_depth) );
		if ( ( vert_info.collector_depth <= vert_info.int_base_depth )
		  || ( vert_info.collector_depth <= vert_info.ext_base_depth ) )
		     panic("collector depth less than base depth. try again\n");
		if  (dop_data[2].type != SUPREM3EXPORT) 
		    computesubstratetype( dop_data[2], 
			&(vert_info.substrate_dop_type ) );
		break;

  	case 2  :  /* ask for information */
		printf( "\tType of profile for Emitter? [a or s]  :   ");
		fflush( stdout );
		scanf( "%s", buf );
		if ( (buf[0] == 's') || (buf[0] == 'S') )
		    typeofprofile = 1;
		else 
		    typeofprofile = 0;
    		read_dop("Emitter", &(dop_data[0]), 
			vert_info.substrate_dop, typeofprofile,
			&(vert_info.emitter_depth) );
		printf( "\tType of profile for Base? [a or s]  :   ");
		fflush( stdout );
		scanf( "%s", buf );
		if ( (buf[0] == 's') || (buf[0] == 'S') )
		    typeofprofile = 1;
		else 
		    typeofprofile = 0;
    		read_dop("Internal Base", &(dop_data[1]), 
		    vert_info.substrate_dop, typeofprofile,
		    &(vert_info.int_base_depth) );
    		read_dop("External Base", &(dop_data[3]), 
		    vert_info.substrate_dop, typeofprofile,
		    &(vert_info.ext_base_depth) );
		if ( vert_info.int_base_depth <= vert_info.emitter_depth )
		     panic( "base depth less than emitter depth. try again\n");

		printf( "\tType of profile for Collector? [a or s]  :   ");
		fflush( stdout );
		scanf( "%s", buf );
		if ( (buf[0] == 's') || (buf[0] == 'S') )
		    typeofprofile = 1;
		else 
		    typeofprofile = 0;
    		read_dop("Collector", &(dop_data[2]), 
			vert_info.substrate_dop, typeofprofile,
			&(vert_info.collector_depth) );
		if ( ( vert_info.collector_depth <= vert_info.int_base_depth )
		  || ( vert_info.collector_depth <= vert_info.ext_base_depth ) )
		     panic("collector depth less than base depth. try again\n");
		if  (dop_data[2].type != SUPREM3EXPORT) 
		    computesubstratetype( dop_data[2], 
			&(vert_info.substrate_dop_type ) );
		break;
    } /* end switch */
}

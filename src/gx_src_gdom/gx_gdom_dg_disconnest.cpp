<<<<<<< HEAD
#include <assert.h>

#include "gx_gdom.h"

void gx::dg::disconnect()
{
	if(connected)  // prevent disconnect before bind()
	{
		// never can't be:
		assert(code);      // "code" is edge factory, can't be NULL
		assert(from);      // "from" in connected state are not NULL
		assert(node);      // "node" in connected state are not NULL

		if(is_set_edge){   // DISCONNECT SET-EDGE:
			node->calc.erase(this);
			code->set_lost_edges_count +=1;
		}
		else {		         // DISCONNECT GET-EDGE 
			node->deps.erase(this);
			code->get_lost_edges_count +=1;
		}
		
		// SET INITIAL STATE (DISCONNECTED)
		(*from) = NULL;    // prevent sending data after disconnect()
		node = NULL;       //
	  connected = false; //
	}
}
=======
#include <assert.h>

#include "gx_gdom.h"

void gx::dg::disconnect()
{
	if(connected)  // prevent disconnect before bind()
	{
		// never can't be:
		assert(code);      // "code" is edge factory, can't be NULL
		assert(from);      // "from" in connected state are not NULL
		assert(node);      // "node" in connected state are not NULL

		if(is_set_edge){   // DISCONNECT SET-EDGE:
			node->calc.erase(this);
			code->set_lost_edges_count +=1;
		}
		else {		         // DISCONNECT GET-EDGE 
			node->deps.erase(this);
			code->get_lost_edges_count +=1;
		}
		
		// SET INITIAL STATE (DISCONNECTED)
		(*from) = NULL;    // prevent sending data after disconnect()
		node = NULL;       //
	  connected = false; //
	}
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182

<<<<<<< HEAD
#include <iostream>

#include "gx_gdom.h"

bool gx::fn::fn_bind() // try to bind all dependences, skip all binded
{
	if( get_lost_edges_count + set_lost_edges_count )
	{
	  // foreach edge ( here we do not know is first try of connection or retry )
		get_lost_edges_count = 0;
		set_lost_edges_count = 0;

		edges_t::iterator curr = edges.begin(), none = edges.end();
		for(; curr != none ; ++curr)
		{
			gx::dg* edge = curr->second;
			
			if(edge->connected)
			{
				continue;
			}

			if(edge->bind()) // try to connect DG attribute
			{
				/*
				if(edge->is_set_edge)
				{
					if(edge->node->calc)
					{
						if(edge->node->calc != this) {  //!!TODO: Something like this possible are? 
							edge->node->calc->on_set_node_stolen(edge, this); // call old connected fn-instance
						}
					}
					edge->node->calc = this;   // this-fn now is output node calc
					edge->node->invalidate();  // Target node's calc changed or connected! emit DG invalidate
				}
				else
				{
					edge->node->deps.insert(edge); //register edge as getter SET & DEL events
				}
				*/
        continue;
			}

			if(edge->is_set_edge)  // edge->bind() => false
			{
				++set_lost_edges_count;
			}
			else
			{
        ++get_lost_edges_count;
			}
		}
	}
	return set_lost_edges_count ? false : get_lost_edges_count ? false : true;
}
=======
#include <iostream>

#include "gx_gdom.h"

bool gx::fn::fn_bind() // try to bind all dependences, skip all binded
{
	if( get_lost_edges_count + set_lost_edges_count )
	{
	  // foreach edge ( here we do not know is first try of connection or retry )
		get_lost_edges_count = 0;
		set_lost_edges_count = 0;

		edges_t::iterator curr = edges.begin(), none = edges.end();
		for(; curr != none ; ++curr)
		{
			gx::dg* edge = curr->second;
			
			if(edge->connected)
			{
				continue;
			}

			if(edge->bind()) // try to connect DG attribute
			{
				/*
				if(edge->is_set_edge)
				{
					if(edge->node->calc)
					{
						if(edge->node->calc != this) {  //!!TODO: Something like this possible are? 
							edge->node->calc->on_set_node_stolen(edge, this); // call old connected fn-instance
						}
					}
					edge->node->calc = this;   // this-fn now is output node calc
					edge->node->invalidate();  // Target node's calc changed or connected! emit DG invalidate
				}
				else
				{
					edge->node->deps.insert(edge); //register edge as getter SET & DEL events
				}
				*/
        continue;
			}

			if(edge->is_set_edge)  // edge->bind() => false
			{
				++set_lost_edges_count;
			}
			else
			{
        ++get_lost_edges_count;
			}
		}
	}
	return set_lost_edges_count ? false : get_lost_edges_count ? false : true;
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182

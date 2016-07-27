<<<<<<< HEAD
#include "gx_gdom.h"
#include <iostream>

// DECLINE: stuppid idea, in some cases always invalid - very good
//void gx::xx::paint_fail() 
//{
//	// some calc from set-calc add flags prevent "calc" of the deeply damaged
//	// trace 
//}

void gx::xx::invalidate(const unsigned short& flags)
{
	// NEED_CALC  1 0000 0000 0000 0001 main bit of the invalidation process
  // NEED_INIT  2 0000 0000 0000 0010 flag, need only for call init before calc
  // DEEP_FAIL  4 0000 0000 0000 0100 flag, need only for build traceback on error
  // SELF_FAIL  8 0000 0000 0000 1000 flag, functor has non-empty error log
	// successful calc cleanup all this bits to 0

  unsigned short _flags = flags;
	_flags |= NEED_CALC; // this bit must be 1 at invalidate process
  //                  // other bits are optional
	std::set<gx::dg*>::iterator curr = deps.begin(), none = deps.end();
	for(; curr != none ; ++curr)
	{
		gx::dg* edge = *curr;
		if(edge->code->invalid)
		{
			continue; //from this edge to up all calcs and nodes are invalid
		}
		edge->code->invalidate(_flags);
	}
}

void gx::fn::invalidate(const unsigned short& )
{
	if(invalid) return; //code=>code not work, data=>code=>data=>code work only

	edges_t::iterator curr = edges.begin(), none = edges.end();
	for(; curr != none ; ++curr)
	{
		if(curr->second->is_set_edge)
		{
			curr->second->node->invalidate();
		}
	}
}
=======
#include "gx_gdom.h"
#include <iostream>

// DECLINE: stuppid idea, in some cases always invalid - very good
//void gx::xx::paint_fail() 
//{
//	// some calc from set-calc add flags prevent "calc" of the deeply damaged
//	// trace 
//}

void gx::xx::invalidate(const unsigned short& flags)
{
	// NEED_CALC  1 0000 0000 0000 0001 main bit of the invalidation process
  // NEED_INIT  2 0000 0000 0000 0010 flag, need only for call init before calc
  // DEEP_FAIL  4 0000 0000 0000 0100 flag, need only for build traceback on error
  // SELF_FAIL  8 0000 0000 0000 1000 flag, functor has non-empty error log
	// successful calc cleanup all this bits to 0

  unsigned short _flags = flags;
	_flags |= NEED_CALC; // this bit must be 1 at invalidate process
  //                  // other bits are optional
	std::set<gx::dg*>::iterator curr = deps.begin(), none = deps.end();
	for(; curr != none ; ++curr)
	{
		gx::dg* edge = *curr;
		if(edge->code->invalid)
		{
			continue; //from this edge to up all calcs and nodes are invalid
		}
		edge->code->invalidate(_flags);
	}
}

void gx::fn::invalidate(const unsigned short& )
{
	if(invalid) return; //code=>code not work, data=>code=>data=>code work only

	edges_t::iterator curr = edges.begin(), none = edges.end();
	for(; curr != none ; ++curr)
	{
		if(curr->second->is_set_edge)
		{
			curr->second->node->invalidate();
		}
	}
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182

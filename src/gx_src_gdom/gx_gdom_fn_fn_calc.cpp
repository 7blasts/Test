<<<<<<< HEAD
#include <iostream>
#include <assert.h>

#include "gx_gdom.h"

// gx::xx::calc - set<gx::dg*> calc
// gx::xx::deps - set

// after fn calc can be called fn_fail, to extract traceback info about fail
// std::string gx::fn::fn_fail()

bool gx::fn::fn_calc() //try calc all dependences, skip all calculated
{
	edges_t::iterator curr = edges.begin(), none = edges.end();
	for(;curr!=none;++curr)
	{
		if(curr->second->is_set_edge)  // skip all output edges
			continue;

		gx::dg* edge = curr->second;   // input edge ptr

        std::cout << curr->first << "=>" <<( (edge->node->invalid)? "<-?>":"<ok>" )<<"\n";
		
		assert(edge->node);            // ASSERT! fn_bind()=>false ? or not called!
		assert(edge->connected);       // ASSERT! fn_bind()=>false ? or not called!

		if(edge->node->invalid)        // node with empty calc-set can't be invalid
		{
			// so, it is input edge with invalid-bit-on, and set of calculations.
			std::set<gx::dg*>::iterator calc = edge->node->calc.begin(), stop = edge->node->calc.end();
			for(; calc!=stop; ++calc)
			{
				gx::fn* f = (*calc)->code;
				if( f->invalid )           // only for calcs in invalid state
				{
					//if(f->invalid & DEEP_FAIL) // prevent multiple call failed sub-tree of the DG
					//{
					//	// who call as with this deffect? node with DEEP_FAIL disabled, so
				  //		// enable self own DEEP_FAIL bit in self invalid state
          //  invalid |= DEEP_FAIL;
          //  return false;            // can't continue with DEEP_FAIL, invalid state
					//}
					f->calc();  // virtual user defined void calc;
					if( f->invalid )
						return false;  // can't calculate value call gx::fn::fn_fail() to extract traceback
				}
			}
		}

		if(edge->node->invalid)
		{
			// Может ли GET в графе произойти без строительства в DG, т.е. внезапно и брутально
			// SET запишет данные в лист, при этом в листе с пустым "calc-set"-ом INVALID не случится
			// вызовется процедура инвалидации-покраски для всех узлов "deps-set" 
			// Должен просчитаться
          std::cout<<"  "<<"calc must be=>"<<(edge->node->invalid?"<-?>":"<ok>")<<"\n";


		}
		//
	}
	return false;
}
=======
#include <iostream>
#include <assert.h>

#include "gx_gdom.h"

// gx::xx::calc - set<gx::dg*> calc
// gx::xx::deps - set

// after fn calc can be called fn_fail, to extract traceback info about fail
// std::string gx::fn::fn_fail()

bool gx::fn::fn_calc() //try calc all dependences, skip all calculated
{
	edges_t::iterator curr = edges.begin(), none = edges.end();
	for(;curr!=none;++curr)
	{
		if(curr->second->is_set_edge)  // skip all output edges
			continue;

		gx::dg* edge = curr->second;   // input edge ptr

        std::cout << curr->first << "=>" <<( (edge->node->invalid)? "<-?>":"<ok>" )<<"\n";
		
		assert(edge->node);            // ASSERT! fn_bind()=>false ? or not called!
		assert(edge->connected);       // ASSERT! fn_bind()=>false ? or not called!

		if(edge->node->invalid)        // node with empty calc-set can't be invalid
		{
			// so, it is input edge with invalid-bit-on, and set of calculations.
			std::set<gx::dg*>::iterator calc = edge->node->calc.begin(), stop = edge->node->calc.end();
			for(; calc!=stop; ++calc)
			{
				gx::fn* f = (*calc)->code;
				if( f->invalid )           // only for calcs in invalid state
				{
					//if(f->invalid & DEEP_FAIL) // prevent multiple call failed sub-tree of the DG
					//{
					//	// who call as with this deffect? node with DEEP_FAIL disabled, so
				  //		// enable self own DEEP_FAIL bit in self invalid state
          //  invalid |= DEEP_FAIL;
          //  return false;            // can't continue with DEEP_FAIL, invalid state
					//}
					f->calc();  // virtual user defined void calc;
					if( f->invalid )
						return false;  // can't calculate value call gx::fn::fn_fail() to extract traceback
				}
			}
		}

		if(edge->node->invalid)
		{
			// Может ли GET в графе произойти без строительства в DG, т.е. внезапно и брутально
			// SET запишет данные в лист, при этом в листе с пустым "calc-set"-ом INVALID не случится
			// вызовется процедура инвалидации-покраски для всех узлов "deps-set" 
			// Должен просчитаться
          std::cout<<"  "<<"calc must be=>"<<(edge->node->invalid?"<-?>":"<ok>")<<"\n";


		}
		//
	}
	return false;
}
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182

<<<<<<< HEAD
#ifndef GX_GDOM_PRINT_FN_H
#define GX_GDOM_PRINT_FN_H

#include <ostream>
#include <string>

#include "gx_gdom.h"

namespace gx
{
	void fn_edge_info(std::ostream& out, const char* name, gx::dg* edge )
	{ //single line output
        out << name << ": ";
		out <<(edge->is_set_edge ? "s ":"g ");
		out	<< edge->link->src_type_name() << " ";
		out << edge->from << " ";
		out <<(edge->connected ? "* ":"? ");
		out << edge->node<<" ";
		out << "\"" << edge->name<<"\", ";		// out <<(edge->connected ? "=>":"??");
		out << "\"" << edge->path.repr() << "\"";
	}
	
	void fn_print_get_dict(std::ostream& out, gx::fn* f, const std::string& head="")
	{ // multiline
		out<<head<<"GET get_lost_edges_count =>" << f->get_lost_edges_count << "\n";
		gx::fn::edges_t::iterator curr = f->edges.begin(), none = f->edges.end();
		for(; curr != none; ++curr)
		{
			if(curr->second->is_set_edge)  // skip output edges
				continue;
			out<<head<<"  ";
			fn_edge_info(out, curr->first.c_str(),  curr->second);
			out<<"\n";
		}
	}

	void fn_print_set_dict(std::ostream& out, gx::fn* f, const std::string& head="")
	{ // multiline
		out<<head<<"SET set_lost_edges_count =>" << f->set_lost_edges_count << "\n";
		gx::fn::edges_t::iterator curr = f->edges.begin(), none = f->edges.end();
		for(; curr != none ; ++curr)
		{
			if(curr->second->is_set_edge)  // skip input edges
			{
				out<<head<<"  ";
				fn_edge_info(out, curr->first.c_str(),  curr->second);
				out<<"\n";
			}
		}
	}

	void fn_print_info(std::ostream& out, gx::fn *f, const std::string& tabs="")
	{
		out << tabs << "CALC gx::fn at <" <<f<<">\n";
		gx::fn_print_get_dict(out, f, tabs + "  ");
		gx::fn_print_set_dict(out, f, tabs + "  ");
	}

}

#endif  // GX_GDOM_PRINT_FN_H
=======
#ifndef GX_GDOM_PRINT_FN_H
#define GX_GDOM_PRINT_FN_H

#include <ostream>
#include <string>

#include "gx_gdom.h"

namespace gx
{
	void fn_edge_info(std::ostream& out, const char* name, gx::dg* edge )
	{ //single line output
        out << name << ": ";
		out <<(edge->is_set_edge ? "s ":"g ");
		out	<< edge->link->src_type_name() << " ";
		out << edge->from << " ";
		out <<(edge->connected ? "* ":"? ");
		out << edge->node<<" ";
		out << "\"" << edge->name<<"\", ";		// out <<(edge->connected ? "=>":"??");
		out << "\"" << edge->path.repr() << "\"";
	}
	
	void fn_print_get_dict(std::ostream& out, gx::fn* f, const std::string& head="")
	{ // multiline
		out<<head<<"GET get_lost_edges_count =>" << f->get_lost_edges_count << "\n";
		gx::fn::edges_t::iterator curr = f->edges.begin(), none = f->edges.end();
		for(; curr != none; ++curr)
		{
			if(curr->second->is_set_edge)  // skip output edges
				continue;
			out<<head<<"  ";
			fn_edge_info(out, curr->first.c_str(),  curr->second);
			out<<"\n";
		}
	}

	void fn_print_set_dict(std::ostream& out, gx::fn* f, const std::string& head="")
	{ // multiline
		out<<head<<"SET set_lost_edges_count =>" << f->set_lost_edges_count << "\n";
		gx::fn::edges_t::iterator curr = f->edges.begin(), none = f->edges.end();
		for(; curr != none ; ++curr)
		{
			if(curr->second->is_set_edge)  // skip input edges
			{
				out<<head<<"  ";
				fn_edge_info(out, curr->first.c_str(),  curr->second);
				out<<"\n";
			}
		}
	}

	void fn_print_info(std::ostream& out, gx::fn *f, const std::string& tabs="")
	{
		out << tabs << "CALC gx::fn at <" <<f<<">\n";
		gx::fn_print_get_dict(out, f, tabs + "  ");
		gx::fn_print_set_dict(out, f, tabs + "  ");
	}

}

#endif  // GX_GDOM_PRINT_FN_H
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182

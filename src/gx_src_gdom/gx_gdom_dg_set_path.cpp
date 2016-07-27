#include "gx_gdom.h"

gx::dg& gx::dg::set_path(const char *str)
{
	if(connected)
	{
		disconnect();
	}
	path.set_path(str);
	return (*this);
}

<<<<<<< HEAD
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
=======
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
>>>>>>> e80704d2f7f95a5beb6e1e6d387ed2eca1824182

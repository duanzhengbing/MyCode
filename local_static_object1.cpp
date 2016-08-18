#include "local_static_object.h"

A& Aa()
{
	static A a(1);
	return a;
}
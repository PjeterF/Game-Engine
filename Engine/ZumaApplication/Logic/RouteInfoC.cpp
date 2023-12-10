#include "RouteInfoC.hpp"

RouteInfoC::RouteInfoC(int tag, int targetSample) : ComponentBase(RouteInfo)
{
	this->tag = tag;
	this->targetSample = targetSample;
}

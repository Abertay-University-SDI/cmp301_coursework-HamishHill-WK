#include "myLight.h"

XMFLOAT3 myLight::getAtten()
{
	XMFLOAT3 temp = XMFLOAT3(atten.x, atten.y, atten.z);
	return temp;
}

int myLight::getType()
{
	return type;
}

void myLight::setAtten(float x, float y, float z)
{
	atten = XMFLOAT3(x, y, z);
}

void myLight::setType(int t)
{
	type = t;
}


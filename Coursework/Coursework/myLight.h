#pragma once
#include "Light.h"

class myLight : public Light{

private:

	XMFLOAT3 atten;
	int type = 0;
public:

	int getType();
	XMFLOAT3 getAtten();

	void setAtten(float x, float y, float z);
	void setType(int t);

};


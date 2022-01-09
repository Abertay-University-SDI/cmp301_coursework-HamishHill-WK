// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"

#include "verManipShader.h"
#include "texShader.h"
#include "LightShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"

#include "myLight.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

	//modifiable lighting values for imgui
	XMFLOAT3 pos;
	XMFLOAT3 direction;

	XMFLOAT4 skydiffuse;
	XMFLOAT4 pointdiffuse;

	XMFLOAT3 attenu;
	XMFLOAT4 ambi;

	XMFLOAT4 skyAmbi;


	bool renderSphere;

	void firstRender();
	void scndRender();

private:
	verManipShader* groundShader;
	texShader* textureShader;
	LightShader* lightShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;

	PlaneMesh* ground;
	AModel* model[4];

	SphereMesh* pointLightSphere;

	myLight* skylight;
	myLight* pointlight;
	myLight* spotlight;

	OrthoMesh* orthomesh;
	RenderTexture* renderTexture;

	ShadowMap* shadowMap;
};

#endif
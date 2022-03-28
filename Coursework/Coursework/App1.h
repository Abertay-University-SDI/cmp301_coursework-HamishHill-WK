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
#include "verEdgeShader.h"
#include "horEdgeShader.h"

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
	XMFLOAT3 spotPos;
	XMFLOAT3 skyPos;
	XMFLOAT3 direction;	
	XMFLOAT3 spotDirection;

	XMFLOAT4 skydiffuse;
	XMFLOAT4 pointdiffuse;	
	XMFLOAT4 spotDiffuse;

	XMFLOAT3 attenu;	
	XMFLOAT3 spotAttenu;
	XMFLOAT4 ambi;	
	XMFLOAT4 spotAmbi;
	XMFLOAT4 skyAmbi;

	XMFLOAT4 specularColour;
	float specularPower;
	float spotRange;
	float spotCone;

	bool renderSphere;
	bool edgeEnabled;
	bool showNorms;

	void depthRender();
	void firstRender();
	void scndRender();

	void verticalEdge();
	void horizontalEdge();

	//void depthPass();
	void finalPass();

private:
	verManipShader* groundShader;
	texShader* textureShader;
	LightShader* lightShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;
	verEdgeShader* verEdgeShader1;
	horEdgeShader* horEdgeShader1;

	PlaneMesh* ground;
	PlaneMesh* shadowGround;
	AModel* model[4];

	SphereMesh* pointLightSphere;
	SphereMesh* spotLightSphere;

	myLight* skylight;
	myLight* pointlight;
	myLight* spotlight;
	//myLight* shadowlight;

	OrthoMesh* orthomesh;
	RenderTexture* renderTexture;
	//RenderTexture* renderTexture1;
	//RenderTexture* renderTexture2;
	RenderTexture* horizEdgeTexture;
	RenderTexture* vertEdgeTexture;

	ShadowMap* shadowMap;
};

#endif
// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "verManipShader.h"
#include "texShader.h"
#include "LightShader.h"
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

	float pos[3];
	bool renderSphere;

	void firstRender();
	void scndRender();

private:
	verManipShader* groundShader;
	texShader* textureShader;
	LightShader* lightShader;

	PlaneMesh* ground;
	AModel* model;

	SphereMesh* pointLightSphere;

	myLight* skylight;
	myLight* pointlight;
	myLight* spotlight;

	OrthoMesh* orthomesh;
	RenderTexture* renderTexture;
};

#endif
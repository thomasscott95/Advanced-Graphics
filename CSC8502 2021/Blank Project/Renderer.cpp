#include "Renderer.h"

#include "../nclgl/Light.h"
#include "../nclgl/Heightmap.h"
#include "../nclgl/Shader.h"
#include "../nclgl/Camera.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"
#include "Terrain.h"
#include "Tree.h"
#include "Chest.h"
#include "Skel.h"
//#include <freeglut/include/GL/freeglut_std.h>


const  int  POST_PASSES = 10;


Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	quad = Mesh::GenerateQuad();

	heightMap = new HeightMap(TEXTUREDIR"newIsland.jpg");

	waterTex = SOIL_load_OGL_texture(
		TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	sandTex = SOIL_load_OGL_texture(
		TEXTUREDIR"Ground027_1K_Color.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	sandBump = SOIL_load_OGL_texture(
		TEXTUREDIR"Ground027_1K_NormalGL.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	grassTex = SOIL_load_OGL_texture(
		TEXTUREDIR"Ground003_2K_Color.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	grassBump = SOIL_load_OGL_texture(
		TEXTUREDIR"Ground003_2K_NormalGL.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	rocksTex = SOIL_load_OGL_texture(
		TEXTUREDIR"Ground003_2K_Color.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	rocksBump = SOIL_load_OGL_texture(
		TEXTUREDIR"Ground003_2K_NormalGL.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"px.png", TEXTUREDIR"nx.png",
		TEXTUREDIR"py.png", TEXTUREDIR"ny.png",
		TEXTUREDIR"pz.png", TEXTUREDIR"nz.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!sandTex || !sandBump || !grassTex || !grassBump || !cubeMap || !waterTex) {
		return;

	}
	SetTextureRepeating(sandTex, true);
	SetTextureRepeating(sandBump, true);
	SetTextureRepeating(grassTex, true);
	SetTextureRepeating(grassBump, true);
	SetTextureRepeating(rocksTex, true);
	SetTextureRepeating(rocksBump, true);
	SetTextureRepeating(waterTex, true);

	reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader = new Shader("TerrainVertex.glsl", "TerrainFragment.glsl");
	meshShader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
	animShader = new Shader("SkinningVertex.glsl", "TexturedFragment.glsl");
	


	tempMesh = Mesh::LoadFromMeshFile("tree-maple-low-poly-Anim.msh");
	anim = new MeshAnimation("tree-maple-low-poly-Anim.anm");
	material = new MeshMaterial("tree-maple-low-poly-Anim.mat");

	chestMesh = Mesh::LoadFromMeshFile("Wooden_Chest.msh");
	chestAnim = new MeshAnimation("Wooden_Chest.anm");
	chestMaterial = new MeshMaterial("Wooden_Chest.mat");

	silverMesh = Mesh::LoadFromMeshFile("Silver_Chest.msh");
	chestAnim = new MeshAnimation("Silver_Chest.anm");
	silverMaterial = new MeshMaterial("Silver_Chest.mat");

	goldMesh = Mesh::LoadFromMeshFile("Golden_Chest.msh");
	chestAnim = new MeshAnimation("Golden_Chest.anm");
	goldMaterial = new MeshMaterial("Golden_Chest.mat");

	deathMesh = Mesh::LoadFromMeshFile("Death_Chest.msh");
	chestAnim = new MeshAnimation("Death_Chest.anm");
	deathMaterial = new MeshMaterial("Death_Chest.mat");

	skelMesh = Mesh::LoadFromMeshFile("Skeleton@Skin.msh");
	skelAnim = new MeshAnimation("Skeleton@Skin.anm");
	skelMaterial = new MeshMaterial("Skeleton@Skin.mat");

	sceneShader = new  Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
	processShader = new  Shader("TexturedVertex.glsl", "processfrag.glsl");

	if (!reflectShader->LoadSuccess() ||
		!skyboxShader->LoadSuccess() ||
		!lightShader->LoadSuccess() ||
		!meshShader->LoadSuccess() ||
		!animShader->LoadSuccess() ||
		!processShader->LoadSuccess() ||
		!sceneShader->LoadSuccess()) {
		return;
	}

	Vector3 heightmapSize = heightMap->GetHeightmapSize();

	camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 1.0f, 0.5f));
	light = new Light(heightmapSize * Vector3(0.5f, 1.5f, 0.5f), Vector4(1, 1, 1, 1), heightmapSize.x);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	root = new SceneNode;
	root->AddChild(new Terrain(lightShader, sandTex, heightMap, sandBump,  grassTex, grassBump));
	root->AddChild(new Tree(animShader, tempMesh, anim, material, Vector3(5996, 81, 5125)));
	root->AddChild(new Tree(animShader, tempMesh, anim, material, Vector3(7290, 84, 5553)));
	root->AddChild(new Tree(animShader, tempMesh, anim, material, Vector3(4489, 80, 11201)));
	root->AddChild(new Tree(animShader, tempMesh, anim, material, Vector3(9839, 85, 10332)));
	root->AddChild(new Tree(animShader, tempMesh, anim, material, Vector3(8468, 97, 7678)));

	root->AddChild(new Chest(animShader, chestMesh, chestAnim, chestMaterial, Vector3(8644, 73, 8549)));
	root->AddChild(new Chest(animShader, chestMesh, chestAnim, chestMaterial, Vector3(10244, 715, 6066)));
	root->AddChild(new Chest(animShader, chestMesh, chestAnim, chestMaterial, Vector3(4683, 85, 11582)));
	root->AddChild(new Chest(animShader, chestMesh, chestAnim, chestMaterial, Vector3(8278, 148, 12041)));

	root->AddChild(new Skel(animShader, skelMesh, skelAnim, skelMaterial, Vector3(8644, 65, 8500)));


	Chest* chest = new Chest(animShader, chestMesh, chestAnim, chestMaterial, Vector3(8450, 73, 8549));
	chest->AddChild(new Skel(animShader, skelMesh, skelAnim, skelMaterial, Vector3(8450, 65, 8500)));
	root->AddChild(chest);
	


	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &processFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[0], 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	init = true;
}

Renderer ::~Renderer(void) {
	delete root;
	delete shader;
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete  sceneShader;
	delete  processShader;

	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
}



void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += dt * 1.0f; //2 degrees a second
	waterCycle += dt * 0.1f; //10 units a second
	root->Update(dt);

}

void   Renderer::BuildNodeLists(SceneNode* from) {
	if (true) {
		Vector3  dir = from->GetWorldTransform().GetPositionVector() -
			camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}

	for (vector <SceneNode*>::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void   Renderer::SortNodeLists() {
	std::sort(transparentNodeList.rbegin(),    
		transparentNodeList.rend(),     
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);
}

void   Renderer::DrawNodes() {
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	for (const auto& i : nodeList) {
		DrawNode(i);
	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);
	}
}

void   Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Shader* shader = n->getShader();
		if (shader) {
			BindShader(shader);
			SetShaderLight(*light);

			glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
			UpdateShaderMatrices();
			SetShaderLight(*light);
			n->Draw(*this);
		}
	}
}

void Renderer::RenderScene() {

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_B)) {
		blur = !blur;
	}
	if (blur == true) {
		projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		DrawSkybox();
		BuildNodeLists(root);
		SortNodeLists();

		DrawNodes();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		DrawPostProcess();
		PresentScene();
		ClearNodeLists();
		DrawWater();

	}
	else {
		projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		DrawSkybox();
		BuildNodeLists(root);
		SortNodeLists();
		DrawNodes();
		ClearNodeLists();
		DrawWater();

	}
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	UpdateShaderMatrices();

	quad->Draw();

	glDepthMask(GL_TRUE);

}

void   Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::DrawWater() {
	BindShader(reflectShader);

	glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);


	Vector3 hSize = heightMap->GetHeightmapSize();

	modelMatrix =
		Matrix4::Translation((hSize * 0.5f) - Vector3(0,600,0)) *
		Matrix4::Scale(hSize * 5.0f) *
		Matrix4::Rotation(90, Vector3(1, 0, 0));

	textureMatrix =
		Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
		Matrix4::Scale(Vector3(10, 10, 10)) *
		Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	UpdateShaderMatrices();
	quad->Draw();
}

void  Renderer::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(processShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(
		processShader->GetProgram(), "sceneTex"), 0);
	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(processShader->GetProgram(),
			"isVertical"), 0);

		glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
		quad->Draw();

		glUniform1i(glGetUniformLocation(processShader->GetProgram(),
			"isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[0], 0);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[1]);
		quad->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

void   Renderer::PresentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(sceneShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
	glUniform1i(glGetUniformLocation(
		sceneShader->GetProgram(), "diffuseTex"), 0);
	quad->Draw();
}


//void drawText(const char* text, int length, int x, int y) {
//	glMatrixMode(GL_PROJECTION);
//	double* matrix = new double[16];
//	glGetDoublev(GL_PROJECTION, matrix);
//	glLoadIdentity();
//	glOrtho(0, 500, 0, 600, -5, 5);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glPushMatrix();
//	glLoadIdentity();
//	glRasterPos2i(x,y);
//
//	for (int i = 0; i < length; i++) {
//		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
//	}
//	glPopMatrix;
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixd(matrix);
//	glMatrixMode(GL_MODELVIEW);
//
//
//};
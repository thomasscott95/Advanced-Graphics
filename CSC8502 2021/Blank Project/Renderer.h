#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"


class Camera;
class Shader;
class HeightMap;
class MeshAnimation;
class MeshMaterial;

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);
	void RenderScene() override;
	void UpdateScene(float dt) override;

protected:
	void DrawWater();
	void DrawSkybox();
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void DrawPostProcess();
	void PresentScene();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMap* heightMap;
	SceneNode* heightMapNode;
	Mesh* quad;

	Light* light;
	Camera* camera;

	GLuint texture;
	GLuint cubeMap;
	GLuint waterTex;
	GLuint sandTex;
	GLuint sandBump;
	GLuint grassTex;
	GLuint grassBump;
	GLuint rocksTex;
	GLuint rocksBump;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;



	//Frustum      frameFrustum;

	vector <SceneNode*> transparentNodeList;
	vector <SceneNode*> nodeList;

	SceneNode* root;

	Shader* meshShader;
	Shader* shader;
	Shader* animShader;
	Shader* sceneShader;
	Shader* processShader;
	


	MeshAnimation* anim;
	MeshAnimation* chestAnim;
	MeshAnimation* skelAnim;



	MeshMaterial* material;
	MeshMaterial* chestMaterial;
	MeshMaterial* deathMaterial;
	MeshMaterial* silverMaterial;
	MeshMaterial* goldMaterial;
	MeshMaterial* skelMaterial;

	vector <GLuint > matTextures;

	Mesh* mesh;
	Mesh* tempMesh;
	Mesh* chestMesh;
	Mesh* deathMesh;
	Mesh* goldMesh;
	Mesh* silverMesh;
	Mesh* skelMesh;


	float waterRotate;
	float waterCycle;
	bool blur = false;



};
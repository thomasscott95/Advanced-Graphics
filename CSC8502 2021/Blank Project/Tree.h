#include "../nclgl/OGLRenderer.h"
#include "SceneShaderNode.h"

class Mesh;
class MeshAnimation;
class MeshMaterial;

class Tree : public SceneShaderNode {
public:

	Tree(Shader* textureShader, Mesh* mesh, MeshAnimation* anim, MeshMaterial* material, Vector3 position);
	void Draw(const OGLRenderer& r);
	void Update(float dt);

protected:
	MeshAnimation* anim;
	MeshMaterial* treeTexture;
	Vector3 treePosition;
	vector<GLuint> matTextures;

	int currentFrame;
	float frameTime;
	//GLuint sandBump;
	//GLuint grassTex;
	//GLuint grassBump;

};
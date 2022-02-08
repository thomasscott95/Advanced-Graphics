#include "../nclgl/OGLRenderer.h"
#include "SceneShaderNode.h"

class Mesh;
class MeshAnimation;
class MeshMaterial;

class Skel : public SceneShaderNode {
public:

	Skel(Shader* textureShader, Mesh* mesh, MeshAnimation* anim, MeshMaterial* material, Vector3 position);
	void Draw(const OGLRenderer& r);
	void Update(float dt);

protected:
	MeshAnimation* anim;
	MeshMaterial* skelTexture;
	Vector3 skelPosition;

	vector<GLuint> matTextures;

	int currentFrame;
	float frameTime;
};
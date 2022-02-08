#include "../nclgl/OGLRenderer.h"
#include "SceneShaderNode.h"

class Mesh;
class MeshAnimation;
class MeshMaterial;

class Chest : public SceneShaderNode {
public:

	Chest(Shader* textureShader, Mesh* mesh, MeshAnimation* anim, MeshMaterial* material, Vector3 position);
	void Draw(const OGLRenderer& r);
	void Update(float dt);

protected:
	MeshAnimation* anim;
	MeshMaterial* chestTexture;
	Vector3 chestPosition;
	Vector3 chestRotation;

	vector<GLuint> matTextures;

	int currentFrame;
	float frameTime;
};
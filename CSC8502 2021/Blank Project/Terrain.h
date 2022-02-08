#include "../nclgl/OGLRenderer.h"
#include "SceneShaderNode.h"



class Camera;
class Shader;
class HeightMap;

class Terrain : public SceneShaderNode {
public:
	
	Terrain(Shader* textureShader, GLuint texture, Mesh* terrainMesh, GLuint sandBump, GLuint grassTex, GLuint grassBump);
	void Draw(const OGLRenderer& r);

protected:
	GLuint sandBump;
	GLuint grassTex;
	GLuint grassBump;

};
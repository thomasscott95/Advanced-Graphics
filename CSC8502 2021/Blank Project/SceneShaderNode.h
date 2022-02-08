#pragma once
#include"../nclgl/SceneNode.h"
#include "../nclgl/OGLRenderer.h"


class Camera;
class SceneShaderNode : public SceneNode {
public:

	SceneShaderNode(Shader*, Mesh*, GLuint = NULL);
	virtual void Draw(const OGLRenderer& r);
	
	Shader* getShader() {
		return shader;
	}
protected:
	void UpdateShaderMatrices();
	void LoadTexture();
};


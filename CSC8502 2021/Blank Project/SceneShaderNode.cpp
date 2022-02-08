#include "SceneShaderNode.h"

SceneShaderNode::SceneShaderNode(Shader* shader, Mesh* m, GLuint texture ) : SceneNode(m) {
	this->shader = shader;
	this->texture = texture;
}

void SceneShaderNode::Draw(const OGLRenderer& r) {
	if (mesh) {
		LoadTexture();

		modelMatrix.ToIdentity();
		textureMatrix.ToIdentity();
		UpdateShaderMatrices();
		mesh->Draw();
	}
}

void SceneShaderNode::UpdateShaderMatrices() {
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, transform.values);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "textureMatrix"), 1, false, textureMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "shadowMatrix"), 1, false, shadowMatrix.values);
}

void SceneShaderNode::LoadTexture() {
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}
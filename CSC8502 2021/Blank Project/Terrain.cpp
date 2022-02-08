#include "Terrain.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OGLRenderer.h"


Terrain::Terrain(Shader* textureShader,GLuint texture, Mesh* terrainMesh, GLuint sandBump, GLuint grassTex, GLuint grassBump) : SceneShaderNode(textureShader, terrainMesh, texture) {
	this->sandBump = sandBump;
	this->grassTex = grassTex;
	this->grassBump = grassBump;

	transform.ToIdentity();

}

void Terrain::Draw(const OGLRenderer&r){
	LoadTexture();

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "diffuseTex2"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grassTex);

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "bumpTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, sandBump);

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "bumpTex2"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grassBump);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	if (mesh) { mesh->Draw(); };
}

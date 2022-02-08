#include "Chest.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"




Chest::Chest(Shader* chestTextureShader, Mesh* m, MeshAnimation* chestAnim, MeshMaterial* chestMaterial, Vector3 chestPosition) : SceneShaderNode(chestTextureShader, m) {
	this->anim = chestAnim;
	this->chestPosition = chestPosition;
	this->chestTexture = chestMaterial;

	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const  MeshMaterialEntry* matEntry = chestMaterial->GetMaterialForLayer(i);

		const  string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);

		string  path = TEXTUREDIR + *filename;
		GLuint  texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}
	currentFrame = 0;
	frameTime = 0.0f;

	//transform.ToIdentity();

}

void Chest::Update(float dt) {
	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}
}

void Chest::Draw(const OGLRenderer& r) {

	const  Matrix4* invBindPose = mesh->GetInverseBindPose();
	const  Matrix4* frameData = anim->GetJointData(currentFrame);

	transform.ToIdentity();
	transform = transform * Matrix4::Translation(chestPosition);
	transform = transform * Matrix4::Scale(Vector3(50, 50, 50));

	UpdateShaderMatrices();
	vector<Matrix4> frameMatrices;

	for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}

	int j = glGetUniformLocation(shader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false,
		(float*)frameMatrices.data());
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		mesh->DrawSubMesh(i);
	}

	if (mesh) { mesh->Draw(); };


}
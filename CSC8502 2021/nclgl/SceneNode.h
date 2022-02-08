#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>


class SceneNode
{
public:
	float     GetBoundingRadius()  const { return  boundingRadius; }
	void      SetBoundingRadius(float f) { boundingRadius = f; }
	
	float     GetCameraDistance()  const { return  distanceFromCamera; }
	void      SetCameraDistance(float f) { distanceFromCamera = f; }
	
	void      SetTexture(GLuint  tex) { texture = tex; }
	GLuint    GetTexture()  const { return  texture; }
	
	static  bool  CompareByCameraDistance(SceneNode* a, SceneNode* b) {

		return (a->distanceFromCamera <
			b->distanceFromCamera) ? true : false;
	}

	SceneNode(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	void SetTransform(const Matrix4& matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	Matrix4		   GetWorldTransform() const { return worldTransform; }

	Vector4		   GetColour() const { return colour; }
	void		   SetColour(Vector4 c) { colour = c; }

	Vector3		   GetModelScale() const { return modelScale; }
	void		   SetModelScale(Vector3 ms) { modelScale = ms; }

	Mesh*		   GetMesh() const { return mesh; }
	void		   SetMesh(Mesh* m) { mesh = m; }

	void		   AddChild(SceneNode* ms);

	virtual void Update(float dt);
	virtual void Draw(const OGLRenderer& r);

	std::vector <SceneNode*>::const_iterator  GetChildIteratorStart() {
		return children.begin();
	}

	std::vector <SceneNode*>::const_iterator  GetChildIteratorEnd() {
		return children.end();
	}

	Shader* getShader() {
		return shader;
	}

protected:
	float     distanceFromCamera; 
	float     boundingRadius; 

	SceneNode* parent;
	Mesh* mesh;
	Matrix4 worldTransform;
	Matrix4 transform;
	Matrix4 projMatrix;		//Projection matrix
	Matrix4 modelMatrix;	//Model matrix. NOT MODELVIEW
	Matrix4 viewMatrix;		//View matrix
	Matrix4 textureMatrix;	//Texture matrix
	Matrix4 shadowMatrix;
	Vector3 modelScale;
	Vector4 colour;
	std::vector<SceneNode*> children;

	GLuint texture;
	Shader* shader;

};


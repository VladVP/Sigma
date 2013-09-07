#include "OpenGLSystem.h"
#include "GLSLShader.h"
#include "GLSixDOFView.h"
#include "../Components/GLSprite.h"
#include "../Components/GLIcoSphere.h"
#include "../Components/GLCubeSphere.h"
#include "../Components/GLMesh.h"

OpenGLSystem::OpenGLSystem() : windowWidth(800), windowHeight(600), deltaAccumulator(0.0) {
	this->view = new GLSixDOFView(); 
}

OpenGLSystem::~OpenGLSystem() {
	delete this->view;
	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			delete *vecitr;
		}
	}
}

IGLComponent* OpenGLSystem::Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) {
	if (type == "GLSprite") {
		GLSprite* spr = new GLSprite(entityID);
		spr->Initialize();
		if (entityID == 2) {
			spr->Transform().Translate(2,2,0);
		}
		this->components[entityID].push_back(spr);
		spr->Transform().Translate(0,0,0);
		return spr;
	} else if (type == "GLIcoSphere") {
		GLIcoSphere* sphere = new GLIcoSphere(entityID);
		sphere->Initialize();
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			}
		}
		sphere->Transform().Scale(scale,scale,scale);
		sphere->Transform().Translate(x,y,z);
		this->components[entityID].push_back(sphere);
		return sphere;
	} else if (type == "GLCubeSphere") {
		GLCubeSphere* sphere = new GLCubeSphere(entityID);

		std::string texture_name = "";
		std::string shader_name = "";
		int subdivision_levels = 1;
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			} else if (p->GetName() == "subdivision_levels") {
				subdivision_levels = p->Get<int>();
			} else if (p->GetName() == "texture_name") {
				texture_name = p->Get<std::string>();
			} else if (p->GetName() == "shader") {
				shader_name = p->Get<std::string>();
			}
		}

		sphere->SetSubdivisions(subdivision_levels);
		sphere->LoadShader(shader_name);
		sphere->Initialize();
		sphere->LoadTexture(texture_name);

		sphere->Transform().Scale(scale,scale,scale);
		sphere->Transform().Translate(x,y,z);

		this->components[entityID].push_back(sphere);
		return sphere;
	} else if (type=="GLMesh") {
		GLMesh* mesh = new GLMesh(entityID);
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &*propitr;
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			} else if (p->GetName() == "meshFile") {
				mesh->LoadMesh(p->Get<std::string>());
			}
		}
		mesh->Initialize();
		mesh->Transform().Scale(scale,scale,scale);
		mesh->Transform().Translate(x,y,z);
		this->components[entityID].push_back(mesh);
	}
	return nullptr;
}

bool OpenGLSystem::Update(const double delta) {
	this->deltaAccumulator += delta;
	this->view->UpdateViewMatrix();

	// Check if the deltaAccumulator is greater than 1/60 of a second.
	if (deltaAccumulator > 16.7) {
		// Set up the scene to a "clean" state.
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

		// Set the ViewProjection matrix to be used in the shader.
		// Loop through and draw each component.
		for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
			for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
				(*vecitr)->Update(&this->view->ViewMatrix[0][0], &this->ProjectionMatrix[0][0]);
			}
		}

		this->deltaAccumulator = 0.0;
		return true;
	}
	return false;
}

IGLComponent* OpenGLSystem::GetComponent(int entityID) {
	if (this->components.find(entityID) != this->components.end()) {
		return this->components[entityID][0];
	}
	return nullptr;
}

const int* OpenGLSystem::Start() {
	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	// Now that GL is up and running load the shaders
	GLSprite::LoadShader();
	GLIcoSphere::LoadShader();

	// Generates a floatly hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
	this->ProjectionMatrix = glm::perspective(
		45.0f,
		4.0f / 3.0f,
		1.0f,
		5000000.0f
		);

	this->view->Move(4.0f,3.0f,-10.f);

	// App specific global gl settings
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_MULTISAMPLE_ARB);

	return OpenGLVersion;
}

void OpenGLSystem::Move(float x, float y, float z) {
	this->view->Move(x,y,z);
}

void OpenGLSystem::Rotate(float x, float y, float z) {
	this->view->Rotate(x,y,z);
}
#include <iostream>

#include "systems/OpenGLSystem.h"
#include "systems/OpenALSystem.h"
#include "systems/BulletPhysics.h"
#include "systems/FactorySystem.h"
#include "controllers/GUIController.h"
#include "controllers/FPSCamera.h"
#include "components/PhysicsController.h"
#include "components/GLScreenQuad.h"
#include "SCParser.h"
#include "systems/WebGUISystem.h"
#include "OS.h"
#include "components/SpotLight.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argCount, char **argValues) {
<<<<<<< HEAD:src/tests/main.cpp
	Sigma::WebGUISystem webguisys;

	CefRefPtr<Sigma::WebGUISystem> app(&webguisys);
#ifdef _WIN32
	CefMainArgs mainArgs(GetModuleHandle(NULL));
	int exitCode = CefExecuteProcess(mainArgs, app.get(), nullptr);
#else
	CefMainArgs mainArgs(argCount, argValues);
	int exitCode = CefExecuteProcess(mainArgs, app.get());
#endif

	if (exitCode >= 0) {
		return exitCode;
	}
=======
	Log::Print::Init(); // Initiatin the Logger must the first thing

	Sigma::WebGUISystem webguisys;

#ifndef NO_CEF
	CefRefPtr<Sigma::WebGUISystem> app(&webguisys);
#ifdef _WIN32
	CefMainArgs mainArgs(GetModuleHandle(NULL));
#else
	CefMainArgs mainArgs(argCount, argValues);
#endif
#ifdef CEFDEV
	int exitCode = CefExecuteProcess(mainArgs, app.get(), nullptr);
#else
	int exitCode = CefExecuteProcess(mainArgs, app.get());
#endif
	if (exitCode >= 0) {
		return exitCode;
	}
#endif
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7:src/tests/main.cpp

	Sigma::OS glfwos;
	Sigma::OpenGLSystem glsys;
	Sigma::OpenALSystem alsys;
	Sigma::BulletPhysics bphys;

	Sigma::FactorySystem& factory = Sigma::FactorySystem::getInstance();
	factory.register_Factory(glsys);
	factory.register_Factory(alsys);
	factory.register_Factory(bphys);
#ifndef NO_CEF
	factory.register_Factory(webguisys);
#endif

	if (!glfwos.InitializeWindow(1024, 768, "Sigma test")) {
		LOG_ERROR << "Failed creating the window or context.";
		return -1;
	}

	/////////////////////////////
	// Start the openGL system //
	/////////////////////////////

	LOG << "Initializing OpenGL system.";
	const int* version = glsys.Start();

	glsys.SetViewportSize(glfwos.GetWindowWidth(), glfwos.GetWindowHeight());

	if (version[0] == -1) {
		LOG_ERROR << "Error starting OpenGL!";
		return -1;
	}
	else {
		LOG << "OpenGL version: " << version[0] << "." << version[1];
	}

	//////////////////////////////
	// Setup deferred rendering //
	//////////////////////////////

	// Create render target for the GBuffer, Light Accumulation buffer, and final composite buffer
	unsigned int geoBuffer = glsys.createRenderTarget(glfwos.GetWindowWidth(), glfwos.GetWindowHeight(), true);
	glsys.createRTBuffer(geoBuffer, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);	// Diffuse texture
	glsys.createRTBuffer(geoBuffer, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);	// Normal texture
	glsys.createRTBuffer(geoBuffer, GL_R32F, GL_RED, GL_FLOAT);				// Depth texture
	glsys.initRenderTarget(geoBuffer); // Create the opengl assets

	///////////////////
	// Setup physics //
	///////////////////

	bphys.Start();

<<<<<<< HEAD
	///////////////
	// Setup GUI //
	///////////////
<<<<<<< HEAD:src/tests/main.cpp

	webguisys.Start();
=======
	webguisys.Start(mainArgs);
>>>>>>> Web GUI now uses Chromium Embedded Framework.
=======
#ifndef NO_CEF
	webguisys.Start(mainArgs);
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7:src/tests/main.cpp
	webguisys.SetWindowSize(glfwos.GetWindowWidth(), glfwos.GetWindowHeight());
#endif

	/////////////////
	// Setup Sound //
	/////////////////

	LOG << "Initializing OpenAL system.";
	alsys.Start();
	alsys.test(); // try sound

	////////////////
	// Load scene //
	////////////////

	// Parse the scene file to retrieve entities
	Sigma::parser::SCParser parser;

	LOG << "Parsing test.sc scene file.";
	if (!parser.Parse("test.sc")) {
		LOG_ERROR << "Failed to load entities from file.";
		exit (-1);
	}

	LOG << "Generating Entities.";

	// Create each entity's components
	for (unsigned int i = 0; i < parser.EntityCount(); ++i) {
		Sigma::parser::Entity* e = parser.GetEntity(i);
		for (auto itr = e->components.begin(); itr != e->components.end(); ++itr) {

			// Currently, physicsmover components must come after gl* components
			if((*itr).type == "PhysicsMover") {
				Sigma::GLTransform *transform = glsys.GetTransformFor(e->id);
				if(transform) {
					Property p("transform", transform);
					itr->properties.push_back(p);
				}
				else {
					assert(0 && "Invalid entity id");
				}
			}

			factory.create(itr->type,e->id, const_cast<std::vector<Property>&>(itr->properties));
		}
	}

	//////////////////////
	// Setup user input //
	//////////////////////

	// View and ViewMover creation has been moved to test.sc, but for
	// now provide sensible defaults. Final engine should require
	// definition in scene file. Currently entity ID for view must be 1
	// for this to work.

	// No view provided, create a default FPS view
	if(!glsys.GetView()) {
		std::vector<Property> props;

		Property p_x("x", 0.0f);
		Property p_y("y", 0.0f);
		Property p_z("z", 0.0f);

		props.push_back(p_x);
		props.push_back(p_y);
		props.push_back(p_z);

		glsys.createGLView(1, props);
	}

	//Create the controller
	//Perhaps a little awkward currently, should create a generic
	//controller class ancestor
	bphys.initViewMover(*glsys.GetView()->Transform());

	Sigma::event::handler::FPSCamera theCamera(*bphys.getViewMover());
	glsys.GetView()->Transform()->SetEuler(true);
	glsys.GetView()->Transform()->SetMaxRotation(glm::vec3(45.0f,0,0));
	glfwos.RegisterKeyboardEventHandler(&theCamera);
	glfwos.RegisterMouseEventHandler(&theCamera);
	theCamera.os = &glfwos;

	// Sync bullet physics object with gl camera

	///////////////////
	// Configure GUI //
	///////////////////
#ifndef NO_CEF
	Sigma::event::handler::GUIController guicon;
	guicon.SetGUI(webguisys.getComponent(100, Sigma::WebGUIView::getStaticComponentTypeName()));
	glfwos.RegisterKeyboardEventHandler(&guicon);
	glfwos.RegisterMouseEventHandler(&guicon);
#endif

	// Call now to clear the delta after startup.
	glfwos.GetDeltaTime();

	Sigma::ALSound *als;
	bool soundflag = false, soundrunning = false;
	{
		als = (Sigma::ALSound *)alsys.getComponent(200, Sigma::ALSound::getStaticComponentTypeName());
		if(als) {
			als->Play(Sigma::PLAYBACK_LOOP);
		}
		als = (Sigma::ALSound *)alsys.getComponent(201, Sigma::ALSound::getStaticComponentTypeName());
		if(als) {
			als->Play(Sigma::PLAYBACK_LOOP);
		}
		als = (Sigma::ALSound *)alsys.getComponent(202, Sigma::ALSound::getStaticComponentTypeName());
		if(als) {
			als->Play(Sigma::PLAYBACK_LOOP);
		}

		// This one must be last
		als = (Sigma::ALSound *)alsys.getComponent(203, Sigma::ALSound::getStaticComponentTypeName());
	}

	enum FlashlightState {
		FL_ON,
		FL_TURNING_ON,
		FL_OFF,
		FL_TURNING_OFF
	};

	FlashlightState fs = FL_OFF;

	LOG << "Main loop begins ";
	while (!glfwos.Closing()) {
		// Get time in ms, store it in seconds too
		double deltaSec = glfwos.GetDeltaTime();

		if(!(glfwos.HasKeyboardFocusLock())) {
			// Process input
			if(glfwos.CheckKeyState(Sigma::event::KS_DOWN, GLFW_KEY_F)) {
				if(fs==FL_OFF) {
					fs=FL_TURNING_ON;
				} else if (fs==FL_ON) {
					fs=FL_TURNING_OFF;
				}
			}

			if(glfwos.CheckKeyState(Sigma::event::KS_UP, GLFW_KEY_F)) {
				if(fs==FL_TURNING_ON) {
					// Enable flashlight
					Sigma::SpotLight *spotlight = static_cast<Sigma::SpotLight *>(glsys.getComponent(151, Sigma::SpotLight::getStaticComponentTypeName()));
					spotlight->enabled = true;
					// Rotate flashlight up
					// Enable spotlight
					fs=FL_ON;
				} else if (fs==FL_TURNING_OFF) {
					// Disable spotlight
					Sigma::SpotLight *spotlight = static_cast<Sigma::SpotLight *>(glsys.getComponent(151, Sigma::SpotLight::getStaticComponentTypeName()));
					spotlight->enabled = false;
					// Rotate flashlight down
					// Disable flashlight
					fs=FL_OFF;
				}
			}
		}

		if(glfwos.CheckKeyState(Sigma::event::KS_DOWN, GLFW_KEY_W) ||
				glfwos.CheckKeyState(Sigma::event::KS_DOWN, GLFW_KEY_A) ||
				glfwos.CheckKeyState(Sigma::event::KS_DOWN, GLFW_KEY_D) ||
				glfwos.CheckKeyState(Sigma::event::KS_DOWN, GLFW_KEY_S)
			) {
			soundflag = true;
		}
		else if(glfwos.CheckKeyState(Sigma::event::KS_UP, GLFW_KEY_W) ||
				glfwos.CheckKeyState(Sigma::event::KS_UP, GLFW_KEY_A) ||
				glfwos.CheckKeyState(Sigma::event::KS_UP, GLFW_KEY_D) ||
				glfwos.CheckKeyState(Sigma::event::KS_UP, GLFW_KEY_S)
				) {
			soundflag = false;
		}

		if(als && (soundflag != soundrunning)) {
			if(soundflag && !(glfwos.HasKeyboardFocusLock())) {
				als->Play(Sigma::PLAYBACK_LOOP);
			} else {
				als->Stop();
			}
			soundrunning = soundflag;
		}

		///////////////////////
		// Update subsystems //
		///////////////////////

		// Pass in delta time in seconds
		bphys.Update(deltaSec);
		webguisys.Update(deltaSec);


		// Update the renderer and present
		if (glsys.Update(deltaSec)) {
			glfwos.SwapBuffers();

			alsys.Update();
			alsys.UpdateTransform(*glsys.GetView()->Transform());
		}

		glfwos.OSMessageLoop();
	}

<<<<<<< HEAD:src/tests/main.cpp
	CefShutdown();
=======
	// do a proper clean up
	alsys.Shutdown();
#ifndef NO_CEF
	CefShutdown();
#endif
	glfwos.Terminate();

>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7:src/tests/main.cpp
	return 0;
}

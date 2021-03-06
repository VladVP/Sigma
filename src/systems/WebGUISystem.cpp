#include "systems/WebGUISystem.h"
#include "Property.h"
#include "components/WebGUIComponent.h"
#include "systems/OpenGLSystem.h"

<<<<<<< HEAD
#include "cef_url.h"
=======
#ifndef NO_CEF
#include "cef_url.h"
#endif

#include "Sigma.h"
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7

namespace Sigma {

	// We need ctor and dstor to be exported to a dll even if they don't do anything
	// this avoids needing to export getFactoryFunctions() which is only used by Sigma
<<<<<<< HEAD
	WebGUISystem::WebGUISystem() : web_core(nullptr) { }
=======
	WebGUISystem::WebGUISystem() { }
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
	WebGUISystem::~WebGUISystem() { }

	std::map<std::string,Sigma::IFactory::FactoryFunction> WebGUISystem::getFactoryFunctions() {
		using namespace std::placeholders;
		std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
		retval["WebGUIView"] = std::bind(&WebGUISystem::createWebGUIView,this,_1,_2);

		return retval;
	}

<<<<<<< HEAD
	bool WebGUISystem::Start(CefMainArgs& mainArgs) {
		std::cout << "Setting up web view." << std::endl;
		CefRefPtr<WebGUISystem> ourselves(this);
		CefSettings settings;
#ifdef _WIN32
=======
#ifndef NO_CEF
	bool WebGUISystem::Start(CefMainArgs& mainArgs) {
		LOG << "Setting up web view.";
		CefRefPtr<WebGUISystem> ourselves(this);
		CefSettings settings;
#ifdef CEFDEV
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
		CefInitialize(mainArgs, settings, ourselves.get(), nullptr);
#else
		CefInitialize(mainArgs, settings, ourselves.get());
#endif
		return true;
	}
#endif
	bool WebGUISystem::Update(const double delta) {
<<<<<<< HEAD
<<<<<<< HEAD
		this->web_core->Update();
		for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
			for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
				if (citr->second->GetSurface()->is_dirty()) {
					// Update texture here. Not sure of the proper api for it
					// http://www.awesomium.com/docs/1_7_2/cpp_api/class_awesomium_1_1_bitmap_surface.html
					citr->second->UpdateTexture();
				}
			}
		}

=======
		CefDoMessageLoopWork();
>>>>>>> Web GUI now uses Chromium Embedded Framework.
=======
#ifndef NO_CEF
		CefDoMessageLoopWork();
#endif
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
		return true;
	}

	IComponent* WebGUISystem::createWebGUIView(const id_t entityID, const std::vector<Property> &properties) {
		float x, y, width, height;
		bool transparent = false;
		std::string textureName = "";
		std::string url;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);

			if (p->GetName() == "left") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "top") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "width") {
				width = p->Get<float>();
			}
			else if (p->GetName() == "height") {
				height = p->Get<float>();
			}
			else if (p->GetName() == "textureName") {
				textureName = p->Get<std::string>();
			}
			else if (p->GetName() == "transparent") {
				transparent = p->Get<bool>();
			}
			else if (p->GetName() == "URL") {
				url = p->Get<std::string>();
			}
		}
		WebGUIView* webview = new WebGUIView(entityID);
<<<<<<< HEAD
<<<<<<< HEAD
		WebView* view = web_core->CreateWebView(width * this->windowWidth, height * this->windowHeight);

		if (!url.IsValid()) {
			std::cerr << "Invalid URL" << std::endl;
		}
		view->LoadURL(url);
		view->SetTransparent(transparent);
=======
>>>>>>> Web GUI now uses Chromium Embedded Framework.

		CefString cefurl(url);
		CefURLParts parts;
		if (!CefParseURL(cefurl, parts)) {
			std::cerr << "Invalid URL" << std::endl;
		}

<<<<<<< HEAD
		if (Sigma::OpenGLSystem::textures.find(textureName) == Sigma::OpenGLSystem::textures.end()) {
			Sigma::resource::GLTexture texture;
			texture.Format(GL_BGRA);
			texture.LoadDataFromMemory(surface->buffer(), surface->width(), surface->height());
			Sigma::OpenGLSystem::textures[textureName] = texture;
		}
		else {
			Sigma::OpenGLSystem::textures[textureName].Format(GL_BGRA);
			Sigma::OpenGLSystem::textures[textureName].LoadDataFromMemory(surface->buffer(), surface->width(), surface->height());
		}
=======
		Sigma::resource::GLTexture texture;
		Sigma::OpenGLSystem::textures[textureName] = texture;
>>>>>>> Web GUI now uses Chromium Embedded Framework.
=======
#ifndef NO_CEF
		CefString cefurl(url);
		CefURLParts parts;
		if (!CefParseURL(cefurl, parts)) {
			LOG_WARN << "Invalid URL";
		}
#endif

		Sigma::resource::GLTexture texture;
		Sigma::OpenGLSystem::textures[textureName] = texture;
		Sigma::OpenGLSystem::textures[textureName].Format(GL_BGRA);
		Sigma::OpenGLSystem::textures[textureName].GenerateGLTexture(this->windowWidth, this->windowHeight);
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7

		webview->SetCaputeArea(x, y, width, height);
		webview->SetWindowSize(this->windowWidth, this->windowHeight);
		webview->SetTexture(&Sigma::OpenGLSystem::textures[textureName]);
		this->addComponent(entityID, webview);

<<<<<<< HEAD
=======
#ifndef NO_CEF
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
		CefWindowInfo windowInfo;
		windowInfo.SetAsOffScreen(nullptr);
		windowInfo.SetTransparentPainting(transparent);

		CefBrowserSettings settings;
		CefRefPtr<Sigma::WebGUIView> client(webview);
		CefBrowserHost::CreateBrowser(windowInfo, client.get(), url, settings, nullptr);
<<<<<<< HEAD
=======
#endif
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
		return webview;
	}
}

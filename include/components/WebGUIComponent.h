#pragma once

#include "../IComponent.h"
#include "systems/KeyboardInputSystem.h"
#include "systems/MouseInputSystem.h"
<<<<<<< HEAD
#include "cef_client.h"
=======
#ifndef NO_CEF
#include "cef_client.h"
#endif
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
#include "resources/GLTexture.h"
#include "Sigma.h"

namespace Sigma {
<<<<<<< HEAD
	class WebGUIView : public Sigma::IComponent, public CefClient, public CefLifeSpanHandler, public CefRenderHandler {
=======
	class WebGUIView : public Sigma::IComponent
#ifndef NO_CEF
		, public CefClient, public CefLifeSpanHandler, public CefRenderHandler
#endif
	{
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
	public:
		SET_COMPONENT_TYPENAME("WebGUIView");
		WebGUIView() : texture(nullptr), entity_id(0), mouseDown(0) { }
		WebGUIView(const id_t entityID) : texture(nullptr), entity_id(entityID), mouseDown(0) { };
<<<<<<< HEAD
		virtual ~WebGUIView() { this->browserHost->CloseBrowser(false); };
=======
		virtual ~WebGUIView() {
#ifndef NO_CEF
			this->browserHost->ParentWindowWillClose();
			this->browserHost->CloseBrowser(true);
#endif
		};
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7

		void SetTexture(Sigma::resource::GLTexture* texture) {
			this->texture = texture;
		}

		void SetCaputeArea(float x, float y, float width, float height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		void SetWindowSize(unsigned int width, unsigned int heigit) {
			this->windowWidth = width;
			this->windowHeight = heigit;
		}

		void InjectKeyboardEvent(const unsigned int key, const Sigma::event::KEY_STATE state);

		void InjectCharDown(const unsigned int c);

		bool InjectMouseMove(float x, float y);
		bool InjectMouseDown(const Sigma::event::BUTTON btn, float x, float y);
		bool InjectMouseUp(const Sigma::event::BUTTON btn, float x, float y);

<<<<<<< HEAD
=======
#ifndef NO_CEF
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
		// CefClient
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
			return this;
		}

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
			return this;
		}

		// CefLifeSpanHandler
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE {
			this->browserHost = browser->GetHost();
			this->browserHost->SetMouseCursorChangeDisabled(true);
		}

		// CefRenderHandler
		virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE {
			rect.x = this->x * this->windowWidth;
			rect.y = this->y * this->windowHeight;
			rect.width = this->width * this->windowWidth;
			rect.height = this->height * this->windowHeight;
			return true;
		}

		virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) {
			screenX = viewX;
			screenY = viewY;
			return true;
		}

		virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void *buffer, int width, int height) OVERRIDE {
			if (this->texture) {
				this->texture->LoadDataFromMemory((const unsigned char*) buffer, width, height);
			}
		}
<<<<<<< HEAD
	private:
		CefRefPtr<CefBrowserHost> browserHost;
=======
#endif
	private:
#ifndef NO_CEF
		CefRefPtr<CefBrowserHost> browserHost;
#endif
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
		Sigma::resource::GLTexture* texture;

		bool hasFocus;
		unsigned int mouseDown;

		float x, y, width, height; // The region in which to capture mouse inputs.
		unsigned int windowWidth; // The width of the window
		unsigned int windowHeight; // The height of the window

<<<<<<< HEAD
		const id_t entity_id;
<<<<<<< HEAD
=======
		IMPLEMENT_REFCOUNTING(WebGUIView);
>>>>>>> Web GUI now uses Chromium Embedded Framework.
=======

#ifndef NO_CEF
		IMPLEMENT_REFCOUNTING(WebGUIView);
#endif
>>>>>>> b27385e1b5d96c34ecc6ea1671f1fe756d99e0f7
	};
}

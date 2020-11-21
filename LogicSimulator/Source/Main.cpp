// Session 1: 3 hours, 14 minutes
// Session 2: 0 hours, 52 minutes
// Session 3: 4 hours, 23 minutes
// Session 4: 6 hours, 36 minutes
// Session 5: 6 hours, 20 minutes

#include "Window/Window.h"
#include "Rendering/Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"

#include "Rendering/Mesh/Primitives/Primitives.h"

#include <iostream>

int main() {
	try {
		Window window;
		window.SetSize(1280, 720);
		Renderer* renderer = Renderer::GetRenderer(RendererType::OPENGL, &window);
		renderer->SetWindowHints();
		if (!window.Create()) {
			throw std::exception("Failed to create GLFW window");
		}
		renderer->Init();

		Scene scene;
		OrthoCamera cam = OrthoCamera({ 5 });
		//cam.transform.position.x = 0.5f;
		scene.AttachEntity(&cam);
		scene.SetMainCamera(&cam);
		Material mat;
		mat.SetShader(Shader::GetShader("shader"));
		MeshedEntity line = MeshedEntity(const_cast<Mesh2D*>(&GetRect2DPrimitive()), &mat);
		scene.AttachEntity(&line);
		Scene::SetMainScene(&scene);

		while (!window.ShouldWindowClose()) {
			Window::PollEvents();
		}

		renderer->DeInit();
		window.Destroy();
	} catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
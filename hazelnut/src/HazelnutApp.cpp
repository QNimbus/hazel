#include <Hazel.h>
// -- Entry Point ---------
#include <hazel/core/Entrypoint.h>
// -- Entry Point ---------

#include "EditorLayer.h"

namespace Hazel {

	class Hazelnut : public Application {
	public:
		Hazelnut()
			: Application("Hazelnut")
		{
			PushLayer(new EditorLayer());
		}

		~Hazelnut() {
		}
	};

	Hazel::Application* Hazel::CreateApplication() {
		return new Hazelnut();
	}
}
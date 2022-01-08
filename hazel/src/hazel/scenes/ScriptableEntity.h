#pragma once

#include "hazel/scenes/Entity.h"

namespace Hazel {

	class ScriptableEntity {
	public:
		template<typename T>
		T& GetComponent() { return m_Entity.GetComponent<T>(); }

		template<typename T>
		bool HasComponent() { return m_Entity.HasComponent<T>(); }

	private:
		Entity m_Entity;
		friend class Scene;
	};

}

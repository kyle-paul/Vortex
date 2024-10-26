#pragma once

#include "Vortex/Scene/Entity.h"
#include "Vortex/Core/TimeStep.h"

namespace Vortex {

	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(TimeStep ts) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};

}
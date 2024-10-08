#pragma once

#include "Renderer/RenderInfo.h"

namespace TooGoodEngine {

	struct MaterialComponent
	{
		MaterialComponent() = default;
		~MaterialComponent() = default;

		MaterialID ID;
		Ref<Renderer> Renderer = nullptr; //renderer it is associated with.
	};

}
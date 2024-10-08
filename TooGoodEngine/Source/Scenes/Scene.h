#pragma once

#include "Events/Event.h"
#include "Assets/Asset.h"
#include "ECS/EntityTree.h"
#include "Renderer/Renderer.h"
#include "ECS/Components/Components.h"
#include "Math/PerspectiveCameraController.h"
#include "Math/OrthographicCameraController.h"

namespace TooGoodEngine {

	//(from the scene camera not a user camera)
	enum class SceneView
	{
		None = 0, View2D, View3D
	};

	//a collection of entities with their componenets.
	class Scene : public EventHandler 
	{
	public:
		Scene();
		~Scene() = default;

		void Play(double delta);
		void Update(double delta);

		virtual void OnEvent(Event* event) override;

		void SetSceneView(SceneView view);

		inline EntityTree& GetRegistry() { return m_Registry; }
		inline Ref<Renderer> GetSceneRenderer() { return m_SceneRenderer; }
		inline PerspectiveCameraController& GetSceneCameraController() { return m_CameraController; }
		inline OrthographicCameraController& GetSceneCameraController2D() { return m_CameraController2D; }

		inline const std::string& GetName() const { return m_SceneName; }
		inline void SetName(const std::string& name) { m_SceneName = name; }

		Entity Add(const std::string& name);

	private:
		EntityTree m_Registry;
		Ref<Renderer> m_SceneRenderer;
		std::string m_SceneName;
		
		Ref<PerspectiveCamera> m_SceneCamera;
		PerspectiveCameraController m_CameraController;

		Ref<OrthographicCamera> m_SceneCamera2D;
		OrthographicCameraController m_CameraController2D;

		ModelInfo m_TestModel;

		SceneView m_SceneView = SceneView::View3D;

		bool m_FirstPlay = true;
	};

}
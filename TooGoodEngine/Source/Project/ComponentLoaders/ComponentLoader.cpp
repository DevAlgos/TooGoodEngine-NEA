#include "ComponentLoader.h"

#include "Project/Project.h"


namespace TooGoodEngine {
		
	TransformComponent ComponentLoader::LoadTransform(const json& jsonComponent)
	{
		TransformComponent component{};

		auto& componentPosition = component.GetPosition();
		auto& componentRotation = component.GetRotation();
		auto& componentScale    = component.GetScale();

		std::vector<float> position = jsonComponent["Position"].get<std::vector<float>>();
		std::vector<float> scale = jsonComponent["Scale"].get<std::vector<float>>();
		std::vector<float> rotation = jsonComponent["Rotation"].get<std::vector<float>>();

		size_t i = 0;
		for (; i < position.size(); i++)
		{
			if (i >= 3) //too many
				break;

			componentPosition[i] = position[i];
		}

		i = 0;
		for (; i < scale.size(); i++)
		{
			if (i >= 3) //too many
				break;

			componentScale[i] = scale[i];
		}

		i = 0;
		for (; i < rotation.size(); i++)
		{
			if (i >= 3) //too many
				break;

			componentRotation[i] = rotation[i];
		}


		component.UpdateTransform();

		return component;
	}
	
	MeshComponent ComponentLoader::LoadMesh(const json& jsonComponent, Renderer& sceneRenderer)
	{
		MeshComponent component;

		//there may be more primitves in the future added here

		if (jsonComponent.get<std::string>() == "##Quad") //quad
		{
			component.ID = 0;
			component.PathToSource = "##Quad";
			return component;
		}
		else if (jsonComponent.get<std::string>() == "##Cube")
		{
			component.ID = 1;
			component.PathToSource = "##Cube";

			return component;
		}

		component.PathToSource = "";
		//TODO: load the source if there is one
		
		return component;
	}

	MaterialComponent ComponentLoader::LoadMaterial(const json& jsonComponent, Renderer& sceneRenderer)
	{
		MaterialInfo info{};

		//
		// ---- Ambient ----
		if (jsonComponent.contains("Ambient Image"))
		{
			std::filesystem::path path = jsonComponent["Ambient Image"].get<std::filesystem::path>();
			info.AmbientTexture = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
		}
		
		{
			std::array<float, 4> ambientCol = jsonComponent["Ambient"].get<std::array<float, 4>>();
			info.Ambient = { ambientCol[0], ambientCol[1], ambientCol[2], ambientCol[3] };
		}

		//
		// ---- Albedo ---- 
		if (jsonComponent.contains("Albedo Image"))
		{
			std::filesystem::path path = jsonComponent["Albedo Image"].get<std::filesystem::path>();
			info.AlbedoTexture = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
		}

		{
			std::array<float, 4> albedoCol = jsonComponent["Albedo"].get<std::array<float, 4>>();
			info.Albedo = { albedoCol[0], albedoCol[1], albedoCol[2], albedoCol[3] };
		}

		info.AlbedoFactor = jsonComponent["Albedo Factor"].get<float>();

		//
		// ---- Metallic ----
		if (jsonComponent.contains("Metallic Image"))
		{
			std::filesystem::path path = jsonComponent["Metallic Image"].get<std::filesystem::path>();
			info.MetallicTexture = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
		}

		{
			std::array<float, 4> metallicCol = jsonComponent["Metallic"].get<std::array<float, 4>>();
			info.Metallic = { metallicCol[0], metallicCol[1], metallicCol[2], metallicCol[3] };
		}

		info.MetallicFactor = jsonComponent["Metallic Factor"].get<float>();

		//
		// ---- Emission ----
		if (jsonComponent.contains("Emission Image"))
		{
			std::filesystem::path path = jsonComponent["Emission Image"].get<std::filesystem::path>();
			info.EmissionTexture = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
		}

		{
			std::array<float, 4> emissionCol = jsonComponent["Emission"].get<std::array<float, 4>>();
			info.Emission = { emissionCol[0], emissionCol[1], emissionCol[2], emissionCol[3] };
		}

		info.MetallicFactor = jsonComponent["Emission Factor"].get<float>();

		//
		// ---- Roughness ----
		if (jsonComponent.contains("Roughness Image"))
		{
			std::filesystem::path path = jsonComponent["Roughness Image"].get<std::filesystem::path>();
			info.RoughnessTexture = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
		}

		{
			info.Roughness = jsonComponent["Roughness"].get<float>();
		}

		MaterialComponent component{};
		component.Material = CreateMaterial(info);
		component.ID = sceneRenderer.AddMaterial(component.Material);

		return component;
	}

	PointLightComponent ComponentLoader::LoadPointLight(const json& jsonComponent)
	{
		PointLightComponent component{};
		
		std::array<float, 4> color = jsonComponent["Color"].get<std::array<float, 4>>();
		component.Color = { color[0], color[1], color[2], color[3] };

		std::array<float, 3> position = jsonComponent["Position"].get<std::array<float, 3>>();
		component.Position = { position[0], position[1], position[2] };

		component.Radius = jsonComponent["Radius"].get<float>();
		component.Intensity = jsonComponent["Intensity"].get<float>();

		return component;
	}

	DirectionalLightComponent ComponentLoader::LoadDirectionalLight(const json& jsonComponent)
	{
		DirectionalLightComponent component{};

		std::array<float, 4> color = jsonComponent["Color"].get<std::array<float, 4>>();
		component.Color = { color[0], color[1], color[2], color[3] };

		std::array<float, 3> direction = jsonComponent["Direction"].get<std::array<float, 3>>();
		component.Direction = { direction[0], direction[1], direction[2] };

		component.Intensity = jsonComponent["Intensity"].get<float>();

		return component;
	}

}
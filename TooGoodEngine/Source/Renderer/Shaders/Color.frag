#version 460

#extension GL_ARB_bindless_texture: require 

layout(location = 0) out vec4 OutColor;

const int MATERIAL_TYPE_IMAGE  = 0;
const int MATERIAL_TYPE_VECTOR = 1;

struct MaterialComponent 
{
	int   Type;
	vec4  Component;
	uvec2 BindlessSampler;
};

struct Material 
{
	MaterialComponent Albedo;
	MaterialComponent Metallic;
	MaterialComponent Emission;
	MaterialComponent Roughness;

	float AlbedoFactor;  
	float MetallicFactor;
	float EmissionFactor;
};

struct MaterialData
{
	vec4  Albedo;
	vec4  Metallic;
	vec4  Emission;
	float Roughness;
};

readonly layout(std430, location = 1) buffer u_MaterialBuffer 
{
	Material Data[];
} Materials;

MaterialData FetchMaterialData(in Material material, in vec2 textureCoordinate)
{
	MaterialData data;

	if(material.Albedo.Type == MATERIAL_TYPE_IMAGE)
		data.Albedo = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate) * material.AlbedoFactor;
	else 
		data.Albedo = material.Albedo.Component * material.AlbedoFactor;

	if(material.Metallic.Type == MATERIAL_TYPE_IMAGE)
		data.Metallic = texture(sampler2D(material.Metallic.BindlessSampler), textureCoordinate) * material.MetallicFactor;
	else   
		data.Metallic = material.Metallic.Component * material.MetallicFactor;

	if(material.Emission.Type == MATERIAL_TYPE_IMAGE)
		data.Emission = texture(sampler2D(material.Emission.BindlessSampler), textureCoordinate) * material.EmissionFactor;
	else 
		data.Emission = material.Emission.Component * material.EmissionFactor;

	if(material.Roughness.Type == MATERIAL_TYPE_IMAGE)
		data.Roughness = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate).r;
	else 
		data.Roughness = material.Roughness.Component.r;

	return data;
}

in vec3 oNormal;
in vec2 oTextureCoord;

void main()
{
	float a = dot(oNormal, oNormal);
    float b = dot(oTextureCoord, oTextureCoord);

    OutColor = vec4(1.0, 1.0, 1.0, 1.0) * a * b;
}

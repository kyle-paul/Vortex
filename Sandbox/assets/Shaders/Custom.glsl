#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform int u_EntityID;

out vec2 v_TexCoord;
out vec3 v_Normals;
flat out int v_EntityID;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Normals = a_Normals;
	v_EntityID = u_EntityID;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entity;

in vec2 v_TexCoord;
in vec3 v_Normals;
flat in int v_EntityID;

const vec3 sunDirection = normalize(vec3(-300.0, 300.0, 300.0)); 

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_TilingFactor;

void main()
{
	float lightStrength = max(0.5, dot(v_Normals, sunDirection));
	color = vec4(1.5 * lightStrength * (texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color).rgb, 1.0);
	entity = v_EntityID;
}
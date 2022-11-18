#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 viewProj;
// uniform mat3 normalMatrix;

void main()
{
    // projection * view * model * position
    gl_Position = viewProj * model * vec4(aPosition, 1.0f);
    // color = aColor;
    color = vec3(1.0, 1.0, 1.0);
    texCoord = aTexCoord;
}

// out vec3 o_position;
// out vec3 o_normal;
// out vec2 o_texCoord;	
// void main()
// {
// 	o_position = vec3(world * vec4(position, 1.0f));
//     o_normal   = normalMatrix * normal;
//     o_texCoord = texcoord.xy;
	
//     gl_Position = viewProj * world * vec4(position, 1.0f);
// }
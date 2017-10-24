#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.
layout (location = 1) in vec3 normal;
layout (location = 0) in vec3 position;
// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform struct Light{
	int light_mode;
	vec3 light_color;
	vec3 light_pos;
	vec3 light_dir;
	float att;
	float cutoff;
	float exponent;
};
uniform vec3 ambient;
uniform vec3 specular;
uniform vec3 diffuse;
uniform int mode;
// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out float sampleExtraOutput;
out vec3 normalvec;
out struct Light;
out vec3 ambientout;
out vec3 diffuseout;
out vec3 specularout;
flat out int modeout;
out vec3 vertex;
void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    sampleExtraOutput = 1.0f;
	normalvec= mat3(transpose(inverse(modelview)))*normal;
	ambientout=ambient;
	diffuseout=diffuse;
	specularout=specular;
	modeout=mode;
	vertex = vec3(model * vec4(position, 1.0f))
}

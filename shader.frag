#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normalvec;
in struct Light;
in vec3 ambientout;
in vec3 diffuseout;
in vec3 specularout;
in vec3 vertex;
flat in int modeout;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
	if(modeout==0){
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
		color = vec4(normalvec.x, normalvec.y, normalvec.z, sampleExtraOutput);
	}
	else
	{
		
		//color = vec4(normalvec.x, normalvec.y, normalvec.z, sampleExtraOutput);
	}
}

#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normalvecin;
uniform struct light{
	int light_mode;
	vec3 light_color;
	vec3 light_pos;
	vec3 light_dir;
	float att;
	float cutoff;
	float exponent;
} Light;
uniform vec3 ambient;
uniform vec3 specular;
uniform vec3 diffuse;
uniform int mode;
uniform float shiness;
in vec3 vertex;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
	vec3 normalvec = normalvecin;
	if(mode==0){
		color = vec4((normalvec.x+1)/2.0f, (normalvec.y+1)/2.0f, (normalvec.z+1)/2.0f, sampleExtraOutput);
	}
	else
	{
		vec3 L=vec3(0);
		vec3 cl=vec3(0);
		if(Light.light_mode==0)
		{
			L= vec3(Light.light_dir.x*(-1), Light.light_dir.y*(-1), Light.light_dir.z*(-1));
			cl=Light.light_color;
		}
		if(Light.light_mode==1)
		{
			L=(Light.light_pos-vertex)/length(Light.light_pos-vertex);
			cl=Light.light_color/(Light.att*(length(Light.light_pos-vertex)));
		}
		if(Light.light_mode==2)
		{
			L=(Light.light_pos-vertex)/length(Light.light_pos-vertex);
			if(dot(-L,Light.light_dir)<=cos(0.1f))
			{
				cl=vec3(0,0,0);
			}
			else
			{
				cl=Light.light_color*pow(dot(-L,Light.light_dir),Light.exponent)/(Light.att*pow(length(Light.light_pos-vertex),2));
			}
		}
		vec3 cd=vec3(0);
		cd=cl*diffuse*dot(normalvec,L);
		vec3 ca=vec3(0);
		ca=cl*ambient;
		vec3 cs=vec3(0);
		vec3 R=normalize(reflect(-L,normalvec));
		vec3 e=normalize(vec3(0.0f, 0.0f, 20.0f)-vertex);
		cs=cl*specular*pow(dot(R,e),shiness);
		vec3 res = vec3(ca.x+cd.x+cs.x,ca.y+cd.y+cs.y,ca.z+cd.z+cs.z);
		color=vec4(res,sampleExtraOutput);
	}
}

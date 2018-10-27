#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.005;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void GenerateLine(int index, vec4 offset)
{
    gl_Position = offset + gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = offset + gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
	vec3 faceNormal = GetNormal();
    GenerateLine(0, explode(gl_in[0].gl_Position, faceNormal)); // first vertex normal
    GenerateLine(1, explode(gl_in[1].gl_Position, faceNormal)); // second vertex normal
    GenerateLine(2, explode(gl_in[2].gl_Position, faceNormal)); // third vertex normal
}
#version 330 core
layout(location=0) in vec3 apos;
layout(location=1) in vec3 acolor;
layout(location=2) in vec2 aTexCoord;

out vec3 mycolor;
out vec2 TexCoord;

uniform mat4 transform;

void main(){
    gl_Position = transform * vec4(apos,1.0);
    mycolor=acolor;
    TexCoord=aTexCoord;
}
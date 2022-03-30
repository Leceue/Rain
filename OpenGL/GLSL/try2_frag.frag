#version 330 core
out vec4 FragColor;
in vec3 mycolor;

void main(){
    FragColor = vec4(mycolor, 1.0);   
}
#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec2 text;
layout(location=2) in vec3 norm;
uniform mat4 mvp;

void main(){
    gl_Position = mvp * pos;
}
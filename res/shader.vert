#version 120

attribute vec3 position;
attribute vec2 textureCoordinates;
attribute vec3 normals;

varying vec2 textureCoordinates0;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position, 1.0);
	textureCoordinates0 = textureCoordinates;
}
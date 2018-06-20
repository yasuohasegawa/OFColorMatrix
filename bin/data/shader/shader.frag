#version 120

uniform vec2 resolution;
uniform sampler2DRect texture;
uniform float time;
uniform float colorMats[20];

varying vec2 vTexCoord;

void main() {
    vec4 col = texture2DRect(texture, vTexCoord);
	vec4 res = vec4(0,0,0,1);

	res.r = (colorMats[0] * col.r) + (colorMats[1] * col.g) + (colorMats[2] * col.b) + (colorMats[3] * col.a) + colorMats[4];
	res.g = (colorMats[5] * col.r) + (colorMats[6] * col.g) + (colorMats[7] * col.b) + (colorMats[8] * col.a) + colorMats[9];
	res.b = (colorMats[10] * col.r) + (colorMats[11] * col.g) + (colorMats[12] * col.b) + (colorMats[13] * col.a) + colorMats[14];
	res.a = (colorMats[15] * col.r) + (colorMats[16] * col.g) + (colorMats[17] * col.b) + (colorMats[18] * col.a) + colorMats[19];
    gl_FragColor = res;
}
#version 420 core

#define MAX_STEPS 50
#define MAX_DIST 300
#define SURF_DIST 0.01
#define M_PI 3.1415926535897932384626433832795

in vec2 texCoord;
in vec3 normalVector;

out vec4 out_color;

uniform float viewportWidth;
uniform float viewportHeight;
uniform mat4 perspectiveMatrix;
uniform mat4 viewMatrix;
uniform vec3 cameraPos;
uniform vec3 cameraViewDirection;

uniform vec3 lightPos;

layout (binding = 0) uniform sampler1D fluidParticles;
layout (binding = 1) uniform sampler2D particleTexture;

uniform int fluidParticleCount;
uniform float blendCoeficient;

uniform float moveFactor;

uniform vec3 boxMin;
uniform vec3 boxMax;

float smoothMin(float distance1, float distance2, float k) {
	float h = max(k - abs(distance1 - distance2), 0) / k;
	return min(distance1, distance2) - h * h * h * k * 1 / 6.0;
}

float getDistance(vec3 p) {
	float minimumDist = MAX_DIST;
	for(int i = 0; i < fluidParticleCount; ++i) {
		vec3 particle = texelFetch(fluidParticles, i, 0).rgb;
		float particleDistance = length(p - particle) - 1.0;
		minimumDist = smoothMin(minimumDist, particleDistance, blendCoeficient);
	}
	
	float x, y, z;
	if(p.x < boxMin.x) x = boxMin.x;
	else if(p.x < boxMax.x) x = p.x;
	else x = boxMax.x;
	
	if(p.y < boxMin.y) y = boxMin.y;
	else if(p.y < boxMax.y) y = p.y;
	else y = boxMax.y;
	
	if(p.z < boxMin.z) z = boxMin.z;
	else if(p.z < boxMax.z) z = p.z;
	else z = boxMax.z;
	
	minimumDist = max(minimumDist, sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z)));
	
	return minimumDist;
}

float getDistanceBall(vec3 p) {
	float minimumDist = MAX_DIST;
	for(int i = 0; i < fluidParticleCount; ++i) {
		vec3 particle = texelFetch(fluidParticles, i, 0).rgb;
		float particleDistance = length(p - particle) - 1.0;
		minimumDist = smoothMin(minimumDist, particleDistance, blendCoeficient);
	}

	return minimumDist;
}

vec4 getDistanceAndCenter(vec3 p) {
	float minimumDist = MAX_DIST;
	vec3 minimumParticle = vec3(0, 0, 0);
	int notSet = 1;
	for(int i = 0; i < fluidParticleCount; ++i) {
		vec3 particle = texelFetch(fluidParticles, i, 0).rgb;
		float particleDistance = length(p - particle) - 1.0;
		if(particleDistance < minimumDist) {
			minimumParticle = particle;
		}
		minimumDist = smoothMin(minimumDist, particleDistance, blendCoeficient);
	}
	
	float x, y, z;
	if(p.x < boxMin.x) x = boxMin.x;
	else if(p.x < boxMax.x) x = p.x;
	else x = boxMax.x;
	
	if(p.y < boxMin.y) y = boxMin.y;
	else if(p.y < boxMax.y) y = p.y;
	else y = boxMax.y;
	
	if(p.z < boxMin.z) z = boxMin.z;
	else if(p.z < boxMax.z) z = p.z;
	else z = boxMax.z;
	
	minimumDist = max(minimumDist, sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z)));
	return vec4(minimumParticle.xyz, minimumDist);
}

vec4 rayMarch(vec3 rayOrigin, vec3 rayDirection) {
	float currentDistance = 0;
	vec3 particle = vec3(0, 0, 0);
	int i = 0;
	
	for(i = 0; i < MAX_STEPS; ++i) {
		vec3 p = rayOrigin + rayDirection * currentDistance;
		vec4 temp = getDistanceAndCenter(p);
		float distance = temp.w;
		currentDistance += distance;
		particle = temp.xyz;
		if(currentDistance > MAX_DIST || distance < SURF_DIST) break;
	}
	
	return vec4(particle, currentDistance);
}

vec3 getNormal(vec3 p) {
	vec3 n;
	float d = getDistanceBall(p);
	vec2 e = vec2(SURF_DIST, 0);
	
	if(p.x <= boxMin.x) n = vec3(-1, 0, 0);
	else if(p.x >= boxMax.x) n = vec3(1, 0, 0);
	else if(p.y <= boxMin.y) n = vec3(0, -1, 0);
	else if(p.y >= boxMax.y) n = vec3(0, 1, 0);
	else if(p.z <= boxMin.z) n = vec3(0, 0, -1);
	else if(p.z >= boxMax.z) n = vec3(0, 0, 1);
	else {
		n = d - vec3(getDistanceBall(p-e.xyy), getDistanceBall(p-e.yxy), getDistanceBall(p-e.yyx));
		n = normalize(n);
	}
	
	return n;
}

float getLight(vec3 p) {
	vec3 l = normalize(lightPos);
	vec3 n = normalize(getNormal(p));
	
	return clamp(dot(n, l), 0.2, 1.0);
}

void main(void){
	float aspectRatio = 16.0 / 9.0;
	float fov = tan(radians(70.0 / 2.0));
	
	vec2 uv = vec2((gl_FragCoord.x - 0.5) / (viewportWidth - 1.0), ((gl_FragCoord.y - 0.5) / (viewportHeight - 1.0)));
	uv.x = (uv.x * 2 - 1) * fov * aspectRatio;
	uv.y = (uv.y * 2 - 1) * fov;
	vec3 left = normalize(cross(vec3(0, 1, 0), cameraViewDirection));
	vec3 up = normalize(cross(cameraViewDirection, left));
	
	left *= uv.x;
	up *= uv.y;
	
	vec3 rayDirection = normalize(cameraViewDirection - left + up);
	vec3 rayOrigin = cameraPos;
	
	vec4 temp = rayMarch(rayOrigin, rayDirection);
	float distance = temp.w;
	vec3 sphereToIntersection = temp.xyz;
	vec3 p = rayOrigin + rayDirection * distance;
	distance /= MAX_DIST;
	if(distance >= 0.99) {
		out_color = vec4(0, 0, 0, 0);
	} else {
		float dif = getLight(p);
		vec3 nor = getNormal(p);
		float test = getDistance(p) * 100;
		vec3 particleToIntersection = normalize(p - sphereToIntersection);
		vec2 textureCoordinates;
		textureCoordinates.x = 0.5 + atan(particleToIntersection.z, particleToIntersection.x) / (2*M_PI);
		textureCoordinates.y = 0.5 - asin(particleToIntersection.y) / M_PI;
		//out_color = texture(particleTexture, vec2(textureCoordinates.x, textureCoordinates.y + moveFactor)) * vec4(dif, dif, dif, 0.6);
		out_color = vec4(0, 0.922, 0.98, 1.0) * vec4(dif, dif, dif, 0.6);
		//out_color = vec4(nor, 1.0);
		//out_color = vec4(test, test, test, 1.0);
	}
}
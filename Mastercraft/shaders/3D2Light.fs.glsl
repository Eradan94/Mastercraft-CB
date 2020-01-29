#version 330

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform sampler2D uTexture;
uniform vec3 uTextureKd;
uniform vec3 uTextureKs;
uniform float uTextureShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;
uniform vec3 uLightAmbient;

out vec3 fFragColor;

vec3 blinnPhong() {
    vec3 hv = (normalize(-vPosition_vs) +  normalize(uLightDir_vs)) / 2.;
    float dotHvNormal = dot(hv, vec3(normalize(vNormal_vs)));
    float dot = dot(normalize(uLightDir_vs), normalize(vNormal_vs));
    if(dot < 0.) {
        dot = 0.;
    }
    return vec3(uLightIntensity * (uTextureKd * dot) + uTextureKs * dotHvNormal);
}

/*vec3 blinnPhong() {
	vec3 hv = (normalize(-vPosition_vs) +  normalize(uLightDir_vs)) / 2.;
	float hvnpow = dot(hv, normalize(vNormal_vs));
	if (hvnpow > 0.0) {
		hvnpow = pow(hvnpow, uTextureShininess);
	}
	else {
		hvnpow = 0.0;
	}
	return uLightIntensity * (uTextureKd * (dot(normalize(uLightDir_vs), normalize(vNormal_vs))) + uTextureKs * hvnpow);
}*/



void main() {
	vec4 resTex = texture(uTexture, vTexCoords);
	vec3 ambiantColor = uLightAmbient * vec3(resTex);
	vec3 blinn = blinnPhong();
	fFragColor = vec3(resTex) * blinn + ambiantColor;
}

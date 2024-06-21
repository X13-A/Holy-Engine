#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform float metallic;
uniform float smoothness; // Change this to roughness internally
uniform vec3 ambientLight;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

const float PI = 3.14159265359;

// PBR functions
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{	
    // Prevent removal by optimizer
    vec2 uv = TexCoords;

    vec3 albedo = vec3(1, 0, 0) + uv.x;
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);
    float roughness = 1.0 - smoothness; // Smoothness to roughness

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	           
    // Calculate per-light radiance
    vec3 L = normalize(lightPos - WorldPos);
    vec3 H = normalize(V + L);
    vec3 radiance = lightColor; // Directional light    
    
    // Cook-Torrance BRDF
    float normalDistribution = DistributionGGX(N, H, roughness);
    float geometryFunction = GeometrySmith(N, V, L, roughness);
    vec3 fresnelTerm = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    vec3 specularReflectance = fresnelTerm;
    vec3 diffuseReflectance = vec3(1.0) - specularReflectance;
    diffuseReflectance *= 1.0 - metallic;
    
    vec3 numerator    = normalDistribution * geometryFunction * fresnelTerm;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  
        
    float normalDotLight = max(dot(N, L), 0.0);
    vec3 outgoingRadiance = (diffuseReflectance * albedo / PI + specular) * radiance * normalDotLight; 
  
    vec3 ambient = ambientLight * albedo;
    vec3 color = ambient + outgoingRadiance;

    // Gamma correction
    color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(color, 1.0);
}

float DistributionGGX(vec3 normal, vec3 halfway, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;
    float NdotH = max(dot(normal, halfway), 0.0);
    float NdotHSquared = NdotH * NdotH;

    float numerator = alphaSquared;
    float denominator = (NdotHSquared * (alphaSquared - 1.0) + 1.0);
    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

float GeometrySchlickGGX(float normalDotView, float roughness)
{
    float adjustedRoughness = (roughness + 1.0);
    float occlusionFactor = (adjustedRoughness * adjustedRoughness) / 8.0;

    float numerator = normalDotView ;
    float denominator = normalDotView  * (1.0 - occlusionFactor) + occlusionFactor;

    return numerator / denominator;
}


float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness)
{
    float normalDotView = max(dot(normal, viewDir), 0.0);
    float normalDotLight = max(dot(normal, lightDir), 0.0);
    float geometryView = GeometrySchlickGGX(normalDotView, roughness);
    float geometryLight = GeometrySchlickGGX(normalDotLight, roughness);

    return geometryView * geometryLight;
}

vec3 fresnelSchlick(float cosTheta, vec3 baseReflectivity)
{
    return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - cosTheta, 5.0);
}

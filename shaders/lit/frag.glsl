#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

// Texture parameters
uniform sampler2D albedoMap;

// Material parameters
uniform float metallicUniform;
uniform float roughnessUniform;
uniform vec3 ambientLight;

// PBR maps
uniform bool hasNormalMap;
uniform sampler2D normalMap;

uniform bool hasRoughnessMap;
uniform sampler2D roughnessMap;

uniform bool hasMetallicMap;
uniform sampler2D metallicMap;

// Lighting parameters
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 camPos;

// Shadow map
uniform sampler2D shadowMap;

const float PI = 3.14159265359;

// PBR functions
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

// Shadow map function
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{	
    vec2 uv = TexCoords;
    vec3 N = normalize(Normal);
    float metallic = metallicUniform;
    float roughness = roughnessUniform; 

    // Read maps if available
    if (hasNormalMap)
    {
        // TODO: unpack normal & convert them to world space
    }
    if (hasRoughnessMap)
    {
        roughness = texture(roughnessMap, uv).r;
    }
    if (hasMetallicMap)
    {
        metallic = texture(metallicMap, uv).r;
    }

    vec4 albedoRGBA = texture(albedoMap, uv);

    vec3 albedo = albedoRGBA.rgb;
    float alpha = albedoRGBA.a;
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	           
    // Calculate per-light radiance
    vec3 L = normalize(-lightDir);
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
  
    float shadow = ShadowCalculation(FragPosLightSpace, Normal, L);
    vec3 ambient = ambientLight * albedo;
    vec3 color = ambient + outgoingRadiance * (1.0 - shadow);

    // Gamma correction
    color = pow(color, vec3(1.0/2.2));  

    FragColor = vec4(color, alpha);
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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Check if the fragment is outside the shadow map
    if (projCoords.z > 1.0) return 0.0;

    // Bias based on angle between normal and light dir
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.001);
    float shadow = 0.0;
    
    // Soft shadowing
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    int samples = 4;
    float radius = 1.5;
    for (int x = -samples; x <= samples; ++x)
    {
        for (int y = -samples; y <= samples; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize * radius).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (2.0 * samples + 1.0) * (2.0 * samples + 1.0);
    return shadow;
}

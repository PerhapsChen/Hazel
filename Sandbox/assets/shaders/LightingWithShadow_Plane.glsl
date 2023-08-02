// LightingWithShadow

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.FragPos = vec3(u_Transform * vec4(a_Position, 1.0)); // 世界空间顶点坐标
    vs_out.Normal = transpose(inverse(mat3(u_Transform))) * a_Normal; // 变换到世界空间法线
    vs_out.TexCoords = a_TexCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0); // 光源空间顶点坐标

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool blinn;
uniform float gamma;
uniform float lightIntensity;
uniform float specPow;
uniform int shadowMethod;
uniform bool autoBias;
uniform float shadowBias;
uniform float filterScale;

// #define gamma 2.2
// #define specPow 32.

#define SHADOW_MAP_SIZE 4096.
#define FRUSTUM_SIZE 40.
#define PI 3.141592653589793
#define PI2 6.283185307179586
#define NUM_SAMPLES 50
#define NUM_RINGS 10
#define FILTER_SCALE filterScale

float rand_1to1(float x) //- 由种子生成[-1,1]的随机数
{
    return fract(sin(x)*10000.0);
}
float rand_2to1(vec2 uv) //- 由两个种子组成生成[0,1]的随机数
{
    float a = 12.9898, b= 78.233, c = 43758.5453;
    float dt = dot(uv.xy, vec2(a,b));
    float sn = mod(dt, PI);
    return fract(sin(sn) * c);
}
vec2 poissonDisk[NUM_SAMPLES];
void poissonDiskSample(vec2 randomSeed)
{
    float ANGLE_STEP = PI2 * float(NUM_RINGS) / float(NUM_SAMPLES);
    float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );
    float angle = rand_2to1( randomSeed ) * PI2;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;    
    for( int i = 0; i < NUM_SAMPLES; i ++ ) 
    {
        poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}

float findBlocker(vec2 uv, float currentDepth)
{
    poissonDiskSample(uv);
    float totalDepth = 0.0;
    int blockerCount = 0;
    for(int i=0; i<NUM_SAMPLES; i++)
    {
        vec2 simpleUV = uv+poissonDisk[i]/float(SHADOW_MAP_SIZE) * 50;
        float closestDepth = texture(shadowMap, simpleUV).r;
        if(currentDepth > closestDepth + 0.03){
            totalDepth += closestDepth;
            blockerCount++;
        }
    }
    if(blockerCount==0)
        return -1.0;
    if(blockerCount==NUM_SAMPLES)
        return 2.0;
    return totalDepth/float(blockerCount);
}

float Bias()
{
    float bias = 0.0;
    if (autoBias)
    {
        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        vec3 normal = normalize(fs_in.Normal); 
        // bias =  max(shadowBias*(1.0 - dot(normal, lightDir)), shadowBias);

        // Depth Bias
        // bias = FRUSTUM_SIZE/ SHADOW_MAP_SIZE * tan(acos(dot(normal, lightDir))) / 2.0f; 
        // Normal Offset Bias
        // bias += max(shadowBias*(1.0 - dot(normal, lightDir)), shadowBias);
        float A = (1.0f + FILTER_SCALE) * FRUSTUM_SIZE/SHADOW_MAP_SIZE * tan(acos(dot(normal, lightDir))) / 2.0f;
        // float B = 1 - dot(normal, lightDir);
        bias = shadowBias * A;

    }

    else
        bias = shadowBias;
    return bias;

    // vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    // vec3 normal = normalize(fs_in.Normal); 
    // float bias =  max(shadowBias*(1.0 - dot(normal, lightDir)), shadowBias);
    // return bias;
}

float useShadowMap(vec4 projCoords)
{
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = Bias();
    if(currentDepth-bias > closestDepth)
        return 0.0;
    else return 1.0; 
}

float PCF(vec4 projCoords)
{
    float bias = Bias();
    float visibility = 0.0;
    float currentDepth = projCoords.z;
    float filterSize = 1.0/SHADOW_MAP_SIZE * FILTER_SCALE;
    poissonDiskSample(projCoords.xy);
    
    for(int i=0; i<NUM_SAMPLES; i++){
        vec2 texCoords = poissonDisk[i] * filterSize + projCoords.xy;
        float closestDepth = texture(shadowMap,texCoords.xy).r;
        visibility += (closestDepth < currentDepth-bias)?0.0:1.0;
    }
    return visibility/float(NUM_SAMPLES);
}

float PCSS(vec4 projCoords)
{
    float bias = Bias();
    float visibility = 0.0;
    float currentDepth = projCoords.z;
    float blockerDepth = findBlocker(projCoords.xy, currentDepth);
    
    float penumbra = (currentDepth - blockerDepth) / blockerDepth * FILTER_SCALE;
    float filterSize = 1.0/SHADOW_MAP_SIZE * penumbra;

    for(int i=0; i<NUM_SAMPLES; i++){
        vec2 texCoords = poissonDisk[i] * filterSize + projCoords.xy;
        float closestDepth = texture(shadowMap,texCoords.xy).r;
        visibility += (closestDepth < currentDepth-bias)?0.0:1.0;
    }

    return visibility/float(NUM_SAMPLES);
}


void main()
{   
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    color = pow(color, vec3(gamma));
    vec3 normal = normalize(fs_in.Normal);
    // vec3 lightColor = vec3(0.9);
    // ambient
    vec3 ambient = 0.2 * color;//lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), specPow);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), specPow);
    }
    vec3 specular =  lightColor * spec * lightIntensity;    

    // calculate shadow
    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float visibility = 1.0;
    if (shadowMethod == 2)
        visibility = PCF(vec4(projCoords,1.0));   
    else if (shadowMethod == 3)
        visibility = PCSS(vec4(projCoords,1.0));   
    else if (shadowMethod == 1)
        visibility = useShadowMap(vec4(projCoords, 1.0));
    else
        visibility = 1.0f;

    vec3 lighting = (ambient + visibility * (diffuse + specular)) * color;    
    FragColor = vec4(lighting, 1.0);
    
}
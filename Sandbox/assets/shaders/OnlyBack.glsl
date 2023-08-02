// Base Depth

#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

uniform float outlineScale;

void main()
{
    vs_out.FragPos = vec3(u_Transform * vec4(a_Position, 1.0)); // 世界空间顶点坐标
    vs_out.Normal = transpose(inverse(mat3(u_Transform))) * a_Normal; // 变换到世界空间法线
    vs_out.TexCoords = a_TexCoord;
    
    vec4 pos_afterMV = u_View * u_Transform * vec4(a_Position, 1.0);
    vec3 normal_afterMV = mat3(u_View) * mat3(u_Transform) * a_Normal;
    normal_afterMV.z = -0.5,
    pos_afterMV = pos_afterMV + vec4(normalize(normal_afterMV), 0) * outlineScale;  //_Outline;


    gl_Position = u_Projection * pos_afterMV;
}

#type fragment
#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float gamma;
uniform vec3 lightColor;


void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    color = pow(color, vec3(gamma));
    vec3 normal = normalize(fs_in.Normal);
    // ambient
    vec3 ambient = vec3(0.2);
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 lighting = (ambient + diffuse) * color;    
    
    FragColor = vec4(lighting, 1.0);
}

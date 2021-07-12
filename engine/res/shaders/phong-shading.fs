#version 460 core

#extension GL_ARB_bindless_texture: enable

#define MAX_POINT_LIGHTS 32
#define MAX_SPOTLIGHTS 32

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 UV;
    vec3 Tangent;
} vs_in;

uniform vec3 CameraPosition;

uniform int PointLightsCount;
uniform int SpotlightsCount;

uniform vec4 Diffuse;
        
uniform vec3 Specular;
        
uniform vec3 Emissive;
        
uniform float Glossiness;

layout(bindless_sampler) uniform sampler2D DiffuseTexture;
layout(bindless_sampler) uniform sampler2D NormalTexture;

layout(bindless_sampler) uniform sampler2D ShadowMaps[MAX_SPOTLIGHTS];
layout(bindless_sampler) uniform sampler2DArray CubeShadowMaps[MAX_POINT_LIGHTS];

struct PointLight
{
    mat4 Cameras[6];

    vec4 Position;
    vec4 Color;

    float Stretch;
    float Offset;

    int ShadowMapId;
};

struct Spotlight
{
    mat4 Camera;

    vec4 Position;
    vec4 Direction;
    vec4 Color;

    float InnnerAngle;
    float OuterAngle;

    int ShadowMapId;
};

layout(std140) uniform LightBlock
{
    PointLight PointLightArray[MAX_POINT_LIGHTS];
    Spotlight SpotlightArray[MAX_SPOTLIGHTS];
} lightBlock;

layout(std140) uniform RenderCfgBlock
{
    int ShadowWidth;
    int ShadowHeight;

    float ShadowBias;
} renderCfgBlock;

const vec2 poissonSamples32[32] =  
{                                                  
    vec2( 0.03464862056371054f, 0.3830547450656243f ),
    vec2( 0.5145850377293442f, 0.9174753691550199f ),
    vec2( 0.014038707478449264f, 0.8703173018024027f ),
    vec2( 0.5467631734358679f, 0.43845822498510534f ),
    vec2( 0.7653101776997029f, 0.16594511523714506f ),
    vec2( 0.2722269318804359f, 0.6373376186211657f ),
    vec2( 0.2895716899246178f, 0.13999017215563025f ),
    vec2( 0.6866378129429584f, 0.6788742820569827f ),
    vec2( 0.012521686824723433f, 0.6452579975729188f ),
    vec2( 0.7816187372420162f, 0.9308681145108323f ),
    vec2( 0.9605805959046061f, 0.08231617046505457f ),
    vec2( 0.2510536624584889f, 0.8751905314585747f ),
    vec2( 0.3193968665820409f, 0.3789385794115725f ),
    vec2( 0.5502804464609835f, 0.16833420844530378f ),
    vec2( 0.789755485624428f, 0.4385978659076841f ),
    vec2( 0.4794300675620451f, 0.7107713974270773f ),
    vec2( 0.12894100325514302f, 0.08975006320093959f ),
    vec2( 0.8431759611120766f, 0.6221180749264188f ),
    vec2( 0.15133208598237768f, 0.25696745032557056f ),
    vec2( 0.4280098098427415f, 0.07217845991502514f ),
    vec2( 0.875305182550406f, 0.28200422587734186f ),
    vec2( 0.659297783178063f, 0.33578325226312045f ),
    vec2( 0.6414662991049945f, 0.8222917599323399f ),
    vec2( 0.4122772289403803f, 0.552781666338601f ),
    vec2( 0.15184103943589355f, 0.533451356590083f ),
    vec2( 0.6265146463512508f, 0.019477396035227135f ),
    vec2( 0.6649670206416741f, 0.5363368990273244f ),
    vec2( 0.45980285481232785f, 0.30992794359316334f ),
    vec2( 0.8822930726545528f, 0.7999528836238489f ),
    vec2( 0.11024772951507045f, 0.753706247601157f ),
    vec2( 0.936761988882629f, 0.48512037408239694f ),
    vec2( 0.18718322063669213f, 0.40138998518437186f ),
};

const vec2 poissonSamples16[16] = 
{
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ), 
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ),
    vec2( -0.91588581, 0.45771432 ), 
    vec2( -0.81544232, -0.87912464 ), 
    vec2( -0.38277543, 0.27676845 ), 
    vec2( 0.97484398, 0.75648379 ), 
    vec2( 0.44323325, -0.97511554 ), 
    vec2( 0.53742981, -0.47373420 ), 
    vec2( -0.26496911, -0.41893023 ), 
    vec2( 0.79197514, 0.19090188 ), 
    vec2( -0.24188840, 0.99706507 ), 
    vec2( -0.81409955, 0.91437590 ), 
    vec2( 0.19984126, 0.78641367 ), 
    vec2( 0.14383161, -0.14100790 ) 
};


float CalculateSpotlightShadow(in int id)
{
    vec4 lightSpaceFrag = lightBlock.SpotlightArray[id].Camera * vec4(vs_in.FragPos, 1.0f);

    vec3 proj = lightSpaceFrag.xyz / lightSpaceFrag.w;

    proj = proj * 0.5f + 0.5f;

    float fragDepth = proj.z;

    if(fragDepth > 1.0f)
        return 0.0f;


    float shadow = 0.0f;                                            

    for(int i = 0; i < 32; ++i)
    {
        float shadowDepth = texture(ShadowMaps[id], proj.xy + (poissonSamples32[i] / 250)).r;

        if((fragDepth - renderCfgBlock.ShadowBias) > shadowDepth)
            shadow += 1.0f;
    }

    shadow /= 32.0f;

    return shadow;
}

float CalculatePointlightShadow(in int id)
{
    float shadow = 0.0f;

    for(int i = 0; i < 6; ++i)
    {
        vec4 lightSpaceFrag = lightBlock.PointLightArray[id].Cameras[i] * vec4(vs_in.FragPos, 1.0f);

        vec3 proj = lightSpaceFrag.xyz / lightSpaceFrag.w;

        proj = proj * 0.5f + 0.5f;

        float fragDepth = proj.z;

        if(fragDepth > 1.0f)
           continue;

        for(int j = 0; j < 32; ++j)
        {
            float shadowDepth = texture(CubeShadowMaps[id],vec3(proj.xy + (poissonSamples32[j] / 250), i)).r;

            if((fragDepth - renderCfgBlock.ShadowBias) > shadowDepth)
                shadow += 1.0f;
        }
    }

    shadow /= 32.0f;

    return shadow;
}

vec3 CalculatePhong(in vec3 lightColor, in vec3 specular, in float gloss, in vec3 emissive, 
                    in vec3 lightDir, in vec3 normal, in vec3 viewDir, in float shadow)
{
    vec3 ambientComponent = lightColor * 0.1f;

    float diffC = max(dot(lightDir, normal), 0.0f);
    vec3 diffuseComponent  = lightColor * diffC;

    vec3 halfVector = normalize(viewDir + lightDir);
    float specC = pow(max(dot(halfVector, normal), 0.0f), gloss);
    vec3 specularComponent = lightColor * specC * specular;

    return (ambientComponent + (1.0f - shadow)) * (diffuseComponent + specularComponent + emissive);
}

void main()
{
    vec3 lightSum;

    vec3 normalTexture = 2.0f * (texture(NormalTexture, vs_in.UV).xyz - 0.5f);

    mat3 tbn = mat3(normalize(vs_in.Tangent),
                    normalize(cross(vs_in.Tangent, vs_in.Normal)),
                    normalize(vs_in.Normal));

    vec3 normal = vs_in.Normal;

    vec3 viewDir = normalize(CameraPosition - vs_in.FragPos);


    for(int i = 0; i < min(PointLightsCount, MAX_POINT_LIGHTS); ++i)
    {
        vec3 lightPos = lightBlock.PointLightArray[i].Position.xyz;
        vec3 lightDir = normalize(lightPos - vs_in.FragPos);
        vec3 lightColor = lightBlock.PointLightArray[i].Color.xyz;

        float stretch = lightBlock.PointLightArray[i].Stretch;
        float offset = lightBlock.PointLightArray[i].Offset;
        
        float shadow = CalculatePointlightShadow(i);

        vec3 phong = CalculatePhong(lightColor, Specular, Glossiness, Emissive, 
                                    lightDir, normal, viewDir, shadow);
        float lightDist = abs(length(lightPos - vs_in.FragPos));
        float attentuation = (1 / pow(offset + lightDist, 2)) * stretch;

        lightSum += phong * attentuation;
    }

    for(int i = 0; i < min(SpotlightsCount, MAX_SPOTLIGHTS); ++i)
    {
        vec3 lightPos = lightBlock.SpotlightArray[i].Position.xyz;
        vec3 spotDir = lightBlock.SpotlightArray[i].Direction.xyz;
        vec3 lightDir = normalize(lightPos - vs_in.FragPos);
        vec3 lightColor = lightBlock.SpotlightArray[i].Color.xyz;

        float innerAngle = lightBlock.SpotlightArray[i].InnnerAngle;
        float outerAngle = lightBlock.SpotlightArray[i].OuterAngle;

        float shadow = CalculateSpotlightShadow(i);

        vec3 phong = CalculatePhong(lightColor, Specular, Glossiness, Emissive, 
                                    lightDir, normal, viewDir, shadow);

        
        float currentA = outerAngle - acos(dot(spotDir, -lightDir));
        float diff = outerAngle - innerAngle;

        float attentuation = clamp(currentA / diff, 0.0f, 1.0f);
        lightSum += phong * attentuation;
    }

    FragColor = texture(DiffuseTexture, vs_in.UV) * vec4(lightSum, 1.0f);
}
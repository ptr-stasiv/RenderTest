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
    vec2(0.03464862056371054, 0.3830547450656243),
    vec2(0.5145850377293442, 0.9174753691550199),
    vec2(0.014038707478449264, 0.8703173018024027),
    vec2(0.5467631734358679, 0.43845822498510534),
    vec2(0.7653101776997029, 0.16594511523714506),
    vec2(0.2722269318804359, 0.6373376186211657),
    vec2(0.2895716899246178, 0.13999017215563025),
    vec2(0.6866378129429584, 0.6788742820569827),
    vec2(0.012521686824723433, 0.6452579975729188),
    vec2(0.7816187372420162, 0.9308681145108323),
    vec2(0.9605805959046061, 0.08231617046505457),
    vec2(0.2510536624584889, 0.8751905314585747),
    vec2(0.3193968665820409, 0.3789385794115725),
    vec2(0.5502804464609835, 0.16833420844530378),
    vec2(0.789755485624428, 0.4385978659076841),
    vec2(0.4794300675620451, 0.7107713974270773),
    vec2(0.12894100325514302, 0.08975006320093959),
    vec2(0.8431759611120766, 0.6221180749264188),
    vec2(0.15133208598237768, 0.25696745032557056),
    vec2(0.4280098098427415, 0.07217845991502514),
    vec2(0.875305182550406, 0.28200422587734186),
    vec2(0.659297783178063, 0.33578325226312045),
    vec2(0.6414662991049945, 0.8222917599323399),
    vec2(0.4122772289403803, 0.552781666338601),
    vec2(0.15184103943589355, 0.533451356590083),
    vec2(0.6265146463512508, 0.019477396035227135),
    vec2(0.6649670206416741, 0.5363368990273244),
    vec2(0.45980285481232785, 0.30992794359316334),
    vec2(0.8822930726545528, 0.7999528836238489),
    vec2(0.11024772951507045, 0.753706247601157),
    vec2(0.936761988882629, 0.48512037408239694),
    vec2(0.18718322063669213, 0.40138998518437186),
};

const vec2 poissonSamples64[64] = 
{
    vec2( 0.6823332673722882f, 0.7693616044902143f ),   
    vec2( 0.14020839003463936f, 0.2412819505527043f ),  
    vec2( 0.6450665264254224f, 0.28146005142352293f ),  
    vec2( 0.16581789780152623f, 0.7664830243290259f ),  
    vec2( 0.8817705223981847f, 0.5065669478378487f ),   
    vec2( 0.4464878094493284f, 0.997767784792779f ),    
    vec2( 0.886577064604794f, 0.05222737916694298f ),   
    vec2( 0.3913184797530187f, 0.5128642838500113f ),   
    vec2( 0.42371069277492535f, 0.764793544766694f ),   
    vec2( 0.1459829341211928f, 0.5229854860244226f ),   
    vec2( 0.20465062700198533f, 0.016569876657159477f ),
    vec2( 0.41724008940139923f, 0.24440311922780034f ), 
    vec2( 0.8675963237097074f, 0.27775463295890435f ),  
    vec2( 0.6331875521007994f, 0.5156230284006605f ),   
    vec2( 0.9285327220696543f, 0.7953601899702276f ),   
    vec2( 0.6939784202081056f, 0.999905034183962f ),    
    vec2( 0.7856621936517407f, 0.6423306725633416f ),   
    vec2( 0.25316731452673513f, 0.38399098804385656f ), 
    vec2( 0.9890703845748741f, 0.6431728980117875f ),   
    vec2( 0.007394794485270251f, 0.3832556752916382f ), 
    vec2( 0.5594419267592333f, 0.8910881453535987f ),   
    vec2( 0.05759988041029762f, 0.09829420964523639f ), 
    vec2( 0.5149553090828863f, 0.6317482830023845f ),   
    vec2( 0.2895173755347359f, 0.6504306084362619f ),   
    vec2( 0.2769491712708452f, 0.16774683610238628f ),  
    vec2( 0.3030138176728262f, 0.8834493304761336f ),   
    vec2( 0.5556858816039051f, 0.12222795493647898f ),  
    vec2( 0.9985454411112139f, 0.9536853046837467f ),   
    vec2( 0.7725891066749621f, 0.40072738603042013f ),  
    vec2( 0.8203496825021748f, 0.9084093946143528f ),   
    vec2( 0.7777664014000151f, 0.17360011721371116f ),  
    vec2( 0.5164550118406248f, 0.38290726554669396f ),  
    vec2( 0.005673933837322953f, 0.507811555122204f ),  
    vec2( 0.3910855828634112f, 0.36366255224493826f ),  
    vec2( 0.98332922014684f, 0.21877366451080205f ),    
    vec2( 0.7946570369904519f, 0.783656779149179f ),    
    vec2( 0.12716297524404863f, 0.9261348602211971f ),  
    vec2( 0.6274875914809608f, 0.6425736910425622f ),   
    vec2( 0.44379974995719906f, 0.13042828769409254f ), 
    vec2( 0.5611156294824691f, 0.7619625987353662f ),   
    vec2( 0.2851273752582246f, 0.7710802414251696f ),   
    vec2( 0.11814305039640616f, 0.6270961899430632f ),  
    vec2( 0.7444066215944408f, 0.534769969621964f ),    
    vec2( 0.3224582442955253f, 0.0021334769045254776f ),
    vec2( 0.6275568890136483f, 0.3988956594544456f ),   
    vec2( 0.06148812070164267f, 0.7395739206479349f ),  
    vec2( 0.8814639302080903f, 0.6864912977729014f ),   
    vec2( 0.6676006387509288f, 0.1721725586644154f ),   
    vec2( 0.580202681726419f, 0.009774520957600674f ),  
    vec2( 0.14156461107995566f, 0.37532152396163465f ), 
    vec2( 0.6644446917775119f, 0.8644431181540676f ),   
    vec2( 0.24797096701967103f, 0.5226437772494446f ),  
    vec2( 0.16695661718302557f, 0.14079502130037436f ), 
    vec2( 0.5296934603530387f, 0.25534717817682373f ),  
    vec2( 0.4053400095964541f, 0.8712099288400578f ),   
    vec2( 0.9024319359702099f, 0.15536018930829099f ),  
    vec2( 0.04637379839035649f, 0.8520112349709928f ),  
    vec2( 0.5109284341174384f, 0.5123768332120583f ),   
    vec2( 0.8935821499133476f, 0.3943086034599217f ),   
    vec2( 0.1984958222102169f, 0.6753096550567069f ),   
    vec2( 0.416123979566031f, 0.6174464722218164f ),    
    vec2( 0.31998389820127393f, 0.27007695448065694f ), 
    vec2( 0.8790507478786997f, 0.5978351490184093f ),   
    vec2( 0.33150024660370936f, 0.4442410467532786f )  
};

float rand(vec2 co)
{
    float a = 12.9898;
    float b = 78.233;
    float c = 43758.5453;
    float dt= dot(co.xy ,vec2(a,b));
    float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

#define FRUSTRUM_WIDTH 3.54f
#define LIGHT_SIZE 0.5f
#define LIGHT_SIZE_UV (LIGHT_SIZE / FRUSTRUM_WIDTH)

float SearchBlocker(in int id)
{
    vec4 lightSpaceFrag = lightBlock.SpotlightArray[id].Camera * vec4(vs_in.FragPos, 1.0f);
    vec3 fragPP = lightSpaceFrag.xyz / lightSpaceFrag.w;
    fragPP = fragPP * 0.5f + 0.5f;

    float kernelSize = LIGHT_SIZE_UV / fragPP.z;

    float avrgDepth = 0.0f;
    float blockersCount = 0.0f;

    for(int i = 0; i < 32; ++i)
    {
       vec2 noiseSample = poissonSamples32[i] * 2.0f - 1.0f;

       float sampleDepth = texture(ShadowMaps[id], fragPP.xy + noiseSample * kernelSize).r;

       if(fragPP.z - renderCfgBlock.ShadowBias > sampleDepth)
       {
          avrgDepth += sampleDepth;
          ++blockersCount;
       }
    }
   
    return blockersCount != 0.0f ? (avrgDepth / blockersCount) : 0.0f;
}

float CalculateSpotlightPCSS(in int id)
{
   float blocker = SearchBlocker(id);

   if(blocker == 0.0f)
      return 0.0f;

   
   vec4 lightSpaceFrag = lightBlock.SpotlightArray[id].Camera * vec4(vs_in.FragPos, 1.0f);
   vec3 fragPP = lightSpaceFrag.xyz / lightSpaceFrag.w;
   fragPP = fragPP * 0.5f + 0.5f;

   float penumbra = (fragPP.z - blocker) * LIGHT_SIZE_UV / blocker;
   penumbra *= 0.7f; //Multiply by arbitrary factor, reduce noise

   float randomJitter = rand(fragPP.xy) * 2.0f - 1.0f; //When use fragPP instead of uv its dramatically reduce moire pattern appearance
   float randomX = cos(randomJitter);
   float randomY = sin(randomJitter);

   float shadow = 0.0f;

   for(int i = 0; i < 64; ++i)
   {
      vec2 noiseSample = poissonSamples64[i] * 2.0f - 1.0f;
      noiseSample.x = noiseSample.x * randomX - noiseSample.y * randomY;
      noiseSample.y = noiseSample.y * randomX + noiseSample.x * randomY;

      float sampleDepth = texture(ShadowMaps[id], fragPP.xy + noiseSample * penumbra).r;

      if(fragPP.z - renderCfgBlock.ShadowBias > sampleDepth)
         shadow += sampleDepth;      
   }

   return shadow / 64.0f;
}

float CalculatePointlightPCSS(in int id)
{
    float blocker = SearchBlocker(id);

    if(blocker == 0.0f)
       return 0.0f;

    
    vec4 lightSpaceFrag = lightBlock.SpotlightArray[id].Camera * vec4(vs_in.FragPos, 1.0f);
    vec3 fragPP = lightSpaceFrag.xyz / lightSpaceFrag.w;
    fragPP = fragPP * 0.5f + 0.5f;

    float penumbra = (fragPP.z - blocker) * LIGHT_SIZE_UV / blocker;
    penumbra *= 0.7f; //Multiply by arbitrary factor, reduce noise

    float randomJitter = rand(fragPP.xy) * 2.0f - 1.0f; //When use fragPP instead of uv its dramatically reduce moire pattern appearance
    float randomX = cos(randomJitter);
    float randomY = sin(randomJitter);

    float shadow = 0.0f;
}

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

        float shadow = CalculateSpotlightPCSS(i);

        vec3 phong = CalculatePhong(lightColor, Specular, Glossiness, Emissive, 
                                    lightDir, normal, viewDir, shadow);

        
        float currentA = outerAngle - acos(dot(spotDir, -lightDir));
        float diff = outerAngle - innerAngle;

        float attentuation = clamp(currentA / diff, 0.0f, 1.0f);
        lightSum += phong * attentuation;
    }

    FragColor = texture(DiffuseTexture, vs_in.UV) * vec4(lightSum, 1.0f);
}

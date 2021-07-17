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

    float FrustrumWidth;

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

    float FrustrumWidth;

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
    float LightSize;
    int SoftShadows;
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
    vec2(0.6823332673722882, 0.769361604490214), 
    vec2(0.14020839003463936, 0.24128195055270), 
    vec2(0.6450665264254224, 0.281460051423522), 
    vec2(0.16581789780152623, 0.76648302432902), 
    vec2(0.8817705223981847, 0.506566947837848), 
    vec2(0.4464878094493284, 0.997767784792779), 
    vec2(0.886577064604794, 0.0522273791669429), 
    vec2(0.3913184797530187, 0.512864283850011), 
    vec2(0.42371069277492535, 0.76479354476669), 
    vec2(0.1459829341211928, 0.522985486024422), 
    vec2(0.20465062700198533, 0.01656987665715),
    vec2(0.41724008940139923, 0.24440311922780), 
    vec2(0.8675963237097074, 0.277754632958904), 
    vec2(0.6331875521007994, 0.515623028400660), 
    vec2(0.9285327220696543, 0.795360189970227), 
    vec2(0.6939784202081056, 0.999905034183962), 
    vec2(0.7856621936517407, 0.642330672563341), 
    vec2(0.25316731452673513, 0.38399098804385), 
    vec2(0.9890703845748741, 0.643172898011787), 
    vec2(0.007394794485270251, 0.3832556752916), 
    vec2(0.5594419267592333, 0.891088145353598), 
    vec2(0.05759988041029762, 0.09829420964523), 
    vec2(0.5149553090828863, 0.631748283002384), 
    vec2(0.2895173755347359, 0.650430608436261), 
    vec2(0.2769491712708452, 0.167746836102386), 
    vec2(0.3030138176728262, 0.883449330476133), 
    vec2(0.5556858816039051, 0.122227954936478), 
    vec2(0.9985454411112139, 0.953685304683746), 
    vec2(0.7725891066749621, 0.400727386030420), 
    vec2(0.8203496825021748, 0.908409394614352), 
    vec2(0.7777664014000151, 0.173600117213711), 
    vec2(0.5164550118406248, 0.382907265546693), 
    vec2(0.005673933837322953, 0.5078115551222), 
    vec2(0.3910855828634112, 0.363662552244938), 
    vec2(0.98332922014684, 0.21877366451080205), 
    vec2(0.7946570369904519, 0.783656779149179), 
    vec2(0.12716297524404863, 0.92613486022119), 
    vec2(0.6274875914809608, 0.642573691042562), 
    vec2(0.44379974995719906, 0.13042828769409), 
    vec2(0.5611156294824691, 0.761962598735366), 
    vec2(0.2851273752582246, 0.771080241425169), 
    vec2(0.11814305039640616, 0.62709618994306), 
    vec2(0.7444066215944408, 0.534769969621964), 
    vec2(0.3224582442955253, 0.002133476904525),
    vec2(0.6275568890136483, 0.398895659454445), 
    vec2(0.06148812070164267, 0.73957392064793), 
    vec2(0.8814639302080903, 0.686491297772901), 
    vec2(0.6676006387509288, 0.172172558664415), 
    vec2(0.580202681726419, 0.0097745209576006), 
    vec2(0.14156461107995566, 0.37532152396163), 
    vec2(0.6644446917775119, 0.864443118154067), 
    vec2(0.24797096701967103, 0.52264377724944), 
    vec2(0.16695661718302557, 0.14079502130037), 
    vec2(0.5296934603530387, 0.255347178176823), 
    vec2(0.4053400095964541, 0.871209928840057), 
    vec2(0.9024319359702099, 0.155360189308290), 
    vec2(0.04637379839035649, 0.85201123497099), 
    vec2(0.5109284341174384, 0.512376833212058), 
    vec2(0.8935821499133476, 0.394308603459921), 
    vec2(0.1984958222102169, 0.675309655056706), 
    vec2(0.416123979566031, 0.6174464722218164), 
    vec2(0.31998389820127393, 0.27007695448065), 
    vec2(0.8790507478786997, 0.597835149018409), 
    vec2(0.33150024660370936, 0.44424104675327)
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

float SearchBlockerSL(in int id)
{
    vec4 lightSpaceFrag = lightBlock.SpotlightArray[id].Camera * vec4(vs_in.FragPos, 1.0f);
    vec3 fragPP = lightSpaceFrag.xyz / lightSpaceFrag.w;
    fragPP = fragPP * 0.5f + 0.5f;

    if(fragPP.z >= 1.0f)
        return 0.0f;

    float lightSize = renderCfgBlock.LightSize / lightBlock.SpotlightArray[id].FrustrumWidth;

    float kernelSize = lightSize / fragPP.z;

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

float SearchBlockerPL(in int id, in int side)
{
    vec4 lightSpaceFrag = lightBlock.PointLightArray[id].Cameras[side] * vec4(vs_in.FragPos, 1.0f);
    vec3 fragPP = lightSpaceFrag.xyz / lightSpaceFrag.w;
    fragPP = fragPP * 0.5f + 0.5f;

    if(fragPP.z >= 1.0f)
        return 0.0f;

    float lightSize = renderCfgBlock.LightSize / lightBlock.PointLightArray[id].FrustrumWidth;

    float kernelSize = lightSize / fragPP.z;

    float avrgDepth = 0.0f;
    float blockersCount = 0.0f;

    for(int i = 0; i < 32; ++i)
    {
       vec2 noiseSample = poissonSamples32[i] * 2.0f - 1.0f;

       float sampleDepth = texture(CubeShadowMaps[id], vec3(fragPP.xy + noiseSample * kernelSize, side)).r;

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
   float blocker = SearchBlockerSL(id);

   if(blocker == 0.0f)
      return 0.0f;

   
   vec4 lightSpaceFrag = lightBlock.SpotlightArray[id].Camera * vec4(vs_in.FragPos, 1.0f);
   vec3 fragPP = lightSpaceFrag.xyz / lightSpaceFrag.w;
   fragPP = fragPP * 0.5f + 0.5f;


   float lightSize = renderCfgBlock.LightSize / lightBlock.SpotlightArray[id].FrustrumWidth;

   float penumbra = (fragPP.z - blocker) * lightSize / blocker;
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
    float shadow = 0.0f;

    for(int i = 0; i < 6; ++i)
    {
        float blocker = SearchBlockerPL(id, i);

        if(blocker == 0.0f)
            continue;


        vec4 lightSpaceFrag = lightBlock.PointLightArray[id].Cameras[i] * vec4(vs_in.FragPos, 1.0f);
        vec3 fragPP = lightSpaceFrag.xyz / lightSpaceFrag.w;
        fragPP = fragPP * 0.5f + 0.5f;


        float lightSize = renderCfgBlock.LightSize / lightBlock.PointLightArray[id].FrustrumWidth;

        float penumbra = (fragPP.z - blocker) * lightSize / blocker;
        penumbra *= 0.7f;

        float randomJitter = rand(fragPP.xy) * 2.0f - 1.0f;
        float randomX = cos(randomJitter);
        float randomY = sin(randomJitter);

        for(int j = 0; j < 64; ++j)
        {
           vec2 noiseSample = poissonSamples64[j] * 2.0f - 1.0f;
           noiseSample.x = noiseSample.x * randomX - noiseSample.y * randomY;
           noiseSample.y = noiseSample.y * randomX + noiseSample.x * randomY;

           float sampleDepth = texture(CubeShadowMaps[id], vec3(fragPP.xy + noiseSample * penumbra, i)).r;

           if(fragPP.z - renderCfgBlock.ShadowBias > sampleDepth)
              shadow += sampleDepth;      
        }
    }

    return shadow / 64.0f;
}

float CalculateSpotlightHardShadow(in int id)
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

float CalculatePointlightHardShadow(in int id)
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
        

        float shadow = 0.0f;

        if(renderCfgBlock.SoftShadows != 0)
            shadow = CalculatePointlightPCSS(i);
        else
            shadow = CalculatePointlightHardShadow(i);

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


        float shadow = 0.0f;

        if(renderCfgBlock.SoftShadows != 0)
            shadow = CalculateSpotlightPCSS(i);
        else
            shadow = CalculateSpotlightHardShadow(i);

        vec3 phong = CalculatePhong(lightColor, Specular, Glossiness, Emissive, 
                                    lightDir, normal, viewDir, shadow);

        
        float currentA = outerAngle - acos(dot(spotDir, -lightDir));
        float diff = outerAngle - innerAngle;

        float attentuation = clamp(currentA / diff, 0.0f, 1.0f);
        lightSum += phong * attentuation;
    }

    FragColor = texture(DiffuseTexture, vs_in.UV) * vec4(lightSum, 1.0f);
}

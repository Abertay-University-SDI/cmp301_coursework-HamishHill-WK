Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

Texture2D depthMapTexture : register(t1);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
    float3 direction;
    int norms;
    
    float3 position;
    float pad;
    
    float3 skypos;
    float pad1;
    
    float3 atten;
    float pad2;
    
    float4 ambient;
    float4 skyAmbient;
    float4 diffuse;
    float4 skyDiffuse;
};

cbuffer spotLightBuffer : register(b1)
{
    float3 spotDirection;
    float specPower;
    float4 specDiffuse;
    float3 spotPosition;
    float range;
    float4 spotAmbient;
    float4 spotDiffuse;
    float3 spotAtten;
    float cone;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float4 lightViewPos : TEXCOORD2;    
    float3 view : TEXCOORD3;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse1)
{
    float intensity1 = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse1 * intensity1);
    return colour;
}

// Is the gemoetry in our shadow map
bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

bool isInShadow(Texture2D sMap, float2 uv, float4 lightViewPosition, float bias)
{
    // Sample the shadow map (get depth of geometry)
    float depthValue = sMap.Sample(shadowSampler, uv).r;
	// Calculate the depth from the light.
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        return false;
    }
    return true;
}

float2 getProjectiveCoords(float4 lightViewPosition)
{
    // Calculate the projected texture coordinates.
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}

float4 calcSpecular(float3 lightD, float3 norm, float3 view1, float4 specCol, float specPowr)
{
    float3 halfway = normalize(lightD + view1);
    float specInt = pow(max(dot(norm, halfway), 0.0f), specPowr);
    return saturate(specCol * specInt);
}

float4 main(InputType input) : SV_TARGET
{
    if(norms == 1)
    {
        float4 colour = float4(input.normal.x, input.normal.y, input.normal.z, 1.0f);
        return colour;
    }
    
    float beta = cone / 3; //inner cone of spotlight
    float alpha; //angle between light vector and light direction 
    float intensity; //multiplier between 0.0 and 1.0 to show if pixel is in cone

    float4 textureColour;
    float shadowMapBias = 0.01f;

    textureColour = texture0.Sample(sampler0, (input.tex * 2));
    float2 pTexCoord = getProjectiveCoords(input.lightViewPos);
	
    float d; //array of distances from source to pixel -hh
    float attenMod; //"" attenuation modifiers -hh
    float3 lightVector; //"" normalized light vectors -hh
    float3 spotLightVector;
    
    float spotD;
	
    //d = length(position[1].xyz - input.worldPosition);
    d = length(position.xyz - input.worldPosition);
    spotD = length(spotPosition.xyz - input.worldPosition);
    attenMod = 1 / ((atten.x + (atten.y * d)) + (atten.z * (d * d)));
	
    //   lightVector = normalize(position[1].xyz - input.worldPosition);
    lightVector = normalize(position.xyz - input.worldPosition);    
    spotLightVector = normalize(spotPosition.xyz - input.worldPosition);
	
    float angleDot = dot(spotLightVector, normalize(spotDirection)); //dot product of vector to vertex and light direction vector 

    alpha = acos(angleDot / (length(spotLightVector) * length(spotDirection))); //find angle between 2 vectors
    intensity = (cos(alpha) - cos(cone / 2)) / (cos(beta / 2) - cos(cone / 2));

    if (intensity < 0.0f)
        intensity = 0.0f;
    
    float4 finalDif = float4(0.f, 0.f, 0.f, 1.f);
    
    //is the pixel in shadow
    if (hasDepthData(pTexCoord))
    {
        //if not in shadow, apply lighting
        if (!isInShadow(depthMapTexture, pTexCoord, input.lightViewPos, shadowMapBias))
        {
            finalDif = calculateLighting(-direction, input.normal, skyDiffuse);
        }              
    }
    
    finalDif = finalDif + (calculateLighting(lightVector, input.normal, diffuse) * attenMod) + ambient + skyAmbient + spotAmbient;
       
    if (spotD >= range)
    {
        return finalDif * textureColour;
    }
    else
    {
        attenMod = 1 / ((spotAtten.x + (spotAtten.y * spotD)) + (spotAtten.z * (spotD * spotD)));

        finalDif = finalDif + calcSpecular(spotLightVector, input.normal, input.view, specDiffuse, specPower) +
        (calculateLighting(spotLightVector, input.normal, spotDiffuse) * attenMod * intensity);
    
        return finalDif * textureColour;
    }
}


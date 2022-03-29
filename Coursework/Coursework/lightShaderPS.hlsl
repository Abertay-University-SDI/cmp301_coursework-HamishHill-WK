
// Calculate diffuse lighting for a single directional light (also texturing)
Texture2D depthTex : register(t1);
SamplerState shadowSamp : register(s1);

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

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
    float3 view : TEXCOORD2;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

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
    float depthValue = sMap.Sample(shadowSamp, uv).r;
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

float4 calcSpecular(float3 lightD, float3 norm, float3 view, float4 specCol, float specPowr)
{
    float3 halfway = normalize(lightD + view);
    float specInt = pow(max(dot(norm, halfway), 0.0f), specPowr);
    return saturate(specCol * specInt);
}


float4 main(InputType input) : SV_TARGET
{ 
    if (norms == 1)
    {
        float4 colour = float4(input.normal.x, input.normal.y, input.normal.z, 1.0f);
        return normalize(colour);
    }
        
    float4 textureColour;
    float4 lightColour;

    float beta = cone / 3; //inner cone of spotlight
    float alpha; //angle between light vector and light direction 
    float intensity; //multiplier between 0.0 and 1.0 to show if pixel is in cone
    
    textureColour = texture0.Sample(sampler0, (input.tex));
	
    float d; //array of distances from source to pixel -hh
    float attenMod; //"" attenuation modifiers -hh
    float3 lightVector; //"" normalized light vectors -hh
    float3 spotLightVector;
    float spotD;
    
    d = length(position.xyz - input.worldPosition);
    spotD = length(spotPosition.xyz - input.worldPosition);

    attenMod = 1 / ((atten.x + (atten.y * d)) + (atten.z * (d * d)));
	
    lightVector = normalize(position.xyz - input.worldPosition);
    spotLightVector = normalize(spotPosition.xyz - input.worldPosition);
	
    float angleDot = dot(spotLightVector, normalize(spotDirection)); //dot product of vector to vertex and light direction vector 

    alpha = acos(angleDot / (length(spotLightVector) * length(spotDirection))); //find angle between 2 vectors
    intensity = (cos(alpha) - cos(cone / 2)) / (cos(beta / 2) - cos(cone / 2));
    
    float4 finalDif;
	
            //calculate lighting for point light                                    //add directional light
    finalDif = (calculateLighting(-lightVector, input.normal, diffuse) * attenMod) + (calculateLighting(-direction, input.normal, skyDiffuse) );
	
    if (spotD >= range)
    {
        lightColour = ambient + skyAmbient + spotAmbient + finalDif;
        return saturate(lightColour * textureColour);
    }
    else
    {
    
        attenMod = 1 / ((spotAtten.x + (spotAtten.y * spotD)) + (spotAtten.z * (spotD * spotD)));

        finalDif = finalDif + calcSpecular(spotLightVector, input.normal, input.view, specDiffuse, specPower) +
        (calculateLighting(spotLightVector, input.normal, spotDiffuse) * attenMod * intensity);
    
    
        lightColour = ambient + skyAmbient + spotAmbient + finalDif;
    
        return saturate(lightColour * textureColour);
    }
}






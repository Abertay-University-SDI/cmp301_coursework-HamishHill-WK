
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float3 lightDirection;
    float pad;
    float4 ambient;
    float4 diffuseColour;
    float3 position;
    int type;
    float3 atten;
    float pad1;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;

};


float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour;

    switch (type)
    {
        case 0:
            textureColour = texture0.Sample(sampler0, (input.tex * 2));
            lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
            break;
        
        case 1:
            break;
        
        case 2:
            textureColour = texture0.Sample(sampler0, input.tex*2);
	
            float d; //array of distances from source to pixel -hh
            float attenMod; //"" attenuation modifiers -hh
            float3 lightVector; //"" normalized light vectors -hh
	
            d = length(position.xyz - input.worldPosition);

            attenMod = 1 / ((atten.x + (atten.y * d)) + (atten.z * (d * d)));
	
            lightVector = normalize(position.xyz - input.worldPosition);
	
            float4 finalDif;
	
            finalDif = (calculateLighting(lightVector, input.normal, diffuseColour) * attenMod) + (calculateLighting(lightVector, input.normal, diffuseColour) * attenMod);
	
            lightColour = ambient + finalDif;

            break;
        
        default:
            break;
        
    }
    //if(type == 0.0f)
    //{

    //    textureColour = texture0.Sample(sampler0, (input.tex * 2));
    //    lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);

  
    //}
    
 //   if(type == 2)
 //   {

	
    //   // return lightColour * textureColour;
   // }
   // normalize(input.normal);
    float4 colour = float4(input.normal.x, input.normal.y, input.normal.z, 0.0f);
    
    return colour;
    
  //  return lightColour * textureColour;
}


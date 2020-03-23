#version 400 core
out vec4 FragColor;

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;

    bool hasTexture;
}; 

struct DirLight { // DIRECTIONAL LIGHT
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 v_TexCoord;
in mat3 TBN;
 
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dLight;

uniform sampler2D u_Texture_diffuse;
uniform sampler2D u_Texture_specular;
uniform sampler2D u_Texture_normal;

// local variables
vec3 ka, kd, ks, norm, viewDir;
mat3 TBN_matrix;

// functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{

    // TEXTURE
    // -------
    if (material.hasTexture)
    {
	// LIGHTING MAPS
	// -------------
        ka = vec3(texture(u_Texture_diffuse, v_TexCoord)) * 0.5;
        kd = vec3(texture(u_Texture_diffuse, v_TexCoord));
        ks = vec3(texture(u_Texture_specular, v_TexCoord)) * 0.5;
        norm = texture(u_Texture_normal, v_TexCoord).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        //norm = normalize(TBN * norm);
        // --------------
        TBN_matrix = TBN;
    }
    else
    {
        // COLOR
        // -----
        ka = material.ka;
        kd = material.kd;
        ks = material.ks;
        norm = normalize(Normal);
        viewDir = normalize(viewPos - FragPos);
        // ---------------------
        TBN_matrix = mat3(1.0f);
    }
    viewDir  = TBN_matrix * normalize(viewPos - FragPos);  

    // RESULT
    // ------
    vec3 result = CalcDirLight(dLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
} 

// calculates the color when using a directional light
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = TBN_matrix * normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient * ka;
    vec3 diffuse  = light.diffuse * diff * kd;
    vec3 specular = light.specular * spec * ks;
    return (ambient + diffuse + specular);
}  

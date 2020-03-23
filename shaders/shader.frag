#version 400 core
out vec4 FragColor;

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
}; 

struct DirLight { // DIRECTIONAL LIGHT
    vec3 direction;
    vec3 intensity;
};

struct PointLight { // POINT LIGHT
    vec3 position;
    vec3 intensity;
    //vec3 ambient;
    //vec3 diffuse;
    //vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
 
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dLight;
uniform PointLight pLight;

void main()
{
    // DIRECTIONAL LIGHT
    // -----------------

    // ambient
    vec3 ambient = dLight.intensity * material.ka;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dLight.intensity * (diff * material.kd);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dLight.intensity * (spec * material.ks);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);


    // POINT LIGHT
    // -----------
    /*
    // ambient
    vec3 ambient = pLight.intensity * material.ka;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pLight.intensity * (diff * material.kd);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pLight.intensity * (spec * material.ks);  
        
    // attenuation
    float distance = length(pLight.position - FragPos);
    float attenuation = 1.0 / (pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
    */
} 


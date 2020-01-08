#version 400 core
out vec4 FragColor;

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
}; 

struct Light { // POINT LIGHT
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
uniform Light light;

void main()
{
    // ambient
    //vec3 ambient = light.ambient * material.ka;
    vec3 ambient = light.intensity * material.ka;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = light.diffuse * (diff * material.kd);
    vec3 diffuse = light.intensity * (diff * material.kd);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * (spec * material.ks);  
    vec3 specular = light.intensity * (spec * material.ks);  
        
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 


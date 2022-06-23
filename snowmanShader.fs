#version 330 core
out vec4 FragColor;

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;   
    vec4 specular;
}; 


uniform Material material;
uniform Light light;


uniform vec3 viewPos;
in vec3 FragPos;  
in vec3 Normal; 

void main()
{    
    
    // ambient
    vec3 ambient = light.ambient * material.ambient.rbg;     	
    // diffuse 
    vec3 norm = normalize(Normal);
      
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse.rbg;  
       
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    vec3 specular = light.specular * spec * material.specular.rbg;  
    //    
    vec3 result = material.diffuse.rgb;
    FragColor = vec4(result, 1.0);
}

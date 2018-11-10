#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 direction;
uniform sampler2D texture1;

void main()
{
		vec3 color=texture(texture1,TexCoord).rgb;		
  	vec3 ambient = vec3(0.5,0.5,0.5);   	
    vec3 lightColor=vec3(0.8,0.8,0.8);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -direction), 0.0);
		vec3 diffuse = lightColor * diff;  
		vec3 result = ambient + diffuse;
		result=result*color;		
    //FragColor = vec4(result, 1.0);
    
    FragColor = texture(texture1,TexCoord);
}
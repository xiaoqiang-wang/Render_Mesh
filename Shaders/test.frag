#version 330 core                             
                                              
in vec2 TexCoord;                             
uniform vec4 global_time;                     
out vec4 color;                               
// texture samplers                           
uniform sampler2D texture1;                   
uniform sampler2D texture2;                   
                                              
void main()                                   
{                                             
	//vec4 color1 =  vec4(1.0, 1.0,1.0,1.0);    
	//vec4 color2 = texture(texture1, TexCoord);
	color =  vec4(1.0, 0.0,0.0,0.0);          
	//color =  vec4(global_time.x);             
	//color = (texture(texture1, TexCoord)*0.8);
}

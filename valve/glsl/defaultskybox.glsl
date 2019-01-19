!!ver 110
!!permu FOG
!!samps reflectcube
!!cvardf gl_fake16bit=0

#include "sys/defs.h"
#include "sys/fog.h"

varying vec3 pos;
#ifdef VERTEX_SHADER
void main ()
{
	pos = v_position.xyz - e_eyepos;
	pos.y = -pos.y;
	gl_Position = ftetransform();
}
#endif
#ifdef FRAGMENT_SHADER
void main ()
{
	vec4 skybox = textureCube(s_reflectcube, pos);
#if gl_fake16bit == 1
	skybox.rgb = floor(skybox.rgb * vec3(32,64,32))/vec3(32,64,32);
#endif
	gl_FragColor = vec4(fog3(skybox.rgb), 1.0);
}
#endif

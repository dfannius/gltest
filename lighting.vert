#version 330

// per-vertex inputs
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

// global inputs
uniform mat4 model_to_camera_mtx;
uniform mat3 normal_to_camera_mtx;
uniform mat4 camera_to_clip_mtx;
uniform vec3 light_pos;

// outputs
out vec4 vert_color;                 // passed to fragment shader

void main()
{
   vec4 position_cam = model_to_camera_mtx * vec4( position, 1.0 );
   gl_Position = camera_to_clip_mtx * position_cam;
   vec3 normal_cam = normalize( normal_to_camera_mtx * normal );
   vec3 dir_to_light = normalize( light_pos.xyz - position_cam.xyz );
   float cos_incidence = clamp( dot( normal_cam, dir_to_light ), 0.02, 1 );

   // vert_color = vec4( cos_incidence * color, 1.0 );
   vert_color = vec4( color, 1.0 );
}

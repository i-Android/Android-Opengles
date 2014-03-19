attribute vec4 vertex_attrib;
attribute vec4 tex_coord_attrib;
attribute vec4 color_attrib;
uniform mat4 mvp_matrix;
uniform mat4 texture_matrix;
varying vec4 frag_color;
varying vec2 tex_coord;

void main(void)
{
  gl_Position = mvp_matrix * vertex_attrib;
  vec4 transformed_tex_coord = texture_matrix * tex_coord_attrib;
  tex_coord = transformed_tex_coord.st / transformed_tex_coord.q;
  frag_color = color_attrib;
}
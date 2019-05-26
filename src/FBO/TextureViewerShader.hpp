#ifndef SPIRITENGINE_TEXTUREVIEWERSHADER_HPP
#define SPIRITENGINE_TEXTUREVIEWERSHADER_HPP

#include <string>

static std::string TextureViewerVertexShader =
				"#version 410\n"
				"layout(location = 0) in vec2 vPosition;\n"
				"layout(location = 1) in vec2 vUV;\n"
				"out vec2 uv;\n"
				"void main()\n"
				"{\n"
				"\tgl_Position = vec4(vPosition,1.0,1.0);\n"
				"\tuv = vUV;\n"
				"}";

static std::string TextureViewerFragmentShader =
				"#version 410\n"
				"in vec2 uv;\n"
				"out vec4 final_color;\n"
				"uniform sampler2D tex;\n"
				"uniform bool depth;\n"
				"uniform float zNear;\n"
				"uniform float zFar;\n"
				"float LinearizeDepth(in vec2 uv)\n"
				"{\n"
				"    float depth = texture(tex, uv).x;\n"
				"    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));\n"
				"}\n"
				"void main()\n"
				"{\n"
				"    //rrra because depth textures are not usual textures, they have only one channel\n"
				"    final_color = (depth) ? texture(tex, uv).rrra : texture(tex, uv).rgba;\n"
				"\n"
				"    float d;\n"
				"    if (depth) {\n"
				"        d = LinearizeDepth(uv);\n"
				"        final_color = vec4(d, d, d, 1.0);\n"
				"    }\n"
				"    //final_color = texture(tex, uv).rrra;\n"
				"    //final_color = vec4(1.0,0.0,0.0,1.0);\n"
				"}";

#endif /* !SPIRITENGINE_TEXTUREVIEWERSHADER_HPP */

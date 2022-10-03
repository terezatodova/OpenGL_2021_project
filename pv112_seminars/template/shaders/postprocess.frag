#version 450

// Edge detection convolution kernel
/*
const float edgeDetectionKernel[3][3] = {
	{-1.0, -1.0, -1.0},
	{-1.0,  8.0, -1.0},
	{-1.0, -1.0, -1.0}
};
*/
const float edgeDetectionKernel[3][3] = {
	{0.0, -1.0, 0.0},
	{-1.0,  4.0, -1.0},
	{0.0, -1.0, 0.0}
};

layout(binding = 0) uniform sampler2D input_image;

layout(location = 0) out vec4 final_color;

void main() {
	// --------------------------------------------------------------------------
	// TODO Task 7.3: Apply post-process effects (grayscale, convolution kernels)
	// --------------------------------------------------------------------------
	// Use texelFetch(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/texelFetch.xhtml)
	// to retrieve exact pixel and gl_FragCoord.xy to retrieve the coordinates of the fragment 
	// currently being processed.
	//
	// - make the color grayscale (https://en.wikipedia.org/wiki/Grayscale)
	// - apply a convolution kernel (https://en.wikipedia.org/wiki/Kernel_(image_processing))

	//vec3 color = texelFetch(input_image, ivec2(gl_FragCoord.xy), 0).rgb;
	//float gray = 0.2126*color[0] + 0.7152*color[1] + 0.0722*color[2];
	//final_color = vec4(gray, gray, gray, 1.0);

	vec3 color = vec3(0, 0, 0);
	ivec2 sz = textureSize(input_image,0);
	for (int i = -1; i<= 1; i++) {
		for (int j = -1; j<= 1; j++) {
			if (gl_FragCoord.x + i < 0)
				continue;
			if (gl_FragCoord.y + j < 0)
				continue;
			if (gl_FragCoord.x + i < 0)
				continue;
			if (gl_FragCoord.x + i > sz[0])
				continue;
			if (gl_FragCoord.y + j > sz[1])
				continue;
			color += texelFetch(input_image, ivec2(gl_FragCoord.x + i, gl_FragCoord.y + j), 0).rgb * (edgeDetectionKernel[i+1][j+1]);
		}
	} 
	
	if (color == vec3(0,0,0))
	{
		color = texelFetch(input_image, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0).rgb;
	}
	else 
	{
		color = vec3(0,0,0);
	}

	final_color = vec4(color, 1.0);	
}
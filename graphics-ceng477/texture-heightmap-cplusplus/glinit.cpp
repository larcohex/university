#include "glinit.h"

void initTextures()
{	
	int width, height, channels = 1;
	
	// read images into memory to generate textures
	unsigned char* textureMapData = SOIL_load_image("res/texturemap.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* heightMapData  = SOIL_load_image("res/heightmap.jpg", &width, &height, &channels, SOIL_LOAD_L);
	
	
	// TODO: Generate texture map and height map for the Earth

	glGenTextures(1, &earthTexture); 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, textureMapData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	printf("Earth Texture Info: %s\n", result_string_pointer);
	

	glGenTextures(1, &heightTexture); 
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, heightTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE, heightMapData);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	printf("Height Map Info: %s\n", result_string_pointer);

	
	// Generate CubeMap
	glGenTextures(1, &gTextureCubeMap);
	glActiveTexture(GL_TEXTURE2);
	
	glBindTexture(GL_TEXTURE_2D, gTextureCubeMap);
	
	gTextureCubeMap = SOIL_load_OGL_single_cubemap("res/cubemap.jpg", "WNESUD", SOIL_LOAD_AUTO, gTextureCubeMap, SOIL_FLAG_MIPMAPS);
	

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
	/**/
	
	printf("CubeMap Info: %s\n", result_string_pointer);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void initShaders()
{
	// Creation and initialization of CubeMap's shaders
	gProgramCubeMap = glCreateProgram();
	
    glAttachShader(gProgramCubeMap, createVS("res/cubemap.vert"));
    glAttachShader(gProgramCubeMap, createFS("res/cubemap.frag"));
    
    glLinkProgram(gProgramCubeMap);
    
	glUniform1i(glGetUniformLocation(gProgramCubeMap, "cubeMap")   , 2);
	
	// TODO: Create and initialize new program(s) to use new shader(s)

	gEarthRender =  glCreateProgram();
	glAttachShader (gEarthRender, createVS("res/shader.vert"));
	glAttachShader (gEarthRender, createFS("res/shader.frag"));

	glLinkProgram (gEarthRender);
	
	
	
	
}


void initCubeMap(GLuint *verticesVBO, GLuint *indicesVBO) 
{
	float vertices[] = {
		  1,  1,  1,				
		 -1,  1,  1,
		 -1, -1,  1,
		  1, -1,  1,
		  1, -1, -1,
		 -1, -1, -1,
		 -1,  1, -1,
		  1,  1, -1
	};

	GLuint indices[] = {
		0, 1, 2, 3,                 // Front face of the cube
		7, 4, 5, 6,                 // Back face of the cube
		6, 5, 2, 1,                 // Left face of the cube
		7, 0, 3, 4,                 // Right face of the cube
		7, 6, 1, 0,                 // Top face of the cube
		3, 2, 5, 4,                 // Bottom face of the cube
	};
	
	glGenBuffers(1, verticesVBO);
	glGenBuffers(1, indicesVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, *verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}



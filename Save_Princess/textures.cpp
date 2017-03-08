#include "error_log.h"
#include "textures.h"
#include "file_load.h"
#include "glerrors.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#pragma pack(1)
struct TGAHeader
{
	uint8_t  idlength;
	uint8_t  colormap;
	uint8_t  datatype;
	uint8_t  colormapinfo[5];
	uint16_t xorigin;
	uint16_t yorigin;
	uint16_t width;
	uint16_t height;
	uint8_t  bitperpel;
	uint8_t  description;
};
#pragma pack()

GLuint get_texture_from_tga(char *name)
{
	TGAHeader *header;
	char *buffer;
	GLint format, internalFormat;
	GLuint texture;
	char *buf = new char[100];
	
	if (!load_file(name, true, true, &buffer))
		return 0;

	header = (TGAHeader*)buffer;

	if (header->datatype != 2 || (header->bitperpel != 24 && header->bitperpel != 32))
	{
		sprintf(buf, "Wrong TGA format in file '%s'\n", name);
		raise_error(buf, false, true);
		delete[] buffer;
		return 0;
	}

	format = (header->bitperpel == 24 ? GL_BGR : GL_BGRA);
	internalFormat = (format == GL_BGR ? GL_RGB8 : GL_RGBA8);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, header->width, header->height, 0, format,
	//	GL_UNSIGNED_BYTE, (const GLvoid*)(buffer + sizeof(TGAHeader)+header->idlength));

	gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, header->width, header->height, format,
		GL_UNSIGNED_BYTE, (const GLvoid*)(buffer + sizeof(TGAHeader)+header->idlength));

	delete[] buffer;
	delete[] buf;

	if (check_GL_error())
		return ST_TEXTURE_INIT_ERROR;

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}


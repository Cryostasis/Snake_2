#ifndef _LOADOBJECT_H_INC_
#define _LOADOBJECT_H_INC_

#define LOAD_BIN 0

#define BIN_FORMAT_VER 1

enum LOAD_STATE {LS_OK, LS_FAIL};

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>

#define MAX_OBJECTS 15

struct ObjInfo
{
	int vert_num;
	int ind_num;
	std::vector<GLfloat> verts;
	std::vector<GLfloat> texcoords;
	std::vector<GLfloat> normals;
	std::vector<GLuint>  indicies;

	std::vector<GLfloat> N_verts;
	std::vector<GLuint>  N_indicies;

	std::vector<GLfloat> P_verts;
	std::vector<GLuint>  P_indicies;

	std::string file;
};

extern int obj_count;
extern ObjInfo objects[MAX_OBJECTS];

//void load_object(g_object *obj, char *file);
int reg_object(char *file);
ObjInfo clone_obj(char *file);
ObjInfo* clone_obj_ptr(char *file);

#endif
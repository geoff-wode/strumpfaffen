#include <gl_loader/gl_loader.h>

//----------------------------------------------------------------------------------
const char* const GetAttribTypeName(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT						 : return "GL_FLOAT";
	case GL_FLOAT_VEC2			 : return "GL_FLOAT_VEC2";
	case GL_FLOAT_VEC3			 : return "GL_FLOAT_VEC3";
	case GL_FLOAT_VEC4			 : return "GL_FLOAT_VEC4";
	case GL_FLOAT_MAT2			 : return "GL_FLOAT_MAT2";
	case GL_FLOAT_MAT3			 : return "GL_FLOAT_MAT3";
	case GL_FLOAT_MAT4			 : return "GL_FLOAT_MAT4";
	case GL_FLOAT_MAT2x3		 : return "GL_FLOAT_MAT2x3";
	case GL_FLOAT_MAT2x4		 : return "GL_FLOAT_MAT2x4";
	case GL_FLOAT_MAT3x2		 : return "GL_FLOAT_MAT3x2";
	case GL_FLOAT_MAT3x4		 : return "GL_FLOAT_MAT3x4";
	case GL_FLOAT_MAT4x2		 : return "GL_FLOAT_MAT4x2";
	case GL_FLOAT_MAT4x3		 : return "GL_FLOAT_MAT4x3";
	case GL_INT							 : return "GL_INT	";
	case GL_INT_VEC2				 : return "GL_INT_VEC2";
	case GL_INT_VEC3				 : return "GL_INT_VEC3";
	case GL_INT_VEC4				 : return "GL_INT_VEC4";
	case GL_UNSIGNED_INT		 : return "GL_UNSIGNED_INT";
	case GL_UNSIGNED_INT_VEC2: return "GL_UNSIGNED_INT_VEC2";
	case GL_UNSIGNED_INT_VEC3: return "GL_UNSIGNED_INT_VEC3";
	case GL_UNSIGNED_INT_VEC4: return "GL_UNSIGNED_INT_VEC4";
	default: break;
	}
	return "<unknown>";
}

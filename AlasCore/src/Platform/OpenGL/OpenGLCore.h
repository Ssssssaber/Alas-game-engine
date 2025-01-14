#include <glad/glad.h>

#ifdef ALAS_ENABLE_ASSERTS
    #define GlCall(x)  GLClearError();\
        x;\
        ALAS_CORE_ASSERT(GLLogCall(#x, __FILE__, __LINE__), "OPEN GL ERROR"); 

    static void GLClearError()
    {
        while(glGetError() != GL_NO_ERROR);
    }

    static bool GLLogCall(const char* function, const char* file, int line)
    {
        while (GLenum error = glGetError())
        {
            ALAS_CORE_ERROR("[OPENGL ERROR]: (code: {0}) {1}; {2}; {3}", error, function, file, line);
            return false;
        }
        return true;
    }
#else
    #define GlCall(x) {}
#endif


#include "OpenGlShader.h"
#include "Platform/OpenGL/OpenGLCore.h"

namespace Alas {

    OpenGLShader::OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) : Shader()
    {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		// Compile the vertex shader
		glCompileShader(vertexShader);
		int isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			// We don't need the shader anymore.
			glDeleteShader(vertexShader);
			ALAS_CORE_ERROR("{0}", infoLog.data());
			ALAS_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}
		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
		// Compile the fragment shader
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);
			ALAS_CORE_ERROR("{0}", infoLog.data());
			ALAS_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		_rendererId = glCreateProgram();
		GLuint program = _rendererId;
		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		// Link our program
		glLinkProgram(program);
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			ALAS_CORE_ERROR("{0}", infoLog.data());
			ALAS_ASSERT(false, "Shader link failure!");
			return;
		}
		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(_rendererId);
    }

    void OpenGLShader::Bind() const 
    {
        GlCall(glUseProgram(_rendererId));
    }
    
    void OpenGLShader::Unbind() const
    {
        GlCall(glUseProgram(0));
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void OpenGLShader::setBool(const std::string &name, bool value) const
    {         
        Bind();
        GlCall(glUniform1i(glGetUniformLocation(_rendererId, name.c_str()), (int)value)); 
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setInt(const std::string &name, int value) const
    { 
        Bind();
        GlCall(glUniform1i(glGetUniformLocation(_rendererId, name.c_str()), value)); 
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setFloat(const std::string &name, float value) const
    { 
        Bind();
        GlCall(glUniform1f(glGetUniformLocation(_rendererId, name.c_str()), value)); 
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        Bind();
        GlCall(glUniform2fv(glGetUniformLocation(_rendererId, name.c_str()), 1, &value[0])); 
    }
    void OpenGLShader::setVec2(const std::string &name, float x, float y) const
    { 
        Bind();
        GlCall(glUniform2f(glGetUniformLocation(_rendererId, name.c_str()), x, y)); 
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        Bind();
        GlCall(glUniform3fv(glGetUniformLocation(_rendererId, name.c_str()), 1, &value[0])); 
    }
    void OpenGLShader::setVec3(const std::string &name, float x, float y, float z) const
    { 
        Bind();
        GlCall(glUniform3f(glGetUniformLocation(_rendererId, name.c_str()), x, y, z)); 
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        Bind();
        GlCall(glUniform4fv(glGetUniformLocation(_rendererId, name.c_str()), 1, &value[0])); 
    }

    void OpenGLShader::setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        Bind();
        auto id = glGetUniformLocation(_rendererId, name.c_str());
        GlCall(
            glUniform4f(
                id,
                x, y, z, w)
            ); 
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        Bind();
        GlCall(glUniformMatrix2fv(glGetUniformLocation(_rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        Bind();
        GlCall(glUniformMatrix3fv(glGetUniformLocation(_rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }
    // ------------------------------------------------------------------------
    void OpenGLShader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        Bind();
        GlCall(glUniformMatrix4fv(glGetUniformLocation(_rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }

}
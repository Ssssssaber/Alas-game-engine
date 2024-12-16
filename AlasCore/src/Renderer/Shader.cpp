#include "Shader.h"
#include "Platform/OpenGL/OpenGLCore.h"

namespace AGS {

    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
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
			AGS_CORE_ERROR("{0}", infoLog.data());
			AGS_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}
		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
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
			AGS_CORE_ERROR("{0}", infoLog.data());
			AGS_ASSERT(false, "Fragment shader compilation failure!");
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
			AGS_CORE_ERROR("{0}", infoLog.data());
			AGS_ASSERT(false, "Shader link failure!");
			return;
		}
		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(_rendererId);
    }

    void Shader::Bind() const 
    {
        GlCall(glUseProgram(_rendererId));
    }
    
    void Shader::Unbind() const
    {
        GlCall(glUseProgram(0));
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void Shader::setBool(const std::string &name, bool value) const
    {         
        GlCall(glUniform1i(glGetUniformLocation(_rendererId, name.c_str()), (int)value)); 
    }
    // ------------------------------------------------------------------------
    void Shader::setInt(const std::string &name, int value) const
    { 
        GlCall(glUniform1i(glGetUniformLocation(_rendererId, name.c_str()), value)); 
    }
    // ------------------------------------------------------------------------
    void Shader::setFloat(const std::string &name, float value) const
    { 
        GlCall(glUniform1f(glGetUniformLocation(_rendererId, name.c_str()), value)); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        GlCall(glUniform2fv(glGetUniformLocation(_rendererId, name.c_str()), 1, &value[0])); 
    }
    void Shader::setVec2(const std::string &name, float x, float y) const
    { 
        GlCall(glUniform2f(glGetUniformLocation(_rendererId, name.c_str()), x, y)); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        GlCall(glUniform3fv(glGetUniformLocation(_rendererId, name.c_str()), 1, &value[0])); 
    }
    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    { 
        GlCall(glUniform3f(glGetUniformLocation(_rendererId, name.c_str()), x, y, z)); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        GlCall(glUniform4fv(glGetUniformLocation(_rendererId, name.c_str()), 1, &value[0])); 
    }
    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        auto id = glGetUniformLocation(_rendererId, name.c_str());
        GlCall(
            glUniform4f(
                id,
                x, y, z, w)
            ); 
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        GlCall(glUniformMatrix2fv(glGetUniformLocation(_rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        GlCall(glUniformMatrix3fv(glGetUniformLocation(_rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        GlCall(glUniformMatrix4fv(glGetUniformLocation(_rendererId, name.c_str()), 1, GL_FALSE, &mat[0][0]));
    }

}
#include "ShaderProgram.h"
#include <iostream>


namespace Renderer{

	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
		GLuint vertexShaderId;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderId)){
			std::cerr << "Error VERTEX Shader Compile-time error\n";
			return;			
		}

		GLuint fragmentShaderId;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderId)) {
			std::cerr << "Error FRAGMENT Shader Compile-time error\n";
			glDeleteShader(vertexShaderId);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderId);
		glAttachShader(m_ID, fragmentShaderId);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success){
			GLchar infoLog[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
			std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
		}else	{
			m_isCompiled = true;
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}


	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID){
		shaderID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);
		
		GLint success;

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if(!success){
			GLchar inforLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, inforLog);
			std::cerr << "Error Shader Compile-time error\n" << inforLog << std::endl;
			return  false;
		}else {
			return  true;
		}		
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_ID);
	}

	void ShaderProgram::use()	const{
		glUseProgram(m_ID);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept	{
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
		return *this;		
	}
	
	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)	noexcept {
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;
		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;		
	}

}



#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// Shader Program
	unsigned int ID;

	Shader() {}
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		ComplieVertexGeometryFragmentShader(vertexPath, fragmentPath, geometryPath);
	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMatrix4fv(const std::string &name, GLsizei count, GLboolean transpose, const GLfloat* value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, transpose, value);
	}
	void setMat4(const std::string &name, const glm::mat4 mat4) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec3(const std::string &name, glm::vec3 pos) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), pos.x, pos.y, pos.z);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void setVec2(const std::string &name, glm::vec2 pos) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), pos.x, pos.y);
	}

	// note: geometry source code is optional
	void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr)
	{
		if (geometrySource != nullptr) {
			printf("Shader, begin complie, vertexSource: %s, fragmentSource: %s, geometrySource: %s\n", vertexSource, fragmentSource, geometrySource);
		}
		else {
			printf("Shader, begin complie, vertexSource: %s, fragmentSource: %s\n", vertexSource, fragmentSource);
		}
		unsigned int sVertex, sFragment, gShader;
		// vertex Shader
		sVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(sVertex, 1, &vertexSource, NULL);
		glCompileShader(sVertex);
		checkCompileErrors(sVertex, "VERTEX");
		// fragment Shader
		sFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(sFragment, 1, &fragmentSource, NULL);
		glCompileShader(sFragment);
		checkCompileErrors(sFragment, "FRAGMENT");
		// if geometry shader source code is given, also compile geometry shader
		if (geometrySource != nullptr)
		{
			gShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShader, 1, &geometrySource, NULL);
			glCompileShader(gShader);
			checkCompileErrors(gShader, "GEOMETRY");
		}
		// shader program
		this->ID = glCreateProgram();
		glAttachShader(this->ID, sVertex);
		glAttachShader(this->ID, sFragment);
		if (geometrySource != nullptr) {
			glAttachShader(this->ID, gShader);
		}
		glLinkProgram(this->ID);
		checkCompileErrors(this->ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(sVertex);
		glDeleteShader(sFragment);
		if (geometrySource != nullptr) {
			glDeleteShader(gShader);
		}
		if (geometrySource != nullptr) {
			printf("Shader, link success, vertexSource: %s, fragmentSource: %s, geometrySource: %s\n", vertexSource, fragmentSource, geometrySource);
		}
		else {
			printf("Shader, link success, vertexSource: %s, fragmentSource: %s\n", vertexSource, fragmentSource);
		}
	}

private:
	// ±àÒë¶¥µã¡¢Æ¬¶Î×ÅÉ«Æ÷
	void ComplieVertexGeometryFragmentShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			if (geometryPath != nullptr) {
				gShaderFile.open(geometryPath);
			}
			std::stringstream vShaderStream, fShaderStream, gShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			if (geometryPath != nullptr) {
				gShaderStream << gShaderFile.rdbuf();
			}
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			if (geometryPath != nullptr) {
				gShaderFile.close();
			}
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			if (geometryPath != nullptr) {
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		const char* gShaderCode = nullptr;
		if (geometryPath != nullptr) {
			gShaderCode = geometryCode.c_str();
		}

		Compile(vShaderCode, fShaderCode, gShaderCode);
	}

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

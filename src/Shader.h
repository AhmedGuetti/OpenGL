#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <unordered_map>

/**
 * A Data Type holding the code for the two type of Shaders
 * Vertex Shader: Positions of the vertexs
 * Fragment Shader: color and resterisation of the objects
 *
 */
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniform
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	// Add a method to set matrix uniforms
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	// Get renderer ID - added for direct access to shader program ID
	inline unsigned int GetRendererID() const { return m_RendererID; }

private:
	unsigned int CompileShader(unsigned int type, std::string& source);
	int CreateShader(std::string& vetrexShader, std::string fragementShader);
	ShaderProgramSource parseShader(const std::string& filePath);
	unsigned int GetUniformLocation(const std::string& name);

};
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VoxelRenderer.h"
#include "Controls.hpp"
#include "ICamera.h"
#include "ILightSource.h"
#include "ShaderManager.h"

#include <iostream>

VoxelRenderer::VoxelRenderer(): _normalBuffer(0)
{
	_vertexBuffer = 0;
	_colorBuffer = 0;
	_numberOfVertices = 0;
	_vertexArrayId = 0;
	_visibilityBuffer = 0;
}


VoxelRenderer::~VoxelRenderer()
{
	deleteBuffersIfNeeded();
}

void VoxelRenderer::deleteBuffersIfNeeded() const
{
	if (_vertexArrayId != 0)
	{
		glDeleteVertexArrays(1, &_vertexArrayId);
	}
	if (_vertexBuffer != 0)
	{
		glDeleteBuffers(1, &_vertexBuffer);
	}
	if (_colorBuffer != 0)
	{
		glDeleteBuffers(1, &_colorBuffer);
	}
	if (_visibilityBuffer != 0)
	{
		glDeleteBuffers(1, &_visibilityBuffer);
	}
}

void VoxelRenderer::buildBuffers(const VoxelContainerGeometry& chunkGeometry)
{
	deleteBuffersIfNeeded();

	if (chunkGeometry.needsRendering())
	{
		glGenVertexArrays(1, &_vertexArrayId);
		glBindVertexArray(_vertexArrayId);

		const GLfloat* vertices = &(chunkGeometry.getVertices()[0]);
		const GLfloat* colors = &(chunkGeometry.getColors()[0]);
		const GLfloat* normals = &(chunkGeometry.getNormals()[0]);
		const GLfloat* vertexVisibilities = &(chunkGeometry.getVisbility()[0]);

		_numberOfVertices = static_cast<GLsizei>(chunkGeometry.getVertices().size());
		GLsizeiptr vertexSize = _numberOfVertices * sizeof(GLfloat);
		glGenBuffers(1, &_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

		GLsizeiptr colorSize = chunkGeometry.getColors().size() * sizeof(GLfloat);
		glGenBuffers(1, &_colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colorSize, colors, GL_STATIC_DRAW);

		GLsizeiptr normalsSize = chunkGeometry.getNormals().size() * sizeof(GLfloat);
		glGenBuffers(1, &_normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normalsSize, normals, GL_STATIC_DRAW);

		GLsizeiptr vertexVisibiltySize = chunkGeometry.getVisbility().size() * sizeof(GLfloat);
		glGenBuffers(1, &_visibilityBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _visibilityBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexVisibiltySize, vertexVisibilities, GL_STATIC_DRAW);
	}
}

void VoxelRenderer::rebindVisibility(const VoxelContainerGeometry& chunkGeometry, const voxelIndex& voxelIndex) const
{
	const GLfloat* vertexVisibilities = &(chunkGeometry.getVisbility()[voxelIndex.start]);
	GLsizeiptr vertexVisibiltySize = chunkGeometry.getVisbility().size() * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, _visibilityBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, voxelIndex.start * sizeof(GLfloat), (voxelIndex.end - voxelIndex.start) * sizeof(GLfloat), vertexVisibilities);
}

void VoxelRenderer::render(const ICamera& camera, const VoxelContainerGeometry& chunkGeometry, const ILightSource& light, glm::mat4 modelMatrix) const
{
	
	if (chunkGeometry.needsRendering())
	{
		glBindVertexArray(_vertexArrayId);

		// shader parameters
		GLuint shaderId = ShaderManager::instance->getSimpleColored();
		GLuint matrixID = glGetUniformLocation(shaderId, "MVP");
		GLuint viewMatrixID = glGetUniformLocation(shaderId, "V");
		GLuint modelMatrixID = glGetUniformLocation(shaderId, "M");
		GLuint lightID = glGetUniformLocation(shaderId, "LightPosition_worldspace");
		GLuint lightPowerID = glGetUniformLocation(shaderId, "LightPower");
		GLuint lightColorID = glGetUniformLocation(shaderId, "LightColor");

		std::cout << "glUseProgram" << std::endl;
		glUseProgram(shaderId);
std::cout << "~glUseProgram" << std::endl;
		// camera
		glm::mat4 ProjectionMatrix = camera.getProjectionMatrix();
		glm::mat4 ViewMatrix = camera.getViewMatrix();
		//glm::mat4 ModelMatrix = glm::mat4(1.0);
		//glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 20.0, 0.0));
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * modelMatrix;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		// lighting
		lightSourcePosition lightPosition = light.getPosition();
		color lightColor = light.getColor();
		glUniform3f(lightID, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform1f(lightPowerID, light.getPower());
		glUniform3f(lightColorID, lightColor.r, lightColor.g, lightColor.b);

		// rendering
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glVertexAttribPointer(
			0,                  // must match the layout in the shader
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
		glVertexAttribPointer(
			1,                                // must match the layout in the shader
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
		glVertexAttribPointer(
			2,                                // must match the layout in the shader
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, _visibilityBuffer);
		glVertexAttribPointer(
			3,                                // must match the layout in the shader
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
std::cout << "glDrawArrays" << std::endl;
		glDrawArrays(GL_TRIANGLES, 0, _numberOfVertices);
		std::cout << "~glDrawArrays" << std::endl;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

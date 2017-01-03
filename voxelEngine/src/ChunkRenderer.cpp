#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ChunkRenderer.h"
#include "controls.hpp"
#include "ICamera.h"
#include "ILightSource.h"

ChunkRenderer::ChunkRenderer()
{
	_vertexBuffer = 0;
	_colorBuffer = 0;
	_numberOfVertices = 0;
	_vertexArrayId = 0;
}


ChunkRenderer::~ChunkRenderer()
{
	deleteBuffersIfNeeded();
}

void ChunkRenderer::deleteBuffersIfNeeded() const
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
}

void ChunkRenderer::buildBuffers(const ChunkGeometry& chunkGeometry)
{
	deleteBuffersIfNeeded();

	if (chunkGeometry.needsRendering())
	{
		glGenVertexArrays(1, &_vertexArrayId);
		glBindVertexArray(_vertexArrayId);

		const GLfloat* vertices = &(chunkGeometry.getVertices()[0]);
		const GLfloat* colors = &(chunkGeometry.getColors()[0]);
		const GLfloat* normals = &(chunkGeometry.getNormals()[0]);

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
	}
}

void ChunkRenderer::render(const ICamera& camera, const ChunkGeometry& chunkGeometry, const ILightSource& light) const
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

		glUseProgram(shaderId);

		// camera
		glm::mat4 ProjectionMatrix = camera.getProjectionMatrix();
		glm::mat4 ViewMatrix = camera.getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
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

		glDrawArrays(GL_TRIANGLES, 0, _numberOfVertices);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

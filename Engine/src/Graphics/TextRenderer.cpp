#include "TextRenderer.hpp"

TextRenderer::TextRenderer(GLuint shaderProgramID, Camera* camera, Texture* bitmapFont, int birmapSizeHorizontal, int birmapSizeVertical)
{
	this->shaderProgramID = shaderProgramID;
	this->camera = camera;
	this->bitmapFont = bitmapFont;
	this->birmapSizeHorizontal = birmapSizeHorizontal;
	this->birmapSizeVertical = birmapSizeVertical;

	std::vector<float> vertices;
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(0);
	vertices.push_back(1);

	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);

	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(0);
	vertices.push_back(0);

	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(0);

	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	VAO = new VertexArray;
	VAO->bind();

	VBO = new ArrayBuffer(vertices);
	VBO->bind();

	EBO = new ElementArrayBuffer(indices);
	EBO->bind();

	VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	VAO->setAttributePointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);

	VAO->unbind();
}

void TextRenderer::draw(float x, float y, float size, char character)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	transform = glm::translate(transform, glm::vec3(camera->getZoom() * x + camera->getOffset().x, camera->getZoom() * y + camera->getOffset().y, 0));
	transform = glm::scale(transform, glm::vec3(camera->getZoom() * size, camera->getZoom() * size, 1));

	glm::vec2 characterSize = glm::vec2(bitmapFont->getWidth() / birmapSizeHorizontal, bitmapFont->getHeight() / birmapSizeVertical);
	int t_x = (int)(character % birmapSizeHorizontal);
	int t_y = (int)(character / birmapSizeVertical);
	t_y = birmapSizeVertical - t_y-1;

	glm::mat4 textureTransform = glm::mat4(1.0f);
	textureTransform = glm::ortho(0.0f, (float)bitmapFont->getWidth(), 0.0f, (float)bitmapFont->getHeight(), -1.0f, 1.0f);
	textureTransform = glm::translate(textureTransform, glm::vec3(characterSize.x/2 * t_x, characterSize.y/2 * t_y, 0.0f));
	textureTransform = glm::scale(textureTransform, glm::vec3(characterSize.x/2, characterSize.y/2, 1));

	glUseProgram(shaderProgramID);

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	unsigned int texTransform = glGetUniformLocation(shaderProgramID, "texTransform");
	glUniformMatrix4fv(texTransform, 1, GL_FALSE, glm::value_ptr(textureTransform));

	unsigned int colorLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(colorLocation, color.x, color.y, color.z);

	bitmapFont->bind();
	VAO->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TextRenderer::draw(float x, float y, float size, std::string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		this->draw(x + i * size, y, size, str[i]);
	}
}

void TextRenderer::setColor(float r, float g, float b)
{
	color = glm::vec3(r, g, b);
}

#include <LOGL/VAO.h>
#include <glad/glad.h>

using namespace std;
using namespace LOGL;

VAO::VAO(float const * data, size_t dataSize, const std::vector<size_t> & attrLen) {
	isValid = false;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	size_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	size_t patchLen = 0;
	for (auto & len : attrLen)
		patchLen += len;
	for (size_t i = 0, accu = 0; i < attrLen.size(); i++) {
		glVertexAttribPointer(i, attrLen[i], GL_FLOAT, GL_FALSE, patchLen * sizeof(float), (void*)(accu * sizeof(float)));
		glEnableVertexAttribArray(i);
		accu += attrLen[i];
	}
	isValid = true;
}

VAO::VAO(float const * data, size_t dataSize, const std::vector<size_t> & attrLen, size_t const * index, size_t indexSize)
	: VAO(data, dataSize, attrLen){
	isValid &= GenBindEBO(index, indexSize);
}

VAO::VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch) {
	isValid = false;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	for (size_t i = 0; i < vec_VBO_DataPatch.size(); i++) {
		auto & dataPatch = vec_VBO_DataPatch[i];
		size_t VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, dataPatch.dataSize, dataPatch.data, GL_STATIC_DRAW);
		glVertexAttribPointer(i, dataPatch.attrLen, GL_FLOAT, GL_FALSE, dataPatch.attrLen * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(i);
	}

	isValid = true;
}

VAO::VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch, size_t const * index, size_t indexSize)
	: VAO(vec_VBO_DataPatch){
	isValid &= GenBindEBO(index, indexSize);
}

bool VAO::GenBindEBO(size_t const * index, size_t indexSize) {
	size_t EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, index, GL_STATIC_DRAW);
	return true;
}

size_t VAO::GetID() const {
	return ID;
}

bool VAO::IsValid() const {
	return isValid;
}

bool VAO::Use() const {
	if (!IsValid())
		return false;

	glBindVertexArray(ID);
	return true;
}
#include <Utility/Sphere.h>

#include <cmath>

using namespace Ubpa;

Sphere::Sphere(size_t n)
	: Shape((n + 1)*(n + 1), 2 * n*n) {
	// normal �� pos ��ֵ��ͬ
	normalArr = vertexArr;
	texCoordsArr = new Array2D<float>(vertexNum, 2);
	indexArr = new Array2D<size_t>(triNum, 3);
	//----------
	float inc = 1.0f / n;
	for (int i = 0; i <= n; i++) {
		float u = inc * i;
		for (int j = 0; j <= n; j++) {
			float v = inc * j;
			float theta = PI * (1-u);
			float phi = 2 * PI * v;
			// ����ϵ: ��y, ��x, ��ֱ��Ļ��Ϊ z
			vertexArr->At(i*(n + 1) + j, 0) = sinf(theta) * sinf(phi);
			vertexArr->At(i*(n + 1) + j, 1) = cosf(theta);
			vertexArr->At(i*(n + 1) + j, 2) = sinf(theta) * cosf(phi);
			// u ��Ӧ��������Ӧ������������� t
			// v ��Ӧ��������Ӧ������������� s
			texCoordsArr->At(i*(n + 1) + j, 0) = v;
			texCoordsArr->At(i*(n + 1) + j, 1) = u;
		}
	}
	//------------
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			// ���� ���� ����
			indexArr->At(2 * (i*n + j), 0) = i * (n + 1) + j;
			indexArr->At(2 * (i*n + j), 1) = i * (n + 1) + j + 1;
			indexArr->At(2 * (i*n + j), 2) = (i + 1) * (n + 1) + j;
			// ���� ���� ����
			indexArr->At(2 * (i*n + j) + 1, 0) = (i + 1) * (n + 1) + j + 1;
			indexArr->At(2 * (i*n + j) + 1, 1) = i * (n + 1) + j + 1;
			indexArr->At(2 * (i*n + j) + 1, 2) = (i + 1) * (n + 1) + j;
		}
	}
}

Sphere::~Sphere() {
	// normalArr �� vertexArr ��ͬ, �������ͷ�normalArr
	normalArr = nullptr;
	delete texCoordsArr;
	texCoordsArr = nullptr;
	delete indexArr;
	indexArr = nullptr;
}

float * Sphere::GetNormalArr() {
	if (normalArr == nullptr)
		return nullptr;

	return normalArr->GetData();
}

float * Sphere::GetTexCoordsArr() {
	if (texCoordsArr == nullptr)
		return nullptr;

	return texCoordsArr->GetData();
}

size_t * Sphere::GetIndexArr() {
	if (indexArr == nullptr)
		return nullptr;

	return indexArr->GetData();
}

size_t Sphere::GetNormalArrSize() {
	return normalArr->GetMemSize();
}

size_t Sphere::GetTexCoordsArrSize() {
	return texCoordsArr->GetMemSize();
}

size_t Sphere::GetIndexArrSize() {
	return indexArr->GetMemSize();
}
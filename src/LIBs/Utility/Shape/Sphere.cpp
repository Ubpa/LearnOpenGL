#include <Utility/Sphere.h>

#include <Utility/Vec.h>
#include <cmath>

using namespace Ubpa;

Sphere::Sphere(size_t n)
	: Shape((n + 1)*(n + 1), 2 * n*n) {
	normalArr = vertexArr;
	texcoordArr = new Array2D<float>((n + 1)*(n + 1), 2);
	indexArr = new Array2D<size_t>(2 * n*n, 3);
	//----------
	float inc = 1.0f / n;
	for (int i = 0; i <= n; i++) {
		float u = inc * i;
		for (int j = 0; j <= n; j++) {
			float v = inc * j;
			float theta = PI * (1-u);
			float phi = 2 * PI * v;
			vertexArr->At(i*(n + 1) + j, 0) = sinf(theta) * sinf(phi);
			vertexArr->At(i*(n + 1) + j, 1) = cosf(theta);
			vertexArr->At(i*(n + 1) + j, 2) = sinf(theta) * cosf(phi);
			texcoordArr->At(i*(n + 1) + j, 0) = v;
			texcoordArr->At(i*(n + 1) + j, 1) = u;
		}
	}
	//------------
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			indexArr->At(2 * (i*n + j), 0) = i * (n + 1) + j;
			indexArr->At(2 * (i*n + j), 1) = i * (n + 1) + j + 1;
			indexArr->At(2 * (i*n + j), 2) = (i + 1) * (n + 1) + j;
			indexArr->At(2 * (i*n + j) + 1, 0) = (i + 1) * (n + 1) + j + 1;
			indexArr->At(2 * (i*n + j) + 1, 1) = i * (n + 1) + j + 1;
			indexArr->At(2 * (i*n + j) + 1, 2) = (i + 1) * (n + 1) + j;
		}
	}
}

Sphere::~Sphere() {
	normalArr = nullptr;
	delete texcoordArr;
	texcoordArr = nullptr;
	delete normalArr;
	normalArr = nullptr;
}

float * Sphere::GetNormalArr() {
	if (normalArr == nullptr)
		return nullptr;

	return normalArr->GetData();

}

float * Sphere::GetTexcoordArr() {
	if (texcoordArr == nullptr)
		return nullptr;

	return texcoordArr->GetData();
}

size_t * Sphere::GetIndexArr() {
	if (indexArr == nullptr)
		return nullptr;

	return indexArr->GetData();
}

size_t Sphere::GetNormalArrSize() {
	return normalArr->GetMemSize();
}

size_t Sphere::GetTexcoordArrSize() {
	return texcoordArr->GetMemSize();
}

size_t Sphere::GetIndexArrSize() {
	return indexArr->GetMemSize();
}
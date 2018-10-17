#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <Utility/Shape.h>
#include <Utility/Array2D.h>

namespace Ubpa {
	class Sphere : public Shape {
	public:
		Sphere(size_t n);
		virtual ~Sphere();
		float * GetNormalArr();
		float * GetTexcoordArr();
		size_t * GetIndexArr();
		size_t GetNormalArrSize();
		size_t GetTexcoordArrSize();
		size_t GetIndexArrSize();
	protected:
		Array2D<float> * normalArr;
		Array2D<float> * texcoordArr;
		Array2D<size_t> * indexArr;
	};
}

#endif // !_SPHERE_H_

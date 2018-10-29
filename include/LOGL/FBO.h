#ifndef _FBO_H_
#define _FBO_H_

namespace LOGL {
	class FBO {
	public:
		enum ENUM_TYPE
		{
			ENUM_TYPE_BASIC
		};

		FBO(size_t width, size_t height, ENUM_TYPE type = ENUM_TYPE_BASIC);
		
		size_t GetID() const;
		size_t GetColorBufferID() const;
		size_t IsValid() const;
	private:
		bool GenFBO_BASIC(size_t width, size_t height);


		FBO(const FBO & fbo) = delete;
		FBO & operator=(const FBO & fbo) = delete;

		ENUM_TYPE type;
		size_t ID;
		size_t colorBufferID;
		bool isValid;
	};
}

#endif // !_FBO_H_

#ifndef _FBO_H_
#define _FBO_H_

namespace LOGL {
	class FBO {
	public:
		enum ENUM_TYPE
		{
			ENUM_TYPE_BASIC,
			ENUM_TYPE_MSAA,
			ENUM_TYPE_COLOR
		};
		enum ENUM_PASS_TYPE {
			ENUM_PASS_COLOR
		};

		FBO(size_t width, size_t height, ENUM_TYPE type = ENUM_TYPE_BASIC);
		
		bool PassTo(const FBO & fbo, ENUM_PASS_TYPE passType = ENUM_PASS_COLOR) const;
		bool Use();
		static void UseDefault();
		size_t GetID() const;
		size_t GetColorBufferID() const;
		bool IsValid() const;
	private:
		bool GenFBO_BASIC(size_t width, size_t height);
		bool GenFBO_MSAA(size_t width, size_t height);
		bool GenFBO_COLOR(size_t width, size_t height);

		bool IsComplete() const;

		FBO(const FBO & fbo) = delete;
		FBO & operator=(const FBO & fbo) = delete;

		ENUM_TYPE type;
		size_t ID;
		size_t colorBufferID;
		size_t width;
		size_t height;
		bool isValid;
	};
}

#endif // !_FBO_H_

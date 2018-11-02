#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <Utility/Image.h>
#include <vector>
#include <string>

namespace LOGL {
	class Texture {
	public:
		enum ENUM_TYPE
		{
			ENUM_TYPE_NOT_VALID,
			ENUM_TYPE_2D,
			ENUM_TYPE_CUBE_MAP,
		};

		Texture(size_t ID = 0, ENUM_TYPE type = ENUM_TYPE_2D);
		Texture(const std::vector<std::string> & skybox);
		Texture(const std::string & path, bool flip = false, bool gammaCorrection = false);

		bool Load(const std::vector<std::string> & skybox);
		bool Load(const std::string & path, bool flip = false, bool gammaCorrection = false);

		bool Use(size_t id = 0);
		void UnBind();

		size_t GetID() const;
		bool IsValid() const;

	private:
		static size_t Type2GL(ENUM_TYPE type);

		size_t ID;
		ENUM_TYPE type;
	};
}

#endif // !_TEXTURE_H_

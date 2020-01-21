#ifndef TEXTURE_H
#define TEXTURE_H

namespace Subsystems{
	namespace TextureSystem{
		class Texture{
		public:
			Texture();
			ALLEGRO_BITMAP* textureToBitmap();
			Subsystems::TextureSystem::Texture& textureToReference();

		private:

		};
	}
}

#endif // TEXTURE_H

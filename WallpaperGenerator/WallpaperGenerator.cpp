#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "WallpaperSynthesis.hpp"
extern "C" {
	#include "log.h"
}

FT_Library library;
FT_Face face;
FT_Error error = FT_Init_FreeType( &library );

using namespace YMC::WallpaperGenerator;

int main() {
	printf("Wallpaper Generator\n");

	if ( error ) {
		printf("An Error Occurred.\n");
	}

	log_trace("hello world");

    return 0;
}

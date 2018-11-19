
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on november of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Adapted to Soil2 by Ilyass Sofi on November of 2018

#include "View.hpp"

extern "C"
{
	#include <SOIL2.h>
}

namespace example
{

    View::View(size_t width, size_t height, const char * image_path)
    :
        width       (width ),
        height      (height),
        pixel_buffer(width, height),
        rasterizer  (pixel_buffer )
    {
        if (image_path != 0)
        {
            load_image (image_path);
        }
    }

    bool View::load_image (const char * image_path)
    {	
		
		int iwidth, iheight, channels;
		unsigned char* loaded_image = SOIL_load_image(image_path, &iwidth, &iheight, &channels, SOIL_LOAD_RGBA);

		if (0 != loaded_image)
		{	
			image.reset(new Pixel_Buffer(iwidth, iheight));

			Pixel_Buffer::Pixel * loaded_image_pixels = reinterpret_cast< Pixel_Buffer::Pixel * >(loaded_image);
			Pixel_Buffer::Pixel * loaded_image_pixels_end = loaded_image_pixels + iwidth * iheight;
			Pixel_Buffer::Pixel * image_buffer_pixels = image->pixels();

			while (loaded_image_pixels <  loaded_image_pixels_end)
			{
				*image_buffer_pixels++ = *loaded_image_pixels++;
			}

			rasterizer.clear(255, 255, 255);
			SOIL_free_image_data(loaded_image);
			return true;
		}
		else
		{	
			rasterizer.clear(255, 0, 0);
			return false;
		}

    }

    void View::paint ()
    {
        if (image.get ())
        {
            rasterizer.draw_image (10, 10, *image);
        }

        pixel_buffer.blit_to_window ();
    }

}


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
		// Declaring image data variables
		int width, height, channels;
		// Loading image
		unsigned char* loaded_image = SOIL_load_image(image_path, &width, &height, &channels, SOIL_LOAD_RGBA);

		// Check if the image has loaded correctly
		if (0 != loaded_image)
		{	
			// Create a new pixel buffer with image data
			image.reset(new Pixel_Buffer(width, height));

			// Pixel buffer for the loaded image
			Pixel_Buffer::Pixel * loaded_image_pixels = reinterpret_cast< Pixel_Buffer::Pixel * >(loaded_image);
			// Last pixel buffer
			Pixel_Buffer::Pixel * loaded_image_pixels_end = loaded_image_pixels + width * height;
			// Take the image pixels into the buffer
			Pixel_Buffer::Pixel * image_buffer_pixels = image->pixels();

			// Loop through pixels and add them to the buffer
			while (loaded_image_pixels <  loaded_image_pixels_end)
			{
				*image_buffer_pixels++ = *loaded_image_pixels++;
			}

			// Clear background
			rasterizer.clear(255, 255, 255);
			// Free data for further use
			SOIL_free_image_data(loaded_image);

			// Image has loaded correctly
			return true;
		}
		else
		{	
			// Image has not loaded, clear background to red
			rasterizer.clear(255, 0, 0);

			// Image has not loaded correctly
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

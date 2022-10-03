#pragma once

#include "color.hpp"
#include "opengl_object.hpp"
#include <iostream>
#include <vector>

/**
 * The base class for representing textures.
 * <p>
 * Note that this class requires OpenGL 4.5., for older OpenGL versions use the @p Texture_3_3 class available in PB009 module.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 */
class Texture : public OpenGLObject {
    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The texture width. */
    int width;

    /** The texture height. */
    int height;

    /** The sized internal format used to store texture image data. */
    GLenum internal_format;

    /** The format of the pixel data. */
    GLenum format;

    /** The type of the pixel data. Current implementation supports only GL_FLOAT. */
    GLenum type;

    /** The number of color channels. */
    int nrChannels = 4;

    /** The flag determining if the OpenGL counterparts should be initialized in the constructor. */
    bool cpu_only;

    /**
     * The array of pixels - the array is organized as list of rows with 'nrChannels' values per pixel. See
     * {@link get_index} method for more details.
     */
    std::vector<float> texture_data;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
     /** Constructs a new @link Texture. */
     Texture() : Texture(0,0){}

    /**
     * Constructs a new @link Texture with specified size and default formats.
     *
     * @param 	width   	The texture width.
     * @param 	height  	The texture height.
     * @param 	cpu_only	The flag determining if the texture will be CPU only (should be @p true for tests as they do
     * 						not have OpenGL context).
     */
    Texture(int width, int height, bool cpu_only = false) : Texture(width, height, GL_RGBA8, GL_RGBA, cpu_only) {}

    /**
     * Constructs a new custom @link Texture and initialized the OpenGL counterpart.
     *
     * @param 	width		   	The texture width.
     * @param 	height		   	The texture height.
     * @param 	internal_format	The internal texture format that specifies the number of color components in the texture.
     * @param 	format		   	The texture format that specifies the format of the pixel data.
     * @param 	cpu_only	   	The flag determining if the texture will be CPU only (should be @p true for tests as they
     * 							do not have OpenGL context).
     */
    Texture(int width, int height, GLint internal_format, GLenum format, bool cpu_only = false)
        : OpenGLObject(GL_TEXTURE_2D), width(width), height(height), internal_format(internal_format), format(format),
          type(GL_FLOAT), cpu_only(cpu_only) {
        // Creates the CPU representation of the data.
        texture_data = std::vector<float>(width * height * nrChannels);
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                set_pixel_color(x, y, Color::WHITE);
            }
        }

        // Determine the number or channels used by the texture based on the specified format.
        switch (format) {
        case GL_DEPTH_COMPONENT:
            nrChannels = 1;
            break;
        case GL_RGBA:
            nrChannels = 4;
            break;
        default:
            std::cout << "The texture class currently supports only GL_DEPTH_COMPONENT and GL_RGBA formats." << std::endl;
        }

        // Creates the GPU representation of the data.
        if (width > 0 && height > 0 && !cpu_only) {
            glCreateTextures(target, 1, &opengl_object);
            glTextureStorage2D(opengl_object, 1, internal_format, width, height);
        }
    }

    /**
     * Copy constructor. Performs a deep copy of the texture.
     *
     * @param 	other	The other texture to copy from.
     */
    Texture(const Texture& other)
        : OpenGLObject(GL_TEXTURE_2D), width(other.width), height(other.height), internal_format(other.internal_format),
          format(other.format), type(GL_FLOAT), cpu_only(other.cpu_only), texture_data(other.texture_data) {

        if (width > 0 && height > 0 && !cpu_only) {
            glCreateTextures(target, 1, &opengl_object);
            glTextureStorage2D(opengl_object, 1, internal_format, width, height);
            update_opengl_data();
        }
    }

    /**
     * Copy assignment. Performs a deep copy of the texture.
     *
     * @param 	other	The other texture to assign to this object.
     *
     * @return	A shallow copy of this object.
     */
    Texture& operator=(Texture other) {
        swap(*this, other);
        return *this;
    }

    /**
     * Move constructor. Moves the Texture to the new object. Zeroes out the old object so that
     * OpenGL's destructor does nothing.
     *
     * @param 	other The other texture.
     */
    Texture(Texture&& other) : Texture(0, 0) { swap(*this, other); }

    /**
     * The custom swap method that exchanges the values of fields of two textures.
     * @param first The first texture.
     * @param second The second texture.
     */
    friend void swap(Texture& first, Texture& second) noexcept {
        using std::swap;

        swap(first.opengl_object, second.opengl_object);
        swap(first.texture_data, second.texture_data);
        swap(first.width, second.width);
        swap(first.height, second.height);
        swap(first.internal_format, second.internal_format);
        swap(first.format, second.format);
        swap(first.type, second.type);
        swap(first.nrChannels, second.nrChannels);
        swap(first.cpu_only, second.cpu_only);
    }

    /**
     * Destroys this @link Texture.
     */
    virtual ~Texture() {
        if (!cpu_only) {
            glDeleteTextures(1, &opengl_object);
        }
    }

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

    /**
     * Bind the texture to the specified texture unit.
     *
     * @param 	unit	The texture unit to which the texture object will be bound to.
     */
   virtual void bind(GLuint unit) const {
         glBindTextureUnit(unit, opengl_object);
     }

    /** @copydoc OpenGLObject::update_opengl_data */
   virtual void update_opengl_data() const override {
        if (opengl_object != 0) {
            glTextureSubImage2D(opengl_object, 0, 0, 0, width, height, format, type, texture_data.data());
        } else {
            // fail silently
        }
    }

    /** Copies the data from GPU to CPU. */
    void update_cpu_data() const {
        if (opengl_object != 0) {
            glGetTextureImage(opengl_object, 0, format, type, width * height * nrChannels * sizeof(float),
                              (void*)&texture_data.data()[0]);
        } else {
            // fail silently
        }
    }

    /**
     * Sets the texture parameters by calling glTextureParameteri.
     *
     * @param 	wrap_s	  	The wrap parameter value for GL_TEXTURE_WRAP_S.
     * @param 	wrap_t	  	The wrap parameter value for GL_TEXTURE_WRAP_T.
     * @param 	min_filter	A filter parameter value for GL_TEXTURE_MIN_FILTER.
     * @param 	mag_filter	A filter parameter value for GL_TEXTURE_MAG_FILTER.
     */
    virtual void set_texture_2d_parameters(GLint wrap_s, GLint wrap_t, GLint min_filter, GLint mag_filter) const {
        if (opengl_object != 0) {
            glTextureParameteri(opengl_object, GL_TEXTURE_WRAP_S, wrap_s);
            glTextureParameteri(opengl_object, GL_TEXTURE_WRAP_T, wrap_t);
            glTextureParameteri(opengl_object, GL_TEXTURE_MIN_FILTER, min_filter);
            glTextureParameteri(opengl_object, GL_TEXTURE_MAG_FILTER, mag_filter);
        } else {
            // fail silently
        }
    }

  private:
    /**
     * Returns the first index (i.e., red channel) of the pixel at x,y in the data array. Use: getIndex(x,y) to get
     * index for red value of pixel at x,y getIndex(x,y) + 1 to get index for green value of pixel at x,y getIndex(x,y)
     * + 2 to get index for blue value of pixel at x,y
     *
     * @param 	x	The x coordinate in the texture.
     * @param 	y	The y coordinate in the texture.
     *
     * @return	The corresponding index in the data array.
     */
    int get_index(int x, int y) const { return (y * width + x) * nrChannels; }

    // ----------------------------------------------------------------------------
    // Getters & Setters
    // ----------------------------------------------------------------------------
  public:
    /**
     * Returns the texture width.
     *
     * @return The texture width.
     */
    int get_width() const { return this->width; }

    /**
     * Returns the texture height.
     *
     * @return The texture height.
     */
    int get_height() const { return this->height; }

    /**
     * Sets the color of a pixel identified by the x (column) and y (row) coordinates.
     *
     * @param 	x	 	The X-coordinate of the pixel to be changed.
     * @param 	y	 	The Y-coordinate of the pixel to be changed.
     * @param 	color	The new color.
     */
    void set_pixel_color(int x, int y, const Color& color) {
        if (x < 0 || x >= get_width() || y < 0 || y >= get_height()) {
            std::cout << "The coordinates " << x << ", " << y << " are out of bounds - the color was not modified." << std::endl;
            return;
        }

        const size_t index = this->get_index(x, y);

        if (nrChannels >= 1) {
            texture_data[index] = color.r;
        }
        if (nrChannels >= 2) {
            texture_data[index + 1] = color.g;
        }
        if (nrChannels >= 3) {
            texture_data[index + 2] = color.b;
        }
        if (nrChannels == 4) {
            texture_data[index + 3] = color.a;
        }
    }

    /**
     * Sets the intensity (i.e., monotone color) of a pixel identified by the x (column) and y (row)
     * coordinates.
     *
     * @param 	x		 	The X-coordinate of the pixel to be changed.
     * @param 	y		 	The Y-coordinate of the pixel to be changed.
     * @param 	intensity	The intensity.
     */
    void set_pixel_gray_scale(int x, int y, float intensity) {
        if (x < 0 || x >= this->get_width() || y < 0 || y >= this->get_height()) {
            std::cout << "The coordinates " << x << ", " << y << " are out of bounds - the color was not modified." << std::endl;
        }

        const size_t index = this->get_index(x, y);
        if (nrChannels >= 1) {
            texture_data[index] = intensity;
        }
        if (nrChannels >= 2) {
            texture_data[index + 1] = intensity;
        }
        if (nrChannels >= 3) {
            texture_data[index + 2] = intensity;
        }
        if (nrChannels == 4) {
            texture_data[index + 3] = 1;
        }
    }

    /**
     * Returns the current color of the raster pixel identified by the x (column) and y (row)
     * coordinates.
     *
     * @param 	x	The X-coordinate of the requested pixel.
     * @param 	y	The Y-coordinate of the requested pixel
     *
     * @return	The pixel color.
     */
    Color get_pixel_color(int x, int y) const {
        if (x < 0 || x >= this->get_width() || y < 0 || y >= this->get_height()) {
            std::cout << "The coordinates " << x << ", " << y << " are out of bounds - black will be returned." << std::endl;
            return Color::BLACK;
        }

        Color color;
        get_pixel_color_fast(x, y, color.r, color.g, color.b, color.a);
        return color;
    }

    void get_pixel_color_fast(int x, int y, float& r, float& g, float& b, float& a) const {
        const size_t index = this->get_index(x, y);

        // TODO consider using format variable instead of nrChannels
        switch (nrChannels) {
        case 1:
            r = texture_data[index];
            g = texture_data[index];
            b = texture_data[index];
            a = 1.0;
            break;
        case 3:
            r = texture_data[index];
            g = texture_data[index + 1];
            b = texture_data[index + 2];
            a = 1.0;
            break;
        case 4:
            r = texture_data[index];
            g = texture_data[index + 1];
            b = texture_data[index + 2];
            a = texture_data[index + 3];
            break;
        default:
            std::cout << "Cannot retrieve the pixel color, the texture has unsupported number of channels." << std::endl;
        }
    }

    /**
     * Returns the current intensity (i.e., monotone color) of the raster pixel identified by the x (column) and y (row)
     * coordinates.
     *
     * @param 	x	The X-coordinate of the requested pixel.
     * @param 	y	The Y-coordinate of the requested pixel.
     *
     * @return	The pixel gray scale.
     */
    float get_pixel_gray_scale(int x, int y) const {
        if (x < 0 || x >= this->get_width() || y < 0 || y >= this->get_height()) {
            std::cout << "The coordinates " << x << ", " << y << " are out of bounds - zero will be returned." << std::endl;
            return 0;
        }

        Color color = this->get_pixel_color(x, y);
        return (color.r + color.g + color.b) / 3.0f;
    }
};

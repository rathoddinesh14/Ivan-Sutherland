#ifndef __RENDER_H__
#define __RENDER_H__

#include <GL/glew.h>
#include <vector>
#include "include/common/math_utils.h"
#include "include/common/shader.h"
#include "scene.h"

/**
 * @brief Ray tracing render class.
 * @author Rathod Dinesh
 */
class RayTraceRender{

    // vao and texture id
    GLuint vao;
    GLuint texture;

    // shader program
    GLuint program;

    // shader uniforms
    GLint u_textureUnit;

    // window size
    int width, height;

    public:
        /**
         * @brief Constructor.
         */
        RayTraceRender(int width, int height);

        /**
         * @brief Destructor.
         */
        ~RayTraceRender();

        /**
         * @brief Render the scene.
         */
        void render();

        /**
         * @brief load texture/copy image data to texture
         */
        void loadTexture(std::vector<Vector4f> &image);
};

#endif // __RENDER_H__
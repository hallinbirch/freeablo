#include <glad/glad.h>
#include <render/OpenGL/renderinstanceopengl.h>
#include <render/OpenGL/vertexarrayobjectopengl.h>

namespace Render
{
#ifdef DEBUG_GRAPHICS
    void gladDebugPostCallCallback(const char* name, void*, int, ...)
    {
        GLenum error_code;
        error_code = glad_glGetError();

        if (error_code != GL_NO_ERROR)
        {
            fprintf(stderr, "ERROR %d in %s\n", error_code, name);
            DEBUG_BREAK;
        }
    }
#endif

    RenderInstanceOpenGL::RenderInstanceOpenGL(SDL_Window* window)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        mGlContext = SDL_GL_CreateContext(window);

        if (!gladLoadGL())
            message_and_abort("gladLoadGL failed");

#ifdef DEBUG_GRAPHICS
        glad_set_post_callback(gladDebugPostCallCallback);

        // Ensure VSYNC is disabled to get actual FPS.
        SDL_GL_SetSwapInterval(0);
#endif

        // Check opengl version is at least 3.3.
        const GLubyte* glVersion(glGetString(GL_VERSION));
        int major = glVersion[0] - '0';
        int minor = glVersion[2] - '0';
        if (major < 3 || (major == 3 && minor < 3))
            message_and_abort_fmt("ERROR: Minimum OpenGL version is 3.3. Your current version is %d.%d\n", major, minor);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
    }

    RenderInstanceOpenGL::~RenderInstanceOpenGL() { SDL_GL_DeleteContext(mGlContext); }

    std::unique_ptr<Buffer> RenderInstanceOpenGL::createBuffer(size_t sizeInBytes) { return std::unique_ptr<Buffer>(new BufferOpenGL(sizeInBytes)); }

    std::unique_ptr<VertexArrayObject> RenderInstanceOpenGL::createVertexArrayObject(std::vector<size_t> bufferSizeCounts,
                                                                                     std::vector<NonNullConstPtr<VertexLayout>> bindings,
                                                                                     size_t indexBufferSizeInElements)
    {
        return std::unique_ptr<VertexArrayObject>(
            new VertexArrayObjectOpenGL(*this, std::move(bufferSizeCounts), std::move(bindings), indexBufferSizeInElements));
    }
}
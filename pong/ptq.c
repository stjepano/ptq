#include "ptq.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *MemAlloc(u64 n)
{
    return malloc(n);
}

void *MemRealloc(void *ptr, u64 n)
{
    return realloc(ptr, n);
}

void MemFree(void *ptr)
{
    free(ptr);
}

void MemZero(void *ptr, u64 n)
{
    memset(ptr, 0, n);
}

#define DECLARE_GL_FUNC(name, ret, ...)                                        \
    typedef ret (*PFN_##name)(__VA_ARGS__);                                    \
    PFN_##name ptq_##name
#define LOAD_GL_FUNC(name)                                                     \
    ptq_##name = (PFN_##name)glfwGetProcAddress(#name);                        \
    assert(ptq_##name != nullptr)

DECLARE_GL_FUNC(glClearBufferfv, void, GLenum, GLint, const GLfloat *);
DECLARE_GL_FUNC(glCreateShader, GLuint, GLenum);
DECLARE_GL_FUNC(glShaderSource, void, GLuint shader, GLsizei count,
                const GLchar **string, const GLint *length);
DECLARE_GL_FUNC(glCompileShader, void, GLuint);
DECLARE_GL_FUNC(glCreateProgram, GLuint, void);
DECLARE_GL_FUNC(glAttachShader, void, GLuint, GLuint);
DECLARE_GL_FUNC(glLinkProgram, void, GLuint);
DECLARE_GL_FUNC(glGenVertexArrays, void, GLsizei, GLuint *);
DECLARE_GL_FUNC(glBindVertexArray, void, GLuint);
DECLARE_GL_FUNC(glDeleteVertexArrays, void, GLsizei, const GLuint *);
DECLARE_GL_FUNC(glUseProgram, void, GLuint);
DECLARE_GL_FUNC(glDrawArrays, void, GLenum, GLint, GLsizei);
DECLARE_GL_FUNC(glGetShaderiv, void, GLuint, GLenum, GLint *);
DECLARE_GL_FUNC(glGetShaderInfoLog, void, GLuint, GLsizei, GLsizei *, GLchar *);
DECLARE_GL_FUNC(glGetProgramiv, void, GLuint, GLenum, GLint *);
DECLARE_GL_FUNC(glGetProgramInfoLog, void, GLuint, GLsizei, GLsizei *,
                GLchar *);
DECLARE_GL_FUNC(glDeleteShader, void, GLuint);
DECLARE_GL_FUNC(glDeleteProgram, void, GLuint);

static void loadGl()
{
    LOAD_GL_FUNC(glClearBufferfv);
    LOAD_GL_FUNC(glCreateShader);
    LOAD_GL_FUNC(glShaderSource);
    LOAD_GL_FUNC(glCompileShader);
    LOAD_GL_FUNC(glAttachShader);
    LOAD_GL_FUNC(glCreateProgram);
    LOAD_GL_FUNC(glLinkProgram);
    LOAD_GL_FUNC(glBindVertexArray);
    LOAD_GL_FUNC(glGenVertexArrays);
    LOAD_GL_FUNC(glDeleteVertexArrays);
    LOAD_GL_FUNC(glUseProgram);
    LOAD_GL_FUNC(glDrawArrays);
    LOAD_GL_FUNC(glGetShaderiv);
    LOAD_GL_FUNC(glGetShaderInfoLog);
    LOAD_GL_FUNC(glGetProgramiv);
    LOAD_GL_FUNC(glGetProgramInfoLog);
    LOAD_GL_FUNC(glDeleteShader);
    LOAD_GL_FUNC(glDeleteProgram);
}

static const char *vertex_shader_text =
    "#version 330 core\n"
    "vec2 positions[3] = vec2[3](\n"
    "    vec2(-1.0, 3.0),\n"
    "    vec2(-1.0, -1.0),\n"
    "    vec2(3.0, -1.0));\n"
    "vec2 tex_coords[3] = vec2[3](\n"
    "    vec2(0.0, -1.0),\n"
    "    vec2(0.0, 1.0),\n"
    "    vec2(2.0, 1.0));\n"
    "out vec2 v_tex_coord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(positions[gl_VertexID], 0, 1);\n"
    "    v_tex_coord = tex_coords[gl_VertexID];\n"
    "}\n";

static const char *fragment_shader_text =
    "#version 330 core\n"
    "out vec4 fragment;\n"
    "uniform sampler2D u_texture;\n"
    "in vec2 v_tex_coord;\n"
    "void main()\n"
    "{\n"
    "    fragment = texture(u_texture, v_tex_coord);\n"
    "}\n";

static void errorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW error, code %d, description: %s\n", error,
            description);
}

static GLuint compileGLProgram()
{
    GLuint vshader = ptq_glCreateShader(GL_VERTEX_SHADER);
    ptq_glShaderSource(vshader, 1, &vertex_shader_text, nullptr);
    ptq_glCompileShader(vshader);
    GLint compile_status;
    ptq_glGetShaderiv(vshader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        GLsizei info_length = 0;
        GLchar info[1024];
        ptq_glGetShaderInfoLog(vshader, 1024, &info_length, info);
        fprintf(stderr, "Vertex shader compilation failed: %s\n", info);
        return 0;
    }

    GLuint fshader = ptq_glCreateShader(GL_FRAGMENT_SHADER);
    ptq_glShaderSource(fshader, 1, &fragment_shader_text, nullptr);
    ptq_glCompileShader(fshader);
    ptq_glGetShaderiv(fshader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        GLsizei info_length = 0;
        GLchar info[1024];
        ptq_glGetShaderInfoLog(fshader, 1024, &info_length, info);
        fprintf(stderr, "Fragment shader compilation failed: %s\n", info);
        return 0;
    }

    GLuint program = ptq_glCreateProgram();
    ptq_glAttachShader(program, vshader);
    ptq_glAttachShader(program, fshader);
    ptq_glLinkProgram(program);
    GLint link_status;
    ptq_glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        GLsizei info_length = 0;
        GLchar info[1024];
        ptq_glGetProgramInfoLog(program, 1024, &info_length, info);
        fprintf(stderr, "Program link failed: %s\n", info);
        return 0;
    }
    ptq_glDeleteShader(vshader);
    ptq_glDeleteShader(fshader);
    return program;
}

#define CHECK_GL_ERROR()                                                       \
    do                                                                         \
    {                                                                          \
        GLenum glerr = glGetError();                                           \
        if (glerr != GL_NO_ERROR)                                              \
        {                                                                      \
            fprintf(stderr, "Failed to create/bind vertex array\n");           \
            return ERROR_FAILED;                                               \
        }                                                                      \
    } while (0)

GLFWwindow *g_window = nullptr;
surface_t *g_surface = nullptr;

static GLuint s_program = 0;
static GLuint s_vao = 0;
static GLuint s_texture = 0;

static audio_mixer_fn_t s_audio_mixer_fun;
static ma_device s_device;

static void maDataCallback(ma_device *device, void *output, const void *input,
                           ma_uint32 frame_count)
{
    if (s_audio_mixer_fun)
    {
        s_audio_mixer_fun((f32 *)output, (u32)frame_count, device->pUserData);
    }
}

error_t PlatformInit(const platform_config_t *config)
{

    s_audio_mixer_fun = config->audio_mixer_function;

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
    {
        fprintf(stderr, "glfwInit failed\n");
        return ERROR_FAILED;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window = glfwCreateWindow(config->window_width, config->window_height,
                                config->window_title, nullptr, nullptr);
    if (g_window == nullptr)
    {
        fprintf(stderr, "Failed to create window!\n");
        return ERROR_FAILED;
    }

    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(1);

    loadGl();
    glGetError();

    ptq_glGenVertexArrays(1, &s_vao);
    ptq_glBindVertexArray(s_vao);

    CHECK_GL_ERROR();

    s_program = compileGLProgram();
    if (s_program == 0)
    {
        fprintf(stderr, "Failed to compile GL program\n");
        return ERROR_FAILED;
    }

    ptq_glUseProgram(s_program);

    CHECK_GL_ERROR();

    glGenTextures(1, &s_texture);

    CHECK_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, s_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, config->surface_width,
                 config->surface_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    CHECK_GL_ERROR();

    g_surface = MemAlloc(sizeof(surface_t));
    assert(g_surface != nullptr);
    MemZero(g_surface, sizeof(surface_t));

    g_surface->width = config->surface_width;
    g_surface->height = config->surface_height;
    g_surface->pixels =
        MemAlloc(g_surface->width * g_surface->height * sizeof(u32));
    assert(g_surface->pixels != nullptr);

    ma_device_config ma_config;
    ma_config = ma_device_config_init(ma_device_type_playback);
    ma_config.playback.format = ma_format_f32;
    ma_config.playback.channels = 2;
    ma_config.sampleRate = (ma_uint32)AUDIO_SAMPLE_RATE;
    ma_config.dataCallback = maDataCallback;
    ma_config.pUserData = nullptr;

    if (ma_device_init(nullptr, &ma_config, &s_device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failed to open playback device!\n");
        return ERROR_FAILED;
    }

    if (ma_device_start(&s_device) != MA_SUCCESS)
    {
        fprintf(stderr, "Failed to start playback device!\n");
        return ERROR_FAILED;
    }

    return ERROR_NO_ERROR;
}

GLfloat s_clear_color[4] = {0, 0, 0, 1.0f};
void PlatformPresent()
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_surface->width, g_surface->height,
                    GL_RGBA, GL_UNSIGNED_BYTE, g_surface->pixels);

    i32 framebuffer[2];
    glfwGetFramebufferSize(g_window, &framebuffer[0], &framebuffer[1]);
    glViewport(0, 0, framebuffer[0], framebuffer[1]);
    ptq_glClearBufferfv(GL_COLOR, 0, s_clear_color);

    if (framebuffer[0] >= g_surface->width &&
        framebuffer[1] >= g_surface->height)
    {
        i32 sx = framebuffer[0] / g_surface->width;
        i32 sy = framebuffer[1] / g_surface->height;

        i32 scale = MIN(sx, sy);

        i32 x = (framebuffer[0] - (g_surface->width * scale)) / 2;
        i32 y = (framebuffer[1] - (g_surface->height * scale)) / 2;
        glViewport(x, y, g_surface->width * scale, g_surface->height * scale);

        ptq_glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(g_window);
}

void PlatformTerminate()
{
    ma_device_uninit(&s_device);
    MemFree(g_surface->pixels);
    MemFree(g_surface);
    glDeleteTextures(1, &s_texture);
    ptq_glDeleteVertexArrays(1, &s_vao);
    ptq_glDeleteProgram(s_program);
    glfwTerminate();
}

void SurfaceFillRect(surface_t *s, const irect_t *rect, u32 color)
{
    assert(s != nullptr && rect != nullptr);
    irect_t trect = {0};
    irect_t surface_rect = {0, 0, s->width, s->height};
    IRectIRectIntersection(&trect, rect, &surface_rect);

    if (trect.w <= 0 || trect.h <= 0)
    {
        return;
    }

    for (i32 row = 0; row < trect.h; row++)
    {
        for (i32 col = 0; col < trect.w; col++)
        {
            u32 *pixel =
                (u32 *)SurfacePixelPtr(s, col + trect.x, row + trect.y);
            *pixel = color;
        }
    }
}

void IRectIRectIntersection(irect_t *out, const irect_t *a, const irect_t *b)
{
    out->x = MAX(a->x, b->x);
    out->y = MAX(a->y, b->y);
    out->w = MIN(a->x + a->w, b->x + b->w) - out->x;
    out->h = MIN(a->y + a->h, b->y + b->h) - out->y;
}

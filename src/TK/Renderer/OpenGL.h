#pragma once



#include <TK/Renderer.h>
#include <TK/Range.h>
#include <glad/glad.h>





typedef struct {
    TK_vec3f position;
    TK_vec3f normal;
    TK_vec2f texureCoordinates;
} TK__GL_Vertex;




struct TK_Renderer
{
    TK_Window *window;
    TK_Allocator *allocator;
    TK_ErrorQueue *errorQueue;
    
    GLuint sceneDepthFramebuffer;
    GLuint sceneDepthTexture;
    
    GLuint basicVertShader;
    
    GLuint positionFragShader;
    GLuint normalFragShader;
    GLuint uvFragShader;
    GLuint flatFragShader;
    GLuint lightFragShader;
    
    GLuint positionProgram;
    GLuint normalProgram;
    GLuint uvProgram;
    GLuint lightProgram;
    
    GLuint cubeMeshVertexArray;
};



typedef enum
{
    TK_RENDER_SCENE_POSITION,
    TK_RENDER_SCENE_COLOR,
    TK_RENDER_SCENE_DEPTH,
    TK_RENDER_SCENE_UV,
    TK_RENDER_SCENE_NORMAL,
    TK_RENDER_SCENE_LIGHT,
} TK_RenderSceneAction;



typedef
    struct TK_RenderSceneInfo TK_RenderSceneInfo;
    struct TK_RenderSceneInfo
{
    TK_RenderSceneAction action;
    float aspect;
    float fov;
    TK_rangef zrange;
    TK_vec3f scale;
    TK_vec3f rotation;
    TK_vec3f position;
    TK_vec3f camera_position;
    TK_vec3f camera_direction;
    TK_vec3f light_position;
    TK_vec3f light_direction;
};
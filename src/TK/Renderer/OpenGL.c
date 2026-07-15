#include <TK/Renderer/OpenGL.h>
#include <TK/Renderer.h>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <cglm/struct.h>



TK_static_assert(sizeof(GLfloat) == sizeof(float));
TK_static_assert(sizeof(GLuint)  == sizeof(uint));

enum {
    CUBE_FACE_COUNT         = 6,
    CUBE_FACE_VERTS_COUNT   = 4,
    CUBE_FACE_TRIS_COUNT    = 2,
    CUBE_VERTEX_COUNT       = CUBE_FACE_COUNT * CUBE_FACE_VERTS_COUNT,
    CUBE_INDEX_COUNT        = CUBE_FACE_COUNT * CUBE_FACE_TRIS_COUNT * 3,
    SCREEN_MESH_INDEX_COUNT = 6,
    SCREEN_MESH_VERT_COUNT  = 4,
    SHADOW_WIDTH            = 1024,
    SHADOW_HEIGHT           = 1024,
};





void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (
        0
        || id == 131169 // TODO: replace magic constants
        || id == 131185
        || id == 131218
        || id == 131204
		|| id == 131222
        || type == GL_DEBUG_TYPE_PERFORMANCE
	) return;

	printf("+- OpenGL ----------------------------------------------------------------------\n| \n");
	printf("| Debug message (%u): %s\n", id, message);

    printf("| ");
	switch (source)
	{
	    case GL_DEBUG_SOURCE_API:             printf("Source: API");             break;
	    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System");   break;
	    case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
	    case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party");     break;
	    case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application");     break;
	    case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other");           break;
	    default:                              printf("Source: ???");             break;
	} putchar('\n');

    printf("| ");
	switch (type)
	{
	    case GL_DEBUG_TYPE_ERROR:               printf("Type: Error");                break;
	    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
	    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour");  break;
	    case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability");          break;
	    case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance");          break;
	    case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker");               break;
	    case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group");           break;
	    case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group");            break;
	    case GL_DEBUG_TYPE_OTHER:               printf("Type: Other");                break;
	    default:                                printf("Type: ???");                  break;
	} putchar('\n');

    printf("| ");
	switch (severity)
	{
        case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high");         break;
        case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium");       break;
        case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low");          break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
        default:                             printf("Severity: ???");          break;
	} putchar('\n');
    
    printf("| \n+-------------------------------------------------------------------------------\n");
    
    if (severity == GL_DEBUG_SEVERITY_HIGH)
        exit(1);
}





TK_Renderer *TK_CreateRendererOPT(TK_RendererCreateInfo createInfo)
{
    TK_Renderer *renderer;
    {
        if (!createInfo.allocator)
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_ERROR_NO_ALLOCATOR);
            
            return NULL;
        }
        
        if (!createInfo.window)
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_ERROR_NO_WINDOW);
            
            return NULL;
        }
        
        
        
        renderer = TK_allocate(createInfo.allocator, sizeof(*renderer));
        if (!renderer)
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_ERROR_ALLOCATION);
            
            return NULL;
        }
        
        
        
        static bool initOpenGL = true;
        if (initOpenGL && !gladLoadGLLoader(TK_GL_GetProcAddress))
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_StringSliceLiteral("glad failed to load"));
            
            TK_free(createInfo.allocator, renderer);
            
            return NULL;
        }
        initOpenGL = false;
        
        
        
        *renderer = (TK_Renderer){
            .allocator  = createInfo.allocator,
            .errorQueue = createInfo.errorQueue,
            .window     = createInfo.window,
        };
    }
    
    
    {
        
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
    
    
    {
        GLuint sceneDepthTexture = 0;
        glGenTextures(1, &sceneDepthTexture);
        glBindTexture(GL_TEXTURE_2D, sceneDepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        
        GLuint sceneDepthFramebuffer = 0;
        glGenFramebuffers(1, &sceneDepthFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, sceneDepthFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sceneDepthTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        
        renderer->sceneDepthFramebuffer = sceneDepthFramebuffer;
        renderer->sceneDepthTexture = sceneDepthTexture;
    }
    
    
    {
        #define nl "\n"
        {
            TK_StringSlice basicVertShaderSource = TK_StringSliceLiteral(
                "#version 460 core"                                                            nl
                                                                                               nl
                "layout(location=0) in vec3 i_position;"                                       nl
                "layout(location=1) in vec3 i_normal;"                                         nl
                "layout(location=2) in vec2 i_uv;"                                             nl
                                                                                               nl
                "layout(location=0) out vec3 o_position;"                                      nl
                "layout(location=1) out vec3 o_normal;"                                        nl
                "layout(location=2) out vec2 o_uv;"                                            nl
                                                                                               nl
                "layout(location=0) uniform mat4 u_model;"                                     nl
                "layout(location=1) uniform mat4 u_camera;"                                    nl
                "layout(location=2) uniform mat4 u_projection;"                                nl
                                                                                               nl
                "void main()"                                                                  nl
                "{"                                                                            nl
                "    gl_Position = u_projection * u_camera * u_model * vec4(i_position, 1.0);" nl
                "    o_position  = i_position;"                                                nl
                "    o_normal    = i_normal;"                                                  nl
                "    o_uv        = i_uv;"                                                      nl
                "}"
            );
            
            GLuint basicVertShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(basicVertShader, 1, (void *)&basicVertShaderSource.items, (void *)&basicVertShaderSource.length);
            glCompileShader(basicVertShader);
            
            renderer->basicVertShader = basicVertShader;
        }
        
        
        {
            TK_StringSlice positionFragShaderSource = TK_StringSliceLiteral(
                "#version 460 core"                                 nl
                                                                    nl
                "layout(location=0) in vec3 i_position;"            nl
                "layout(location=1) in vec3 i_normal;"              nl
                "layout(location=2) in vec2 i_uv;"                  nl
                                                                    nl
                "layout(location=0) out vec4 o_color;"              nl
                                                                    nl
                "layout(location=0) uniform mat4 u_model;"          nl
                "layout(location=1) uniform mat4 u_camera;"         nl
                "layout(location=2) uniform mat4 u_projection;"     nl
                                                                    nl
                "void main()"                                       nl
                "{"                                                 nl
                "    vec4 world = u_model * vec4(i_position, 1.0);" nl
                "    o_color = vec4(vec3(world.xyz), 1.0);"         nl
                "}"
            );
            
            GLuint positionFragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(positionFragShader, 1, (void *)&positionFragShaderSource.items, (void *)&positionFragShaderSource.length);
            glCompileShader(positionFragShader);
            renderer->positionFragShader = positionFragShader;
            
            GLuint positionProgram = glCreateProgram();
            
            glAttachShader(positionProgram, renderer->basicVertShader);
            glAttachShader(positionProgram, renderer->positionFragShader);
            glLinkProgram(positionProgram);
            glValidateProgram(positionProgram);
            
            glDetachShader(positionProgram, renderer->basicVertShader);
            glDetachShader(positionProgram, renderer->positionFragShader);
            
            renderer->positionProgram = positionProgram;
        }
        
        
        {
            TK_StringSlice normalFragShaderSource = TK_StringSliceLiteral(
                "#version 460 core"                             nl
                                                                nl
                "layout(location=0) in vec3 i_position;"        nl
                "layout(location=1) in vec3 i_normal;"          nl
                "layout(location=2) in vec2 i_uv;"              nl
                                                                nl
                "layout(location=0) out vec4 o_color;"          nl
                                                                nl
                "layout(location=0) uniform mat4 u_model;"      nl
                "layout(location=1) uniform mat4 u_camera;"     nl
                "layout(location=2) uniform mat4 u_projection;" nl
                                                                nl
                "void main()"                                   nl
                "{"                                             nl
                "    o_color = vec4(abs(i_normal), 1.0);"       nl
                "}"
            );
            
            GLuint normalFragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(normalFragShader, 1, (void *)&normalFragShaderSource.items, (void *)&normalFragShaderSource.length);
            glCompileShader(normalFragShader);
            renderer->normalFragShader = normalFragShader;
            
            
            GLuint normalProgram = glCreateProgram();
            
            glAttachShader(normalProgram, renderer->basicVertShader);
            glAttachShader(normalProgram, renderer->normalFragShader);
            glLinkProgram(normalProgram);
            glValidateProgram(normalProgram);
            
            glDetachShader(normalProgram, renderer->basicVertShader);
            glDetachShader(normalProgram, renderer->normalFragShader);
            
            
            renderer->normalProgram  = normalProgram;
        }
        
        
        {
            TK_StringSlice uvFragShaderSource = TK_StringSliceLiteral(
                "#version 460 core"                             nl
                                                                nl
                "layout(location=0) in vec3 i_position;"        nl
                "layout(location=1) in vec3 i_normal;"          nl
                "layout(location=2) in vec2 i_uv;"              nl
                                                                nl
                "layout(location=0) out vec4 o_color;"          nl
                                                                nl
                "layout(location=0) uniform mat4 u_model;"      nl
                "layout(location=1) uniform mat4 u_camera;"     nl
                "layout(location=2) uniform mat4 u_projection;" nl
                                                                nl
                "void main()"                                   nl
                "{"                                             nl
                "    o_color = vec4(i_uv, 0.0, 1.0);"           nl
                "}"
            );
            
            GLuint uvFragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(uvFragShader, 1, (void *)&uvFragShaderSource.items, (void *)&uvFragShaderSource.length);
            glCompileShader(uvFragShader);
            renderer->uvFragShader = uvFragShader;
            
            
            GLuint uvProgram = glCreateProgram();
            
            glAttachShader(uvProgram, renderer->basicVertShader);
            glAttachShader(uvProgram, renderer->uvFragShader);
            glLinkProgram(uvProgram);
            glValidateProgram(uvProgram);
            
            glDetachShader(uvProgram, renderer->basicVertShader);
            glDetachShader(uvProgram, renderer->uvFragShader);
            
            
            renderer->uvProgram = uvProgram;
        }
        
        
        {
            TK_StringSlice lightFragShaderSource = TK_StringSliceLiteral(
                "#version 460 core"                                                                        nl
                                                                                                           nl
                "layout(location=0) in vec3 i_position;"                                                   nl
                "layout(location=1) in vec3 i_normal;"                                                     nl
                "layout(location=2) in vec2 i_uv;"                                                         nl
                                                                                                           nl
                "layout(location=0) out vec4 o_color;"                                                     nl
                                                                                                           nl
                "layout(location=0) uniform mat4 u_model;"                                                 nl
                "layout(location=1) uniform mat4 u_camera;"                                                nl
                "layout(location=2) uniform mat4 u_projection;"                                            nl
                "layout(location=3) uniform sampler2D u_lightDepth;"                                       nl
                "layout(location=4) uniform mat4 u_light;"                                                 nl
                "layout(location=5) uniform mat4 u_lightProj;"                                             nl
                "layout(location=6) uniform vec3 u_lightPos;"                                              nl
                "layout(location=7) uniform vec3 u_cameraPos;"                                             nl
                                                                                                           nl
                                                                                                           nl
                "vec2 lightRenderUV(vec4 world)"                                                           nl
                "{"                                                                                        nl
                "    vec3  lightDir   = normalize(u_lightPos - world.xyz);"                                nl
                "    vec4  lightCoord = u_lightProj * u_light * world;"                                    nl
                "    vec3  projCoords = lightCoord.xyz / lightCoord.w;"                                    nl
                "    "                                                                                     nl
                "    return projCoords.xy * 0.5 + 0.5;"                                                    nl
                "}"                                                                                        nl
                                                                                                           nl
                                                                                                           nl
                "float calcShadow(vec4 world)"                                                             nl
                "{"                                                                                        nl
                "    vec3  lightDir   = normalize(u_lightPos - world.xyz);"                                nl
                "    vec4  lightCoord = u_lightProj * u_light * world;"                                    nl
                "    vec3  projCoords = lightCoord.xyz / lightCoord.w;"                                    nl
                "    vec2  uv         = projCoords.xy * 0.5 + 0.5;"                                        nl
                "    "                                                                                     nl
                "    if (uv.x > 1.0 || uv.x < 0.0 || uv.y > 1.0 || uv.y < 0.0)"                            nl
                "       return 0.0;"                                                                       nl
                "    "                                                                                     nl
                "    return texture(u_lightDepth, uv).r < projCoords.z ? 1.0 : 0.0;"                       nl
                "}"                                                                                        nl
                                                                                                           nl
                                                                                                           nl
                "void main()"                                                                              nl
                "{"                                                                                        nl
                "    const vec4  ambientColor  = vec4(0.0, 0.1, 0.2, 1.0);"                                nl
                "    const vec4  lightColor    = vec4(0.8, 0.3, 0.3, 1.0);"                                nl
                "    "                                                                                     nl
                "    const vec4  world            = u_model * vec4(i_position, 1.0);"                      nl
                "    const float lightPower       = 2.0;"                                                  nl
                "    const vec3  lightDir         = normalize(vec3(1.0, -1.0, -1.0));"                     nl
                "    const vec3  viewDir          = normalize(u_cameraPos - world.xyz);"                   nl
                "    "                                                                                     nl
                "    const vec3 incoming   = -normalize(u_lightPos - world.xyz);"                          nl
                "    const vec3 reflectDir = reflect(incoming, i_normal);"                                 nl
                "    "                                                                                     nl
                "    "                                                                                     nl
                "    o_color = vec4(ambientColor.xyz * ambientColor.w, 1.0);"                              nl
                "    "                                                                                     nl
                "    if (calcShadow(world) == 0.0)"                                                        nl
                "    {"                                                                                    nl
                "       float diffuse = max(0.0, dot(i_normal, -incoming)) * lightPower;"                  nl
                "       "                                                                                  nl
                "       o_color += vec4(diffuse * lightColor.xyz    * lightColor.w,    0.0);"              nl
                "    }"                                                                                    nl
                "}"                                                                                        nl
            );
            
            
            GLuint lightFragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(lightFragShader, 1, (void *)&lightFragShaderSource.items, (void *)&lightFragShaderSource.length);
            glCompileShader(lightFragShader);
            renderer->lightFragShader = lightFragShader;
            
            
            GLuint lightProgram = glCreateProgram();
            
            glAttachShader(lightProgram, renderer->basicVertShader);
            glAttachShader(lightProgram, renderer->lightFragShader);
            glLinkProgram(lightProgram);
            glValidateProgram(lightProgram);
            
            glDetachShader(lightProgram, renderer->basicVertShader);
            glDetachShader(lightProgram, renderer->lightFragShader);
            
            
            renderer->lightProgram = lightProgram;
        }
        #undef nl
    }
    
    
    {
        TK__GL_Vertex cubeMeshVerticies[CUBE_VERTEX_COUNT] = {
            /* BACK */
            { .position = { +0.5f, +0.5f, -0.5f }, .normal = { +0.0f, +0.0f, -1.0f }, .texureCoordinates = { 0.f, 0.f } },
            { .position = { +0.5f, -0.5f, -0.5f }, .normal = { +0.0f, +0.0f, -1.0f }, .texureCoordinates = { 0.f, 1.f } },
            { .position = { -0.5f, -0.5f, -0.5f }, .normal = { +0.0f, +0.0f, -1.0f }, .texureCoordinates = { 1.f, 1.f } },
            { .position = { -0.5f, +0.5f, -0.5f }, .normal = { +0.0f, +0.0f, -1.0f }, .texureCoordinates = { 1.f, 0.f } },
            
            /* FRONT */
            { .position = { +0.5f, +0.5f, +0.5f }, .normal = { +0.0f, +0.0f, +1.0f }, .texureCoordinates = { 1.f, 0.f }  },
            { .position = { +0.5f, -0.5f, +0.5f }, .normal = { +0.0f, +0.0f, +1.0f }, .texureCoordinates = { 1.f, 1.f }  },
            { .position = { -0.5f, -0.5f, +0.5f }, .normal = { +0.0f, +0.0f, +1.0f }, .texureCoordinates = { 0.f, 1.f }  },
            { .position = { -0.5f, +0.5f, +0.5f }, .normal = { +0.0f, +0.0f, +1.0f }, .texureCoordinates = { 0.f, 0.f }  },
            
            /* TOP */
            { .position = { +0.5f, +0.5f, +0.5f }, .normal = { +0.0f, +1.0f, +0.0f }, .texureCoordinates = { 1.f, 1.f }  },
            { .position = { +0.5f, +0.5f, -0.5f }, .normal = { +0.0f, +1.0f, +0.0f }, .texureCoordinates = { 1.f, 0.f }  },
            { .position = { -0.5f, +0.5f, -0.5f }, .normal = { +0.0f, +1.0f, +0.0f }, .texureCoordinates = { 0.f, 0.f }  },
            { .position = { -0.5f, +0.5f, +0.5f }, .normal = { +0.0f, +1.0f, +0.0f }, .texureCoordinates = { 0.f, 1.f }  },
            
            /* BOTTOM */
            { .position = { +0.5f, -0.5f, +0.5f }, .normal = { +0.0f, -1.0f, +0.0f }, .texureCoordinates = { 1.f, 1.f }  },
            { .position = { +0.5f, -0.5f, -0.5f }, .normal = { +0.0f, -1.0f, +0.0f }, .texureCoordinates = { 1.f, 0.f }  },
            { .position = { -0.5f, -0.5f, -0.5f }, .normal = { +0.0f, -1.0f, +0.0f }, .texureCoordinates = { 0.f, 0.f }  },
            { .position = { -0.5f, -0.5f, +0.5f }, .normal = { +0.0f, -1.0f, +0.0f }, .texureCoordinates = { 0.f, 1.f }  },
            
            /* RIGHT */
            { .position = { +0.5f, +0.5f, +0.5f }, .normal = { +1.0f, +0.0f, +0.0f }, .texureCoordinates = { 0.f, 0.f }  },
            { .position = { +0.5f, +0.5f, -0.5f }, .normal = { +1.0f, +0.0f, +0.0f }, .texureCoordinates = { 1.f, 0.f }  },
            { .position = { +0.5f, -0.5f, -0.5f }, .normal = { +1.0f, +0.0f, +0.0f }, .texureCoordinates = { 1.f, 1.f }  },
            { .position = { +0.5f, -0.5f, +0.5f }, .normal = { +1.0f, +0.0f, +0.0f }, .texureCoordinates = { 0.f, 1.f }  },
            
            /* LEFT */
            { .position = { -0.5f, +0.5f, +0.5f }, .normal = { -1.0f, +0.0f, +0.0f }, .texureCoordinates = { 1.f, 0.f }  },
            { .position = { -0.5f, +0.5f, -0.5f }, .normal = { -1.0f, +0.0f, +0.0f }, .texureCoordinates = { 0.f, 0.f }  },
            { .position = { -0.5f, -0.5f, -0.5f }, .normal = { -1.0f, +0.0f, +0.0f }, .texureCoordinates = { 0.f, 1.f }  },
            { .position = { -0.5f, -0.5f, +0.5f }, .normal = { -1.0f, +0.0f, +0.0f }, .texureCoordinates = { 1.f, 1.f }  },
        };
        
        
        GLuint indexOffsets[6] = {
            0, 1, 2,
            2, 3, 0,
        };
        
        
        GLuint cubeMeshIndicies[CUBE_INDEX_COUNT] = {
            /* BACK */
            0 + indexOffsets[0],  0 + indexOffsets[1],  0 + indexOffsets[2],
            0 + indexOffsets[3],  0 + indexOffsets[4],  0 + indexOffsets[5],

            /* FRONT */
            4 + indexOffsets[0],  4 + indexOffsets[1],  4 + indexOffsets[2],
            4 + indexOffsets[3],  4 + indexOffsets[4],  4 + indexOffsets[5],

            /* TOP */
            8 + indexOffsets[0],  8 + indexOffsets[1],  8 + indexOffsets[2],
            8 + indexOffsets[3],  8 + indexOffsets[4],  8 + indexOffsets[5],

            /* BOTTOM */
            12 + indexOffsets[0], 12 + indexOffsets[1], 12 + indexOffsets[2],
            12 + indexOffsets[3], 12 + indexOffsets[4], 12 + indexOffsets[5],

            /* RIGHT */
            16 + indexOffsets[0], 16 + indexOffsets[1], 16 + indexOffsets[2],
            16 + indexOffsets[3], 16 + indexOffsets[4], 16 + indexOffsets[5],

            /* LEFT */
            20 + indexOffsets[0], 20 + indexOffsets[1], 20 + indexOffsets[2],
            20 + indexOffsets[3], 20 + indexOffsets[4], 20 + indexOffsets[5],
        };
        
        GLuint vertexArray;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        {
            GLuint vertexBuffer;
            glGenBuffers(1, &vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMeshVerticies), cubeMeshVerticies, GL_STATIC_DRAW);
            
            GLuint indexBuffer;
            glGenBuffers(1, &indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeMeshIndicies), cubeMeshIndicies, GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TK__GL_Vertex), cast(void *)offsetof(TK__GL_Vertex, position));
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  sizeof(TK__GL_Vertex), cast(void *)offsetof(TK__GL_Vertex, normal));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE,  sizeof(TK__GL_Vertex), cast(void *)offsetof(TK__GL_Vertex, texureCoordinates));
        }
        glBindVertexArray(0);
        
        renderer->cubeMeshVertexArray = vertexArray;
    }
    
    
    return renderer;
}



void TK_DestroyRenderer(TK_Renderer *renderer)
{
    if (!renderer)
        return;
    
    glDeleteFramebuffers(1, &renderer->sceneDepthFramebuffer);
    glDeleteTextures(1, &renderer->sceneDepthTexture);
    glDeleteShader(renderer->basicVertShader);
    glDeleteShader(renderer->positionFragShader);
    glDeleteShader(renderer->normalFragShader);
    glDeleteShader(renderer->uvFragShader);
    glDeleteShader(renderer->lightFragShader);
    glDeleteProgram(renderer->positionProgram);
    glDeleteProgram(renderer->normalProgram);
    glDeleteProgram(renderer->uvProgram);
    glDeleteProgram(renderer->lightProgram);
    glDeleteVertexArrays(1, &renderer->cubeMeshVertexArray);
    
    TK_free(renderer->allocator, renderer);
}



void TK_RenderPresent(TK_Renderer *renderer)
{
    if (!renderer)
        return;
    
    TK_SwapBuffers(renderer->window);
}



void TK_RenderScene_REAL(TK_Renderer *renderer, TK_Entity *entity, TK_RenderSceneInfo info)
{
    enum {
        RENDER_NONE,
        RENDER_CUBE,
    };
    
    int render = RENDER_NONE;
    
    
    TK_RenderSceneInfo prev = info;
    for (size_t i = 0; i < entity->properties.length; ++i)
    {
        TK_Property prop = entity->properties.items[i];
        
        if (TK_AreStringSlicesEq(TK_PROP_ENTITY_BOX, prop.name))
            render = prop.value.data.bool_ ? RENDER_CUBE : render;
        else if (TK_AreStringSlicesEq(TK_PROP_ENTITY_POSITION3, prop.name))
            info.position = TKM_vector_add(info.position, TKM_vector_mul(prop.value.data.vec3f, prev.scale));
        else if (TK_AreStringSlicesEq(TK_PROP_ENTITY_SCALE3, prop.name))
            info.scale = TKM_vector_mul(info.scale, prop.value.data.vec3f);
    }
    
    
    
    switch (render) {
        case RENDER_NONE: break;
        case RENDER_CUBE: {
            mat4s model = glms_mat4_mul(
                glms_translate_make((vec3s){ .x = info.position.x, .y = info.position.y, .z = info.position.z }),
                glms_scale_make((vec3s){ .x = info.scale.x, .y = info.scale.y, .z = info.scale.z })
            );
            
            mat4s camera = glms_look(
                (vec3s){ .x = info.camera_position.x,  .y = info.camera_position.y,  .z = info.camera_position.z  },
                (vec3s){ .x = info.camera_direction.x, .y = info.camera_direction.y, .z = info.camera_direction.z },
                (vec3s){ .y = 1.f }
            );
            
            mat4s projection = glms_perspective(info.fov, info.aspect, info.zrange.near, info.zrange.far);
            
            
            GLuint program = 0;
            switch (info.action) {
                case TK_RENDER_SCENE_POSITION: program = renderer->positionProgram; break;
                case TK_RENDER_SCENE_COLOR:    program = renderer->uvProgram;       break;
                case TK_RENDER_SCENE_DEPTH:    program = renderer->normalProgram;   break;
                case TK_RENDER_SCENE_UV:       program = renderer->uvProgram;       break;
                case TK_RENDER_SCENE_NORMAL:   program = renderer->normalProgram;   break;
                case TK_RENDER_SCENE_LIGHT:    program = renderer->lightProgram;    break;
            }
            
            
            glBindVertexArray(renderer->cubeMeshVertexArray);
            glUseProgram(program);
            glUniformMatrix4fv(0, 1, GL_FALSE, &model.m00);
            glUniformMatrix4fv(1, 1, GL_FALSE, &camera.m00);
            glUniformMatrix4fv(2, 1, GL_FALSE, &projection.m00);
            
            if (info.action == TK_RENDER_SCENE_LIGHT)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, renderer->sceneDepthTexture);
                
                mat4s light = glms_look(
                    (vec3s){ .x = info.light_position.x,  .y = info.light_position.y,  .z = info.light_position.z  },
                    (vec3s){ .x = info.light_direction.x, .y = info.light_direction.y, .z = info.light_direction.z },
                    (vec3s){ .y = 1.f }
                );
                
                mat4s projection = glms_perspective(info.fov, cast(float)SHADOW_WIDTH / cast(float)SHADOW_HEIGHT, info.zrange.near, info.zrange.far);
                
                glUniform1i(3, 0);
                glUniformMatrix4fv(4, 1, GL_FALSE, &light.m00);
                glUniformMatrix4fv(5, 1, GL_FALSE, &projection.m00);
                glUniform3fv(6, 1, info.light_position.components);
                glUniform3fv(7, 1, info.camera_position.components);
            }
            
            glDrawElements(GL_TRIANGLES, CUBE_INDEX_COUNT, GL_UNSIGNED_INT, NULL);
        } break;
    }
    
    
    
    TK_Entity *child = entity->children;
    for (size_t i = 0; i < entity->child_count; ++i)
    {
        TK_assert(child);
        TK_RenderScene_REAL(renderer, child, info);
        child = child->next;
    }
}



void TK_RenderScene(TK_Renderer *renderer, TK_Entity *sceneRoot)
{
    if (!renderer)
        return;
    
    
    TK_vec2u windowSize = TK_GetWindowSize(renderer->window);
    
    struct {
        TK_vec3f position;
        TK_vec3f direction;
    } camera = {
        .position  = {15.f, 15.f, 15.f},
        .direction = TKM_vector_normalize((TK_vec3f){ -1.f, -1.f, -1.f }),
    };
    
    
    struct {
        TK_vec3f position;
        TK_vec3f direction;
    } light = {
        .position  = {-7.f, 5.f, 7.f},
        .direction = TKM_vector_normalize((TK_vec3f){ 1.f, -1.f, -1.f }),
    };
    
    
    
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1.f);
    
    
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->sceneDepthFramebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TK_RenderScene_REAL(renderer, sceneRoot, (TK_RenderSceneInfo){
        .action   = TK_RENDER_SCENE_POSITION,
        .aspect   = cast(float)SHADOW_WIDTH / cast(float)SHADOW_HEIGHT,
        .fov      = TK_radians(60.f),
        .zrange   = {0.1f, 1000.0f},
        .scale    = {1.f, 1.f, 1.f},
        .position = {0.f, 0.f, 0.f},
        
        .camera_position  = light.position,
        .camera_direction = light.direction,
    });
    
    
    glViewport(0, 0, windowSize.width, windowSize.height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TK_RenderScene_REAL(renderer, sceneRoot, (TK_RenderSceneInfo){
        .action   = TK_RENDER_SCENE_LIGHT,
        .aspect   = cast(float)windowSize.width / cast(float)windowSize.height,
        .fov      = TK_radians(60.f),
        .zrange   = {0.1f, 1000.0f},
        .scale    = {1.f, 1.f, 1.f},
        .position = {0.f, 0.f, 0.f},
        
        .camera_position  = camera.position,
        .camera_direction = camera.direction,
        .light_position   = light.position,
        .light_direction  = light.direction,
    });
}
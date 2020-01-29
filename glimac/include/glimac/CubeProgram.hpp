#pragma once

#include "glm.hpp"
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

namespace glimac {

    class CubeProgram {

    public :

        Program m_Program;

        GLint uMVMatrix;
        GLint uMVPMatrix;
        GLint uNormalMatrix;
        GLint uTexture;

        GLint uTextureKs; //
        GLint uTextureKd; // diffuse
        GLint uTextureShininess; // Texture's shininess
        GLint uLightDir_vs;
        GLint uLightIntensity;
        GLint uLightAmbient;

        CubeProgram(const FilePath& applicationPath):
            m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/3D2Light.fs.glsl")) {
            uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
            uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
            uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
            uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");

            uTextureKs = glGetUniformLocation(m_Program.getGLId(), "uTextureKs");
            uTextureKd = glGetUniformLocation(m_Program.getGLId(), "uTextureKd");
            uTextureShininess = glGetUniformLocation(m_Program.getGLId(), "uTextureShininess");
            uLightDir_vs = glGetUniformLocation(m_Program.getGLId(), "uLightDir_vs");
            uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
            uLightAmbient = glGetUniformLocation(m_Program.getGLId(), "uLightAmbient");
        }
    };
}

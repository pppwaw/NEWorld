// 
// GUI: renderer.cpp
// NEWorld: A Free Game with Similar Rules to Minecraft.
// Copyright (C) 2015-2018 NEWorld Team
// 
// NEWorld is free software: you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// NEWorld is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General 
// Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with NEWorld.  If not, see <http://www.gnu.org/licenses/>.
// 

#include "renderer.h"
#include "Common/Filesystem.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Common/JsonHelper.h"

namespace {
    GLuint gProgram{0}, gIndex{0}, gMvpId{0}, gVao{0};

    class Shader {
    public:
        explicit Shader(GLenum stage): mShader(glCreateShader(stage)) {}

        void compile(const std::string& text) {
            auto array = text.c_str();
            glShaderSource(mShader, 1, &array, nullptr);
            glCompileShader(mShader);
            int status {};
            glGetShaderiv(mShader, GL_COMPILE_STATUS, &status);
            if(status != GL_TRUE){
                int logLen = 0;
                glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &logLen);
                std::string log(static_cast<size_t>(logLen + 1), '\0');
                glGetShaderInfoLog(mShader,logLen + 1,&logLen,log.data());
                const auto message = "Could not compile shader, error :" + log;
                errorstream << "Could not compile shader, error " << status << ": " << log;
                throw std::runtime_error(log);
            }
        }

        ~Shader() {
            if (mShader) {
                glDeleteShader(mShader);
            }
        }

        [[nodiscard]] GLuint native() const noexcept { return mShader; }
    private:
        GLuint mShader {};
    };

    std::string loadFile(const filesystem::path& path) {
        std::ifstream file(path);
        return std::string(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());
    }

    void buildIndexBuffer() {
        glGenBuffers(1, &gIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 262144/2*3*sizeof(int), nullptr, GL_STATIC_DRAW);
        auto idx = reinterpret_cast<std::uint32_t*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
        auto cnt = 0;
        for (auto i = 0; i<262144/4; ++i) {
            auto b = i*4;
            idx[cnt++] = b+2;
            idx[cnt++] = b+1;
            idx[cnt++] = b;
            idx[cnt++] = b+3;
            idx[cnt++] = b+2;
            idx[cnt++] = b;
        }
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Renderer::init() {
    debugstream << "Start to initialize GLEW...";
    const auto err = glewInit();
    if (err != GLEW_OK)
        fatalstream << "Failed to initialize GLEW! Error code: "
            << err << ": " << glewGetErrorString(err);
    else
        debugstream << "GLEW initialized!";
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    buildIndexBuffer();
    glGenVertexArrays(1, &gVao);
    glBindVertexArray(gVao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Renderer::configShader(const std::string& name) {
    if (gProgram) { glDeleteProgram(gProgram); gProgram = 0; }
    gProgram = glCreateProgram();
    const auto base = assetDir("infinideas.gui") / "Shaders" / name;
    nlohmann::json config = readJsonFromFile((base / "Info.json").string());

    auto vertex = Shader(GL_VERTEX_SHADER);
    auto pixel = Shader(GL_FRAGMENT_SHADER);
    if (config.find("vertex") != config.end()) {
        vertex.compile(loadFile(base / config["vertex"].get<std::string>()));
    }
    if (config.find("pixel") != config.end()) {
        pixel.compile(loadFile(base / config["pixel"].get<std::string>()));
    }
    glAttachShader(gProgram, vertex.native());
    glAttachShader(gProgram, pixel.native());
    glLinkProgram(gProgram);
    glDetachShader(gProgram, vertex.native());
    glDetachShader(gProgram, pixel.native());
    gMvpId = glGetUniformLocation(gProgram, "MVP");
    glUniform1i(glGetUniformLocation(gProgram, "Texture"), 0);
}

void Renderer::startFrame() {
    glBindVertexArray(gVao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndex);
    glUseProgram(gProgram);
}

void Renderer::endFrame() {
    glUseProgram(0);
    glBindVertexArray(0);
}

void Renderer::setMatrix() {
    const auto matrix = getMvpMatrix();
    glUniformMatrix4fv(gMvpId, 1, GL_FALSE, matrix.data);
}

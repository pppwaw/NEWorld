// 
// GUI: vertexarray.cpp
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

#include "vertexarray.h"

void VertexBuffer::update(const VertexArray& va) {
    vertexes = va.getVertexCount();
    format = va.getFormat();
    if (vertexes == 0)
        destroy();
    else {
        if (id == 0)
            glGenBuffersARB(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, va.getVertexCount() * sizeof(float) *
                        format.vertexAttributeCount, va.getData(), GL_STATIC_DRAW);
    }
}

VertexBuffer::VertexBuffer(const VertexArray& va)
    : VertexBuffer() { update(va); }

void VertexBuffer::render() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
            reinterpret_cast<const void*>(sizeof(float)*3));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
            reinterpret_cast<const void*>(sizeof(float)*2));
    glDrawElements(GL_TRIANGLES, vertexes + vertexes / 2, GL_UNSIGNED_INT, nullptr);
}

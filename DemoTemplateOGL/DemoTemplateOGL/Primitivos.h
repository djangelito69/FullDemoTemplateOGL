#pragma once

#include "Base/Model.h"

class Primitivos : public Model {
public:
    Primitivos(int stacks, int slices, float radio, Camera* camera) : Model() {
        UTILITIES_OGL::Maya maya = UTILITIES_OGL::Esfera(stacks, slices, radio, 0.0f, 2.0f);
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        const unsigned int cantidadVertices = stacks * slices;
        const unsigned int cantidadIndices = (stacks - 1) * (slices - 1) * 6;
        vertices.reserve(cantidadVertices);
        indices.reserve(cantidadIndices);
        UTILITIES_OGL::vectoresEsfera(maya, vertices, indices, cantidadVertices, cantidadIndices);
        for (unsigned int i = 0; i < indices.size(); i += 3)
            std::swap(indices[i + 1], indices[i + 2]);

        meshes.emplace_back(Mesh::createMesh(vertices, indices, textures));
        cameraDetails = camera;

        delete[] maya.maya;
        delete[] maya.indices;
    }
};


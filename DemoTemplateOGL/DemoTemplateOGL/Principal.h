#pragma once
#include "Base/model.h"

class Principal : public Model {
private:
    int vida = 250;
    int vidaMaxima = 250;

    void initVidas(int vidaInicial = 250) {
        vidaMaxima = vidaInicial;
        vida = vidaInicial;
    }

public:
    Principal() : Model() {
        initVidas();
    }

    Principal(string const& path, Camera* camera, bool rotationX = false, bool rotationY = true, bool gamma = false)
        : Model(path, camera, rotationX, rotationY, gamma) {
        initVidas();
    }

    Principal(vector<Vertex>& vertices, unsigned int numVertices, vector<unsigned int>& indices, unsigned int numIndices, Camera* camera)
        : Model(vertices, numVertices, indices, numIndices, camera) {
        initVidas();
    }

    Principal(string const& path, glm::vec3& actualPosition, Camera* cam, bool rotationX = false, bool rotationY = true, bool gamma = false)
        : Model(path, actualPosition, cam, rotationX, rotationY, gamma) {
        initVidas();
    }

    float restaVida() {
        if (vida > 0)
            vida--;
        if (vida == 0)
            morir();
        return vida;
    }

    bool estaMuerto() const {
        return vida <= 0;
    }

    void morir() {
        vida = 0;
        setAnimation(7);
    }

    int getVida() {
        return vida;
    }

    int getVidaMaxima() {
        return vidaMaxima;
    }

    void setVida(int nuevaVida) {
        if (nuevaVida < 0) nuevaVida = 0;
        if (nuevaVida > vidaMaxima) nuevaVida = vidaMaxima;
        vida = nuevaVida;
        if (vida == 0)
            morir();
    }

    void regenerarVida() {
        vida = vidaMaxima;
    }
}; 
#ifndef GRAPHICSRR_H
#define GRAPHICSRR_H

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Base/camera.h"
#include "Base/Billboard.h"
#include <iostream>
#include "Base/Scene.h"
#include "Texto.h"
#include "Billboard2D.h"
#include "CollitionBox.h"
#include "spritesec.h"
#include "Enemigo.h"
#include "VidaFlor.h"
#include "TemporizadorJuego.h"

class Scenario : public Scene {
private:
	SkyDome* sky;
	Terreno* terreno;
	std::vector<Billboard*> billBoard;
	std::vector<BillboardAnimation*> billBoardAnim;
	std::vector<Billboard2D*> billBoard2D;
	std::vector<Model*> ourModel;
	std::vector<Enemigo*> enemigos;
	std::vector<Enemigo*> enemigosDisponibles;
	Model* camara;
	Model* florObjetivo;
	Model* puntoSpawnEnemigos;
	VidaFlor* vidaFlor;
	TemporizadorJuego* temporizador;
	bool juegoFinalizado;
	bool juegoGanado;
	float angulo;
	std::vector<Texto*> ourText;

	void inicializaEnemigos();
	void crearEnemigo();
	void mostrarResultado(const wchar_t* mensaje);
	void actualizarVidaFlorEnPantalla();
	void actualizarTemporizadorEnPantalla();
public:
	Scenario(Camera *cam);
	Scenario(Model *camIni);
	void InitGraph(Model *main);

	void inicializaArboles();
	void inicializaTrigo();
	void inicializaPollos();
	void inicializaPuercos();
	void inicializaVacas();
	void inicializaOvejas();

	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
	Scene* Render();
	int update() override;

	std::vector<Model*> *getLoadedModels();
	std::vector<Billboard*> *getLoadedBillboards();
	std::vector<BillboardAnimation*> *getLoadedBillboardsAnimation();
	std::vector<Billboard2D*> *getLoadedBillboards2D();
	std::vector<Texto*> *getLoadedText();
	Model* getMainModel();
	void setMainModel(Model* mainModel);
	float getAngulo();
	void setAngulo(float angulo);
	SkyDome* getSky();
	Terreno* getTerreno();
//    void update() override;
    ~Scenario();
};

#endif
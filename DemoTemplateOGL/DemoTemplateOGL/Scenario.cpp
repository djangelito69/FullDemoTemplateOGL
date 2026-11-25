#include "Scenario.h"
#include "Ammo.h"
#include <random>
#ifdef __linux__ 
#define ZeroMemory(x,y) memset(x,0,y)
#define wcscpy_s(x,y,z) wcscpy(x,z)
#define wcscat_s(x,y,z) wcscat(x,z)
#endif

Scenario::Scenario(Camera *cam) {
	florObjetivo = NULL;
	puntoSpawnEnemigos = NULL;
	vidaFlor = NULL;
	temporizador = NULL;
	juegoFinalizado = false;
	juegoGanado = false;
	angulo = 0;
	glm::vec3 translate;
	glm::vec3 scale;
    Model* model = new Model("models/Cube/Cube.obj", cam);
	translate = glm::vec3(0.0f, 0.0f, 3.0f);
	scale = glm::vec3(0.25f, 0.25f, 0.25f);	// it's a bit too big for our scene, so scale it down
	model->setScale(&scale);
	model->setTranslate(&translate);
	model->setNextTranslate(&translate);
	InitGraph(model);
}
Scenario::Scenario(Model *camIni) {
    florObjetivo = NULL;
    puntoSpawnEnemigos = NULL;
    vidaFlor = NULL;
    temporizador = NULL;
    juegoFinalizado = false;
    juegoGanado = false;
    angulo = 0;
    InitGraph(camIni);
}
void Scenario::InitGraph(Model* main) {
	float matAmbient[] = { 1,1,1,1 };
	float matDiff[] = { 1,1,1,1 };
	angulo = 0;
	camara = main;
	//creamos el objeto skydome
	sky = new SkyDome(32, 32, 20, (WCHAR*)L"skydome/nublado.jpg", main->cameraDetails);
	//creamos el terreno
	terreno = new Terreno((WCHAR*)L"skydome/TerrenoGranja2.jpg", (WCHAR*)L"skydome/Grass2.jpg", 400, 400, main->cameraDetails);
	glm::vec3 translate;
	glm::vec3 scale;
	glm::vec3 rotation;
	translate = glm::vec3(0.0f, 20.0f, 30.0f);
	// load models
	// -----------
	ourModel.emplace_back(main);
	Model* model;
	ModelAttributes m;


	model = new Model("models/Especial/FLOR.fbx", main->cameraDetails);
	florObjetivo = model;
	florObjetivo->name = "Flor";
	vidaFlor = new VidaFlor(500, 20);
	translate = glm::vec3(136.0f, 6.7f, -86.0f);
	scale = glm::vec3(3.0f, 3.0f, 3.0f);
	model->setTranslate(&translate);
	model->setNextTranslate(&translate);
	model->setScale(&scale);
	ourModel.emplace_back(model);
	try{
		std::vector<Animation> animations = Animation::loadAllAnimations("models/Especial/FLOR.fbx", model->GetBoneInfoMap(), model->getBonesInfo(), model->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("models/Especial/FLOR.fbx", model->GetBoneInfoMap(), model->getBonesInfo(), model->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			model->setAnimator(Animator(animation));
		model->setAnimation(1);
	}catch(...){
		ERRORL("Could not load animation!", "ANIMACION");
	}

	Model* ufo = new Model("models/Casa/UFO.fbx", main->cameraDetails);
	translate = glm::vec3(0.5f, 50.0f, -0.5f);
	scale = glm::vec3(35.0f, 35.0f, 30.0f);
	ufo->setTranslate(&translate);
	ufo->setNextTranslate(&translate);
	ufo->setScale(&scale);
	ufo->setRotX(270.0f);
	ufo->setNextRotX(270.0f);
	ufo->setRotZ(270.0f);
	ufo->setNextRotZ(270.0f);
	ufo->ignoreAABB = true;
	ufo->walkeable = false;
	puntoSpawnEnemigos = ufo;
	ourModel.emplace_back(ufo);

	Model* hay1 = new Model("models/Casa/HAYBALE.fbx", main->cameraDetails);
	translate = glm::vec3(168.5f, 6.0f, -130.5f);
	scale = glm::vec3(2.0f, 2.0f, 3.0f);
	hay1->setTranslate(&translate);
	hay1->setNextTranslate(&translate);
	hay1->setScale(&scale);
	hay1->setRotZ(270.0f);
	hay1->setNextRotZ(270.0f);
	hay1->ignoreAABB = false;
	hay1->walkeable = false;
	ourModel.emplace_back(hay1);

	Model* pwr = new Model("models/Casa/PowerGenerator.fbx", main->cameraDetails);
	translate = glm::vec3(183.5f, 7.0f, -135.5f);
	scale = glm::vec3(6.0f, 6.0f, 6.0f);
	pwr->setTranslate(&translate);
	pwr->setNextTranslate(&translate);
	pwr->setScale(&scale);
	pwr->setRotX(270.0f);
	pwr->setNextRotX(270.0f);
	pwr->ignoreAABB = false;
	pwr->walkeable = false;
	ourModel.emplace_back(pwr);


	Model* casa = new Model("models/Casa/CASA.fbx", main->cameraDetails);
	translate = glm::vec3(159.5f, 24.0f, -121.5f);
	scale = glm::vec3(0.22f, 0.22f, 0.22f);
	casa->setTranslate(&translate);
	casa->setNextTranslate(&translate);
	casa->setScale(&scale);
	casa->setRotX(270.0f);
	casa->setNextRotX(270.0f);
	casa->setRotZ(270.0f);
	casa->setNextRotZ(270.0f);
	casa->ignoreAABB = true;
	casa->walkeable = false;
	ourModel.emplace_back(casa);

	//Colisiones de la Casa
	CollitionBox* paredCasa1 = new CollitionBox(
		164.13f, 20.2f, -147.1f,
		26.1f, 20.0f, 1.5f, main->cameraDetails);
	paredCasa1->name = "ParedCasa1";
	paredCasa1->ignoreAABB = false;
	paredCasa1->walkeable = false;
	ourModel.emplace_back(paredCasa1);

	CollitionBox* paredCasa2 = new CollitionBox(
		164.13f, 20.2f, -96.3f,
		26.1f, 20.0f, 1.5f, main->cameraDetails);
	paredCasa2->name = "ParedCasa2";
	paredCasa2->ignoreAABB = false;
	paredCasa2->walkeable = false;
	ourModel.emplace_back(paredCasa2);

	CollitionBox* paredCasa3 = new CollitionBox(
		190.3f, 20.2f, -121.3f,
		1.5f, 20.0f, 25.4f, main->cameraDetails);
	paredCasa3->name = "ParedCasa3";
	paredCasa3->ignoreAABB = false;
	paredCasa3->walkeable = false;
	ourModel.emplace_back(paredCasa3);

	CollitionBox* paredCasa4 = new CollitionBox(
		138.0f, 20.2f, -141.9f,
		1.5f, 20.0f, 5.0f, main->cameraDetails);
	paredCasa4->name = "ParedCasa4";
	paredCasa4->ignoreAABB = false;
	paredCasa4->walkeable = false;
	ourModel.emplace_back(paredCasa4);

	CollitionBox* paredCasa5 = new CollitionBox(
		138.13f, 20.2f, -100.0f,
		1.5f, 20.0f, 5.0f, main->cameraDetails);
	paredCasa5->name = "ParedCasa5";
	paredCasa5->ignoreAABB = false;
	paredCasa5->walkeable = false;
	ourModel.emplace_back(paredCasa5);



	Model* mesa = new Model("models/Casa/MESA.fbx", main->cameraDetails);
	translate = glm::vec3(159.5f, 8.5f, -121.5f);
	scale = glm::vec3(5.0f, 7.0f, 5.0f);
	mesa->setTranslate(&translate);
	mesa->setNextTranslate(&translate);
	mesa->setScale(&scale);
	mesa->setRotX(270.0f);
	mesa->setNextRotX(270.0f);
	mesa->setRotZ(180.0f);
	mesa->setNextRotZ(180.0f);
	mesa->ignoreAABB = false;
	mesa->walkeable = false;
	ourModel.emplace_back(mesa);

	Model* crates = new Model("models/Casa/Crates.fbx", main->cameraDetails);
	translate = glm::vec3(181.5f, 8.5f, -103.5f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	crates->setTranslate(&translate);
	crates->setNextTranslate(&translate);
	crates->setScale(&scale);
	crates->ignoreAABB = false;
	crates->walkeable = false;
	ourModel.emplace_back(crates);

	Model* horse = new Model("models/horse.glb", main->cameraDetails);
	translate = glm::vec3(155.5f, 7.6f, -100.5f);
	scale = glm::vec3(7.0f, 7.0f, 7.0f);
	horse->setTranslate(&translate);
	horse->setNextTranslate(&translate);
	horse->setRotY(270.0f);
	horse->setNextRotY(270.0f);
	horse->setScale(&scale);
	horse->ignoreAABB = false;
	horse->walkeable = false;
	ourModel.emplace_back(horse);

	model = new Ammo("models/bullet/bullet_9_mm.glb", main->cameraDetails);
	translate = glm::vec3(159.5f, 8.0f, -123.5f);
	scale = glm::vec3(0.10f, 0.10f, 0.10f);
	model->setTranslate(&translate);
	model->setNextTranslate(&translate);
	model->setScale(&scale);
	model->setRotY(-90);
	model->setNextRotY(-90);
	delete model->getModelAttributes()->at(0).hitbox;
	model->getModelAttributes()->at(0).hitbox = NULL;
	ourModel.emplace_back(model);

	Model* aid = new Model("models/Casa/AID.fbx", main->cameraDetails);
	aid->name = "AID";
	translate = glm::vec3(159.5f, 10.0f, -121.5f);
	scale = glm::vec3(5.0f, 5.0f, 2.0f);
	aid->setTranslate(&translate);
	aid->setNextTranslate(&translate);
	aid->setScale(&scale);
	aid->setRotX(270.0f);
	aid->setNextRotX(270.0f);
	aid->setRotZ(270.0f);
	aid->setNextRotZ(270.0f);
	aid->ignoreAABB = false;
	aid->walkeable = false;
	ourModel.emplace_back(aid);





	Model* agua = new Model("models/agua.fbx", main->cameraDetails);
	agua->ignoreAABB = true;
	agua->walkeable = false;
	translate = glm::vec3(-105.0f, 3.5f, -104.0f);
	scale = glm::vec3(25.1f, 4.1f, 25.1f);

	agua->setTranslate(&translate);
	agua->setNextTranslate(&translate);
	agua->setScale(&scale);

	ourModel.emplace_back(agua);
	try {
		std::vector<Animation> animations = Animation::loadAllAnimations("models/agua.fbx", agua->GetBoneInfoMap(), agua->getBonesInfo(), agua->GetBoneCount());
		for (Animation animation : animations)
			agua->setAnimator(Animator(animation));
		agua->setAnimation(0);
	}
	catch (...) {
		ERRORL("Could not load animation!", "ANIMACION");
	}

	inicializaArboles();
	inicializaTrigo();
	inicializaPollos();
	inicializaPuercos();
	inicializaOvejas();
	Texto* vidaPrincipal = new Texto((WCHAR*)L"VIDA: 250", 0.35f, 0,
		20.0f, SCR_HEIGHT - 50.0f, 0, main->cameraDetails);
	vidaPrincipal->name = "Vida";
	ourText.emplace_back(vidaPrincipal);

	Texto* vidaFlorTexto = new Texto((WCHAR*)L"VIDA FLOR: 500", 0.35f, 0,
		SCR_WIDTH - 330.0f, 55.0f, 0, main->cameraDetails);
	vidaFlorTexto->name = "VidaFlor";
	ourText.emplace_back(vidaFlorTexto);

	temporizador = new TemporizadorJuego(150.0f);
	Texto* tiempoTexto = new Texto((WCHAR*)temporizador->getTexto().c_str(), 0.35f, 0,
		SCR_WIDTH / 2.0f - 95.0f, 20.0f, 0, main->cameraDetails);
	tiempoTexto->name = "TiempoRestante";
	ourText.emplace_back(tiempoTexto);

	inicializaEnemigos();

	}

	void Scenario::crearEnemigo() {
		if (juegoFinalizado || florObjetivo == NULL || puntoSpawnEnemigos == NULL) return;

		try {
			glm::vec3 posicionSpawn = *puntoSpawnEnemigos->getNextTranslate();
			std::mt19937 generador(static_cast<unsigned int>(get_nanos()));
			std::uniform_real_distribution<float> anguloAleatorio(0.0f, 360.0f);
			std::uniform_real_distribution<float> distanciaAleatoria(20.0f, 62.0f);
			std::uniform_real_distribution<float> alturaAleatoria(-1.0f, 2.0f);

			float angulo = anguloAleatorio(generador) * 3.14159265f / 180.0f;
			float distancia = distanciaAleatoria(generador);
			glm::vec3 posicion = posicionSpawn;
			posicion.x += cos(angulo) * distancia;
			posicion.z += sin(angulo) * distancia;
			posicion.y -= 5.0f;
			posicion.y = glm::max(posicion.y,
				terreno->Superficie(posicion.x, posicion.z) + 2.0f + alturaAleatoria(generador));

			Enemigo* enemigo = NULL;
			if (!enemigosDisponibles.empty()) {
				enemigo = enemigosDisponibles.back();
				enemigosDisponibles.pop_back();
				enemigo->reiniciar(florObjetivo, camara);
			}
			else {
				enemigo = new Enemigo(camara->cameraDetails, florObjetivo);
				enemigo->setObjetivoAlternativo(camara);
				enemigo->setRadioDeteccionJugador(35.0f);
			}
			enemigo->setTranslate(&posicion);
			enemigo->setNextTranslate(&posicion);
			glm::vec3 escala(0.04f, 0.04f, 0.04f);
			enemigo->setScale(&escala);
			enemigo->setEstado(Enemigo::Estado::Corriendo);
			enemigo->setAnimation(0);

			enemigos.emplace_back(enemigo);
			ourModel.emplace_back(enemigo);
		}
		catch (...) {
			ERRORL("No se pudo crear el enemigo.", "ENEMIGO");
		}
	}

	void Scenario::inicializaEnemigos() {
		for (int i = 0; i < 15; i++)
			crearEnemigo();
	}

	void Scenario::inicializaArboles() {
		const int cantidadArboles = 60;
		const float limiteX = 190.0f;
		const float limiteZ = 190.0f;

		std::random_device semilla;
		std::mt19937 generador(semilla());
		std::uniform_real_distribution<float> distribucionX(-limiteX, limiteX);
		std::uniform_real_distribution<float> distribucionZ(-limiteZ, limiteZ);

		glm::vec3 scale = glm::vec3(3.0f, 4.0f, 3.0f);

		float x = distribucionX(generador);
		float z = distribucionZ(generador);
		float ye = terreno->Superficie(x, z);
		glm::vec3 pos(x, ye, z);

		Model* basePino = new Model("models/arbol/Pino.fbx", camara->cameraDetails);
		try {
			std::vector<Animation> animations = Animation::loadAllAnimations("models/arbol/Pino.fbx", basePino->GetBoneInfoMap(), basePino->getBonesInfo(), basePino->GetBoneCount());
			for (Animation animation : animations)
				basePino->setAnimator(Animator(animation));
			basePino->setAnimation(0);
		}
		catch (...) {
			ERRORL("Could not load tree animation!", "ANIMACION");
		}

		basePino->setScale(&scale);
		basePino->setTranslate(&pos);
		basePino->setNextTranslate(&pos);
		basePino->walkeable = false;
		basePino->ignoreAABB = true;
		ourModel.emplace_back(basePino);

		for (int i = 1; i < cantidadArboles; i++) {
			x = distribucionX(generador);
			z = distribucionZ(generador);
			ye = terreno->Superficie(x, z);
			pos = glm::vec3(x, ye, z);

			Model* clon = basePino->clone();
			clon->setScale(&scale);
			clon->setTranslate(&pos);
			clon->setNextTranslate(&pos);
			clon->walkeable = false;
			ourModel.emplace_back(clon);
		}

		ye = terreno->Superficie(-9, -15) + 4;
	}


	void Scenario::inicializaTrigo() {
		const int cantidadTrigo = 150;
		const float limiteX = 190.0f;
		const float limiteZ = 190.0f;

		std::random_device semilla;
		std::mt19937 generador(semilla());
		std::uniform_real_distribution<float> distribucionX(-limiteX, limiteX);
		std::uniform_real_distribution<float> distribucionZ(-limiteZ, limiteZ);

		glm::vec3 scale = glm::vec3(6.0f, 6.0f, 6.0f);

		float x = distribucionX(generador);
		float z = distribucionZ(generador);
		float ye = terreno->Superficie(x, z);
		glm::vec3 pos(x, ye, z);

		Model* trigo = new Model("models/arbol/trigo.fbx", camara->cameraDetails);
		try {
			std::vector<Animation> animations = Animation::loadAllAnimations("models/arbol/trigo.fbx", trigo->GetBoneInfoMap(), trigo->getBonesInfo(), trigo->GetBoneCount());
			for (Animation animation : animations)
				trigo->setAnimator(Animator(animation));
			trigo->setAnimation(0);
		}
		catch (...) {
			ERRORL("Could not load wheat animation!", "ANIMACION");
		}

		trigo->setScale(&scale);
		trigo->setTranslate(&pos);
		trigo->setNextTranslate(&pos);
		trigo->walkeable = false;
		trigo->ignoreAABB = true;
		ourModel.emplace_back(trigo);

		for (int i = 1; i < cantidadTrigo; i++) {
			x = distribucionX(generador);
			z = distribucionZ(generador);
			ye = terreno->Superficie(x, z);
			pos = glm::vec3(x, ye, z);

			Model* clon = trigo->clone();
			clon->setScale(&scale);
			clon->setTranslate(&pos);
			clon->setNextTranslate(&pos);
			clon->walkeable = false;
			ourModel.emplace_back(clon);
		}

		ye = terreno->Superficie(-9, -15) + 4;
	}

	void Scenario::inicializaPollos() {
		const int cantidadPollos = 25;
		const float limiteX = 190.0f;
		const float limiteZ = 190.0f;

		std::random_device semilla;
		std::mt19937 generador(semilla());
		std::uniform_real_distribution<float> distribucionX(-limiteX, limiteX);
		std::uniform_real_distribution<float> distribucionZ(-limiteZ, limiteZ);

		glm::vec3 scale = glm::vec3(1.5f, 1.5f, 1.5f);

		float x = distribucionX(generador);
		float z = distribucionZ(generador);
		float ye = terreno->Superficie(x, z);
		glm::vec3 pos(x, ye, z);

		Model* pollo = new Model("models/Animales/POLLITO.fbx", camara->cameraDetails);
		try {
			std::vector<Animation> animations = Animation::loadAllAnimations("models/Animales/POLLITO.fbx", pollo->GetBoneInfoMap(), pollo->getBonesInfo(), pollo->GetBoneCount());
			for (Animation animation : animations)
				pollo->setAnimator(Animator(animation));
			pollo->setAnimation(0);
		}
		catch (...) {
			ERRORL("Could not load pollos animation!", "ANIMACION");
		}

		pollo->setScale(&scale);
		pollo->setTranslate(&pos);
		pollo->setNextTranslate(&pos);
		pollo->walkeable = false;
		pollo->ignoreAABB = true;
		ourModel.emplace_back(pollo);

		for (int i = 1; i < cantidadPollos; i++) {
			x = distribucionX(generador);
			z = distribucionZ(generador);
			ye = terreno->Superficie(x, z);
			pos = glm::vec3(x, ye, z);

			Model* clon = pollo->clone();
			clon->setScale(&scale);
			clon->setTranslate(&pos);
			clon->setNextTranslate(&pos);
			clon->walkeable = false;
			ourModel.emplace_back(clon);
		}

		ye = terreno->Superficie(-9, -15) + 4;
	}

	void Scenario::inicializaPuercos() {
		const int cantidadPuercos = 10;
		const float limiteX = 190.0f;
		const float limiteZ = 190.0f;

		std::random_device semilla;
		std::mt19937 generador(semilla());
		std::uniform_real_distribution<float> distribucionX(-limiteX, limiteX);
		std::uniform_real_distribution<float> distribucionZ(-limiteZ, limiteZ);

		glm::vec3 scale = glm::vec3(1.5f, 1.5f, 1.5f);

		float x = distribucionX(generador);
		float z = distribucionZ(generador);
		float ye = terreno->Superficie(x, z);
		glm::vec3 pos(x, ye, z);

		Model* puerco = new Model("models/Animales/PUERCO.fbx", camara->cameraDetails);
		try {
			std::vector<Animation> animations = Animation::loadAllAnimations("models/Animales/PUERCO.fbx", puerco->GetBoneInfoMap(), puerco->getBonesInfo(), puerco->GetBoneCount());
			for (Animation animation : animations)
				puerco->setAnimator(Animator(animation));
			puerco->setAnimation(0);
		}
		catch (...) {
			ERRORL("Could not load puerco animation!", "ANIMACION");
		}

		puerco->setScale(&scale);
		puerco->setTranslate(&pos);
		puerco->setNextTranslate(&pos);
		puerco->walkeable = false;
		puerco->ignoreAABB = true;
		ourModel.emplace_back(puerco);

		for (int i = 1; i < cantidadPuercos; i++) {
			x = distribucionX(generador);
			z = distribucionZ(generador);
			ye = terreno->Superficie(x, z);
			pos = glm::vec3(x, ye, z);

			Model* clon = puerco->clone();
			clon->setScale(&scale);
			clon->setTranslate(&pos);
			clon->setNextTranslate(&pos);
			clon->walkeable = false;
			ourModel.emplace_back(clon);
		}

		ye = terreno->Superficie(-9, -15) + 4;
	}

	

	void Scenario::inicializaOvejas() {
		const int cantidadOvejas = 8;
		const float limiteX = 190.0f;
		const float limiteZ = 190.0f;

		std::random_device semilla;
		std::mt19937 generador(semilla());
		std::uniform_real_distribution<float> distribucionX(-limiteX, limiteX);
		std::uniform_real_distribution<float> distribucionZ(-limiteZ, limiteZ);

		glm::vec3 scale = glm::vec3(1.5f, 1.5f, 1.5f);

		float x = distribucionX(generador);
		float z = distribucionZ(generador);
		float ye = terreno->Superficie(x, z);
		glm::vec3 pos(x, ye, z);

		Model* oveja = new Model("models/Animales/OVEJA.fbx", camara->cameraDetails);
		try {
			std::vector<Animation> animations = Animation::loadAllAnimations("models/Animales/OVEJA.fbx", oveja->GetBoneInfoMap(), oveja->getBonesInfo(), oveja->GetBoneCount());
			for (Animation animation : animations)
				oveja->setAnimator(Animator(animation));
			oveja->setAnimation(0);
		}
		catch (...) {
			ERRORL("Could not load ovejas animation!", "ANIMACION");
		}

		oveja->setScale(&scale);
		oveja->setTranslate(&pos);
		oveja->setNextTranslate(&pos);
		oveja->walkeable = false;
		oveja->ignoreAABB = true;
		ourModel.emplace_back(oveja);

		for (int i = 1; i < cantidadOvejas; i++) {
			x = distribucionX(generador);
			z = distribucionZ(generador);
			ye = terreno->Superficie(x, z);
			pos = glm::vec3(x, ye, z);

			Model* clon = oveja->clone();
			clon->setScale(&scale);
			clon->setTranslate(&pos);
			clon->setNextTranslate(&pos);
			clon->walkeable = false;
			ourModel.emplace_back(clon);
		}

		ye = terreno->Superficie(-9, -15) + 4;
	}



int Scenario::update() {
	if (juegoFinalizado) return -1;

	for (Enemigo* enemigo : enemigos) {
		if (enemigo->getActive()) {
			float alturaTerreno = terreno->Superficie(
				enemigo->getNextTranslate()->x,
				enemigo->getNextTranslate()->z);
			enemigo->actualizar(gameTime.deltaTime, alturaTerreno, enemigos);
		}
	}

	for (int i = static_cast<int>(enemigos.size()) - 1; i >= 0; i--) {
		Enemigo* enemigo = enemigos[i];
		if (!enemigo->puedeEliminarse()) continue;

		auto modelo = std::find(ourModel.begin(), ourModel.end(), enemigo);
		if (modelo != ourModel.end())
			ourModel.erase(modelo);
		enemigos.erase(enemigos.begin() + i);
		enemigosDisponibles.emplace_back(enemigo);
		enemigo->setActive(false);

		if (temporizador != NULL && !temporizador->estaTerminado())
			crearEnemigo();
	}

	if (temporizador != NULL)
		temporizador->actualizar(gameTime.deltaTime);

	int resultado = Scene::update();

	if (!juegoFinalizado && vidaFlor != NULL) {
		Principal* principal = dynamic_cast<Principal*>(camara);
		for (Enemigo* enemigo : enemigos) {
			if (enemigo->getActive() && !enemigo->estaMuerto()
				&& enemigo->getObjetivo() == camara
				&& enemigo->estaEnRango() && enemigo->puedeAtacar()
				&& principal != NULL) {
				enemigo->atacar(camara);
				principal->restaVida();
				for (Texto* texto : ourText) {
					if (texto->name.compare("Vida") == 0) {
						std::wstring vida = L"VIDA: " + std::to_wstring(principal->getVida());
						texto->initTexto(vida);
						break;
					}
				}
				continue;
			}

			if (enemigo->getActive() && !enemigo->estaMuerto()
				&& enemigo->getObjetivo() == florObjetivo
				&& enemigo->estaEnRango() && enemigo->puedeAtacar()) {
				enemigo->atacar(florObjetivo);
				vidaFlor->recibirDano();
				actualizarVidaFlorEnPantalla();
				break;
			}
		}
	}

	Principal* principalFinal = dynamic_cast<Principal*>(camara);
	if (principalFinal != NULL && principalFinal->estaMuerto()) {
		juegoFinalizado = true;
		juegoGanado = false;
		if (temporizador != NULL) temporizador->pausar();
		mostrarResultado(L"PERDISTE");
	}
	else if (vidaFlor != NULL && vidaFlor->estaDestruida()) {
		juegoFinalizado = true;
		juegoGanado = false;
		if (temporizador != NULL) temporizador->pausar();
		mostrarResultado(L"PERDISTE");
	}
	else if (temporizador != NULL && temporizador->estaTerminado()) {
		juegoFinalizado = true;
		juegoGanado = true;
		mostrarResultado(L"GANASTE");
	}

	actualizarTemporizadorEnPantalla();
	return resultado;
}

void Scenario::actualizarVidaFlorEnPantalla() {
	if (vidaFlor == NULL) return;
	for (Texto* texto : ourText) {
		if (texto->name.compare("VidaFlor") == 0) {
			std::wstring vida = L"VIDA FLOR: " + std::to_wstring(vidaFlor->getVida());
			texto->initTexto(vida);
			break;
		}
	}
}

void Scenario::actualizarTemporizadorEnPantalla() {
	if (temporizador == NULL) return;
	for (Texto* texto : ourText) {
		if (texto->name.compare("TiempoRestante") == 0) {
			std::wstring tiempo = temporizador->getTexto();
			texto->initTexto(tiempo);
			break;
		}
	}
}

void Scenario::mostrarResultado(const wchar_t* mensaje) {
	for (Texto* texto : ourText) {
		if (texto->name.compare("Resultado") == 0) {
			texto->initTexto((WCHAR*)mensaje);
			return;
		}
	}

	Texto* resultado = new Texto((WCHAR*)mensaje, 0.75f, 0,
		SCR_WIDTH / 2.0f - 170.0f, SCR_HEIGHT / 2.0f, 0,
		camara->cameraDetails);
	resultado->name = "Resultado";
	ourText.emplace_back(resultado);
}

Scene* Scenario::Render() {
	clearScreen();
	// Decimos que dibuje la media esfera
	sky->Draw();
	// Ahora el terreno
	terreno->Draw();
	// Dibujamos cada billboard que este cargado en el arreglo de billboards.
	for (int i = 0; i < billBoard.size(); i++)
		billBoard[i]->Draw();
	for (int i = 0; i < billBoard2D.size(); i++)
		billBoard2D[i]->Draw();
	// Dibujamos cada modelo que este cargado en nuestro arreglo de modelos
	for (int i = 0; i < ourModel.size(); i++) {
		ourModel[i]->Draw();
	}
	for (int i = 0; i < ourText.size(); i++) {
		ourText[i]->Draw();
	}
		// Le decimos a winapi que haga el update en la ventana
	return this;
}
	
std::vector<Model*> *Scenario::getLoadedModels() {
	return &ourModel;
}
std::vector<Billboard*> *Scenario::getLoadedBillboards() {
	return &billBoard;
}
std::vector<BillboardAnimation*> *Scenario::getLoadedBillboardsAnimation(){
	return &billBoardAnim;
}
std::vector<Billboard2D*> *Scenario::getLoadedBillboards2D(){
	return &billBoard2D;
}
std::vector<Texto*> *Scenario::getLoadedText(){
	return &ourText;
}
Model* Scenario::getMainModel() {
	return this->camara;
}
void Scenario::setMainModel(Model* mainModel){
	this->camara = mainModel;
}
float Scenario::getAngulo() {
	return this->angulo;
}
void Scenario::setAngulo(float angulo) {
	this->angulo = angulo;
}
SkyDome* Scenario::getSky() {
	return sky;
}
Terreno* Scenario::getTerreno() {
	return terreno;
}

Scenario::~Scenario() {
	if (this->sky != NULL) {
		delete this->sky;
		this->sky = NULL;
	}
	if (this->terreno != NULL) {
		delete this->terreno;
		this->terreno = NULL;
	}
	if (billBoard.size() > 0)
		for (int i = 0; i < billBoard.size(); i++)
			delete billBoard[i];
	this->billBoard.clear();
	if (billBoard2D.size() > 0)
		for (int i = 0; i < billBoard2D.size(); i++)
			delete billBoard2D[i];
	this->billBoard2D.clear();
	if (ourText.size() > 0)
		for (int i = 0; i < ourText.size(); i++)
			if (!(ourText[i]->name.compare("FPSCounter") == 0 || ourText[i]->name.compare("Coordenadas") == 0))
				delete ourText[i];
	this->ourText.clear();
	if (ourModel.size() > 0)
		for (int i = 0; i < ourModel.size(); i++)
			if (ourModel[i] != camara)
			delete ourModel[i];
	this->ourModel.clear();
	this->enemigos.clear();
	for (Enemigo* enemigo : enemigosDisponibles)
		delete enemigo;
	this->enemigosDisponibles.clear();

	if (vidaFlor != NULL) {
		delete vidaFlor;
		vidaFlor = NULL;
	}
	if (temporizador != NULL) {
		delete temporizador;
		temporizador = NULL;
	}
}

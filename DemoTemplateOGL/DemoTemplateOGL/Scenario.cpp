#include "Scenario.h"
#include <random>
#ifdef __linux__ 
#define ZeroMemory(x,y) memset(x,0,y)
#define wcscpy_s(x,y,z) wcscpy(x,z)
#define wcscat_s(x,y,z) wcscat(x,z)
#endif

Scenario::Scenario(Camera *cam) {
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
	terreno = new Terreno((WCHAR*)L"skydome/TerrenoGranja.jpg", (WCHAR*)L"skydome/Grass2.jpg", 400, 400, main->cameraDetails);
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

	/*
	Model* muro1 = new Model("models/arbol/MURO.fbx", main->cameraDetails);
	translate = glm::vec3(200.0f, 7.0f, 0.0f);
	scale = glm::vec3(1.0f, 200.0f, 20.0f);
	muro1->setTranslate(&translate);
	muro1->setNextTranslate(&translate);
	muro1->setScale(&scale);
	muro1->setRotX(90.0f);
	muro1->setNextRotX(90.0f);
	muro1->ignoreAABB = false;
	muro1->walkeable = false;
	ourModel.emplace_back(muro1);

	Model* muro2 = new Model("models/arbol/MURO.fbx", main->cameraDetails);
	translate = glm::vec3(-200.0f, 7.0f, 0.0f);
	scale = glm::vec3(1.0f, 200.0f, 20.0f);
	muro2->setTranslate(&translate);
	muro2->setNextTranslate(&translate);
	muro2->setScale(&scale);
	muro2->setRotX(90.0f);
	muro2->setNextRotX(90.0f);
	muro2->ignoreAABB = false;
	muro2->walkeable = false;
	ourModel.emplace_back(muro2);

	Model* muro3 = new Model("models/arbol/MURO.fbx", main->cameraDetails);
	translate = glm::vec3(0.0f, 7.0f, 200.0f);
	scale = glm::vec3(1.0f, 200.0f, 20.0f);
	muro3->setTranslate(&translate);
	muro3->setNextTranslate(&translate);
	muro3->setScale(&scale);
	muro3->setRotX(90.0f);
	muro3->setNextRotX(90.0f);
	muro3->setRotY(90.0f);
	muro3->setNextRotY(90.0f);
	muro3->ignoreAABB = false;
	muro3->walkeable = false;
	ourModel.emplace_back(muro3);

	Model* muro4 = new Model("models/arbol/MURO.fbx", main->cameraDetails);
	translate = glm::vec3(0.0f, 7.0f, -200.0f);
	scale = glm::vec3(1.0f, 200.0f, 20.0f);
	muro4->setTranslate(&translate);
	muro4->setNextTranslate(&translate);
	muro4->setScale(&scale);
	muro4->setRotX(90.0f);
	muro4->setNextRotX(90.0f);
	muro4->setRotY(90.0f);
	muro4->setNextRotY(90.0f);
	muro4->ignoreAABB = false;
	muro4->walkeable = false;
	ourModel.emplace_back(muro4);
	*/

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

	Model* hay2 = new Model("models/Casa/HAYBALE.fbx", main->cameraDetails);
	translate = glm::vec3(168.5f, 6.0f, -124.5f);
	scale = glm::vec3(2.0f, 2.0f, 3.0f);
	hay2->setTranslate(&translate);
	hay2->setNextTranslate(&translate);
	hay2->setScale(&scale);
	hay2->setRotZ(270.0f);
	hay2->setNextRotZ(270.0f);
	hay2->ignoreAABB = false;
	hay2->walkeable = false;
	ourModel.emplace_back(hay2);

	Model* hay3 = new Model("models/Casa/HAYBALE.fbx", main->cameraDetails);
	translate = glm::vec3(168.5f, 6.0f, -118.5f);
	scale = glm::vec3(2.0f, 2.0f, 3.0f);
	hay3->setTranslate(&translate);
	hay3->setNextTranslate(&translate);
	hay3->setScale(&scale);
	hay3->setRotZ(270.0f);
	hay3->setNextRotZ(270.0f);
	hay3->ignoreAABB = false;
	hay3->walkeable = false;
	ourModel.emplace_back(hay3);


	Model* casa = new Model("models/Casa/CASA.fbx", main->cameraDetails);
	translate = glm::vec3(159.5f, 16.0f, -121.5f);
	scale = glm::vec3(0.12f, 0.12f, 0.12f);
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

	Model* mesa = new Model("models/Casa/MESA.fbx", main->cameraDetails);
	translate = glm::vec3(159.5f, 7.0f, -121.5f);
	scale = glm::vec3(5.0f, 5.0f, 2.0f);
	mesa->setTranslate(&translate);
	mesa->setNextTranslate(&translate);
	mesa->setScale(&scale);
	mesa->setRotX(270.0f);
	mesa->setNextRotX(270.0f);
	mesa->setRotZ(270.0f);
	mesa->setNextRotZ(270.0f);
	mesa->ignoreAABB = false;
	mesa->walkeable = false;
	ourModel.emplace_back(mesa);

	Model* aid = new Model("models/Casa/AID.fbx", main->cameraDetails);
	translate = glm::vec3(159.5f, 8.0f, -121.5f);
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
	inicializaVacas();
	inicializaOvejas();
	std::wstring prueba(L"Esta es una prueba");
	ourText.emplace_back(new Texto(prueba, 0.30, 0, 0, SCR_HEIGHT, 0, model->cameraDetails));
	billBoard2D.emplace_back(new Billboard2D((WCHAR*)L"billboards/awesomeface.png", 6, 6, 100, 200, 0, camara->cameraDetails));
	scale = glm::vec3(100.0f, 100.0f, 0.0f);
	billBoard2D.back()->setScale(&scale);
	}

	void Scenario::inicializaArboles() {
		const int cantidadArboles = 70;
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

	void Scenario::inicializaVacas() {
		const int cantidadVacas = 8;
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

		Model* vaca = new Model("models/Animales/VACA.fbx", camara->cameraDetails);
		try {
			std::vector<Animation> animations = Animation::loadAllAnimations("models/Animales/VACA.fbx", vaca->GetBoneInfoMap(), vaca->getBonesInfo(), vaca->GetBoneCount());
			for (Animation animation : animations)
				vaca->setAnimator(Animator(animation));
			vaca->setAnimation(0);
		}
		catch (...) {
			ERRORL("Could not load vacas animation!", "ANIMACION");
		}

		vaca->setScale(&scale);
		vaca->setTranslate(&pos);
		vaca->setNextTranslate(&pos);
		vaca->walkeable = false;
		vaca->ignoreAABB = true;
		ourModel.emplace_back(vaca);

		for (int i = 1; i < cantidadVacas; i++) {
			x = distribucionX(generador);
			z = distribucionZ(generador);
			ye = terreno->Superficie(x, z);
			pos = glm::vec3(x, ye, z);

			Model* clon = vaca->clone();
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
}

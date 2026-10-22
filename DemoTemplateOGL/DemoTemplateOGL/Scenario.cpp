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


	model = new Model("models/dancing_vampire/dancing_vampire.dae", main->cameraDetails);
	translate = glm::vec3(0.0f, terreno->Superficie(0.0f, 60.0f), 60.0f);
	scale = glm::vec3(0.02f, 0.02f, 0.02f);	// it's a bit too big for our scene, so scale it down
	model->setTranslate(&translate);
	model->setNextTranslate(&translate);
	model->setScale(&scale);
	model->setNextRotY(90);
	ourModel.emplace_back(model);
	try{
		std::vector<Animation> animations = Animation::loadAllAnimations("models/dancing_vampire/dancing_vampire.dae", model->GetBoneInfoMap(), model->getBonesInfo(), model->GetBoneCount());
		std::vector<Animation> animation = Animation::loadAllAnimations("models/dancing_vampire/dancing_vampire.dae", model->GetBoneInfoMap(), model->getBonesInfo(), model->GetBoneCount());
		std::move(animation.begin(), animation.end(), std::back_inserter(animations));
		for (Animation animation : animations)
			model->setAnimator(Animator(animation));
		model->setAnimation(1);
	}catch(...){
		ERRORL("Could not load animation!", "ANIMACION");
	}


	Model* agua = new Model("models/agua.fbx", main->cameraDetails);
	agua->ignoreAABB = true;
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

	inicializaBillboards();
	std::wstring prueba(L"Esta es una prueba");
	ourText.emplace_back(new Texto(prueba, 0.30, 0, 0, SCR_HEIGHT, 0, model->cameraDetails));
	billBoard2D.emplace_back(new Billboard2D((WCHAR*)L"billboards/awesomeface.png", 6, 6, 100, 200, 0, camara->cameraDetails));
	scale = glm::vec3(100.0f, 100.0f, 0.0f);	// it's a bit too big for our scene, so scale it down
	billBoard2D.back()->setScale(&scale);
	}

	// ANIMACION DEL ARBOL?

void Scenario::inicializaBillboards() {
	const int cantidadArboles = 70;
	const float limiteX = 190.0f;
	const float limiteZ = 190.0f;
	const float alturaArbol = 10.5f;

	std::random_device semilla;
	std::mt19937 generador(semilla());
	std::uniform_real_distribution<float> distribucionX(-limiteX, limiteX);
	std::uniform_real_distribution<float> distribucionZ(-limiteZ, limiteZ);

	float x = distribucionX(generador);
	float z = distribucionZ(generador);
	float ye = terreno->Superficie(x, z) + alturaArbol;

	std::vector<std::wstring> texturasPino = {
		L"billboards/Pino1.png",
		L"billboards/Pino2.png"
	};
	arbolPino = new spritesec(texturasPino, 16, 22, x, ye, z, camara->cameraDetails);
	billBoard.emplace_back(arbolPino);

	for (int i = 1; i < cantidadArboles; i++) {
		x = distribucionX(generador);
		z = distribucionZ(generador);
		ye = terreno->Superficie(x, z) + alturaArbol;

		ModelAttributes mAttr;
		glm::vec3 pos(x, ye, z);
		mAttr.setTranslate(&pos);
		arbolPino->getModelAttributes()->push_back(mAttr);
	}

	ye = terreno->Superficie(-9, -15) + 4;
	billBoard.emplace_back(new Billboard((WCHAR*)L"billboards/Arbol3.png", 8, 8, -9, ye, -15, camara->cameraDetails));
}

	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
Scene* Scenario::Render() {
	clearScreen();

	tiempoAnimacionPino += gameTime.deltaTime;
	if (tiempoAnimacionPino >= 800.0 && arbolPino != nullptr) {
		tiempoAnimacionPino -= 800.0;
		arbolPino->siguienteTextura();
	}
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

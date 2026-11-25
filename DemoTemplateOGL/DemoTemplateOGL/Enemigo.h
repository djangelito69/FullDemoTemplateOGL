#pragma once
#include "Base/model.h"

class Enemigo : public Model {
public:
	enum class Estado {
		Corriendo,
		Atacando,
		RecibiendoDano,
		Muerto
	};

private:
	int vida;
	int vidaMaxima;
	int danoAtaque;
	float velocidad;
	float rangoAtaque;
	float tiempoEntreAtaques;
	float tiempoDesdeUltimoAtaque;
	float tiempoMuerte;
	Estado estado;
	Model* objetivo;
	Model* objetivoPrincipal;
	Model* objetivoAlternativo;
	float radioDeteccionJugador;
	bool siguiendoJugador;

public:
	Enemigo(Camera* camera, Model* objetivo,
		const std::string& modelo = "models/Enemigo/Enemy_Idle.fbx",
		int vidaMaxima = 100,
		int danoAtaque = 10,
		float velocidad = 8.0f,
		float rangoAtaque = 12.0f)
		: Model(modelo, camera), vidaMaxima(vidaMaxima), vida(vidaMaxima),
		danoAtaque(danoAtaque), velocidad(velocidad), rangoAtaque(rangoAtaque),
		tiempoEntreAtaques(1.0f), tiempoDesdeUltimoAtaque(0.0f),
		tiempoMuerte(0.0f), estado(Estado::Corriendo), objetivo(objetivo),
		objetivoPrincipal(objetivo), objetivoAlternativo(NULL),
		radioDeteccionJugador(35.0f), siguiendoJugador(false) {
		name = "Enemigo";
		ignoreAABB = false;
		walkeable = false;
		cargarTodasLasAnimaciones();
	}

	virtual ~Enemigo() {}

	int getVida() const { return vida; }
	int getVidaMaxima() const { return vidaMaxima; }
	int getDanoAtaque() const { return danoAtaque; }
	float getVelocidad() const { return velocidad; }
	float getRangoAtaque() const { return rangoAtaque; }
	Estado getEstado() const { return estado; }
	Model* getObjetivo() const { return objetivo; }
	Model* getObjetivoPrincipal() const { return objetivoPrincipal; }
	float getRadioDeteccionJugador() const { return radioDeteccionJugador; }
	bool estaSiguiendoJugador() const { return siguiendoJugador; }
	void setObjetivoAlternativo(Model* jugador) { objetivoAlternativo = jugador; }
	void setRadioDeteccionJugador(float radio) { radioDeteccionJugador = radio >= 0 ? radio : 0; }
	bool estaMuerto() const { return estado == Estado::Muerto; }
	bool puedeEliminarse() const { return estado == Estado::Muerto && tiempoMuerte >= 2.0f; }
	void reiniciarTemporizadorMuerte() { tiempoMuerte = 0.0f; }

	void setVida(int vidaNueva) {
		if (vidaNueva < 0) vidaNueva = 0;
		if (vidaNueva > vidaMaxima) vidaNueva = vidaMaxima;
		vida = vidaNueva;
		if (vida == 0) morir();
	}

	void setVidaMaxima(int vidaNueva) {
		vidaMaxima = vidaNueva > 0 ? vidaNueva : 1;
		if (vida > vidaMaxima) vida = vidaMaxima;
	}

	void setDanoAtaque(int dano) { danoAtaque = dano >= 0 ? dano : 0; }
	void setVelocidad(float nuevaVelocidad) { velocidad = nuevaVelocidad >= 0 ? nuevaVelocidad : 0; }
	void setRangoAtaque(float nuevoRango) { rangoAtaque = nuevoRango >= 0 ? nuevoRango : 0; }
	void setObjetivo(Model* nuevoObjetivo) {
		objetivo = nuevoObjetivo;
		objetivoPrincipal = nuevoObjetivo;
	}
	void reiniciar(Model* objetivoPrincipalNuevo, Model* objetivoAlternativoNuevo) {
		vida = vidaMaxima;
		tiempoDesdeUltimoAtaque = 0.0f;
		tiempoMuerte = 0.0f;
		objetivoPrincipal = objetivoPrincipalNuevo;
		objetivo = objetivoPrincipalNuevo;
		objetivoAlternativo = objetivoAlternativoNuevo;
		siguiendoJugador = false;
		estado = Estado::Corriendo;
		setActive(true);
		reproducirAnimacionSegunEstado();
	}
	bool puedeAtacar() const { return tiempoDesdeUltimoAtaque >= tiempoEntreAtaques; }
	bool estaEnRango() {
		if (objetivo == NULL) return false;
		glm::vec3 diferencia = *objetivo->getNextTranslate() - *getNextTranslate();
		diferencia.y = 0;
		return glm::length(diferencia) <= rangoAtaque;
	}
	void setEstado(Estado nuevoEstado) {
		if (estado == nuevoEstado) return;
		estado = nuevoEstado;
		reproducirAnimacionSegunEstado();
	}

	virtual void recibirDano(int cantidad) {
		if (estado == Estado::Muerto || cantidad <= 0) return;
		setEstado(Estado::RecibiendoDano);
		setVida(vida - cantidad);
	}

	virtual void atacar(Model* objetivoAtaque) {
		if (estado == Estado::Muerto) return;
		tiempoDesdeUltimoAtaque = 0.0f;
		setEstado(Estado::Atacando);
		(void)objetivoAtaque;
	}

	virtual void morir() {
		vida = 0;
		tiempoMuerte = 0.0f;
		setEstado(Estado::Muerto);
		setAnimation(2);
	}

	virtual void actualizar(float deltaMilisegundos, float alturaTerreno) {
		actualizar(deltaMilisegundos, alturaTerreno, std::vector<Enemigo*>());
	}

	virtual void actualizar(float deltaMilisegundos, float alturaTerreno,
		const std::vector<Enemigo*>& otrosEnemigos) {
		if (estado == Estado::Muerto) {
			tiempoMuerte += deltaMilisegundos / 1000.0f;
			setAnimation(2);
			return;
		}
		if (objetivoPrincipal == NULL) return;

		siguiendoJugador = false;
		objetivo = objetivoPrincipal;
		if (objetivoAlternativo != NULL) {
			glm::vec3 posicionJugador = *objetivoAlternativo->getNextTranslate();
			glm::vec3 posicionActual = *getNextTranslate();
			posicionJugador.y = posicionActual.y;
			if (glm::length(posicionJugador - posicionActual) <= radioDeteccionJugador) {
				objetivo = objetivoAlternativo;
				siguiendoJugador = true;
			}
		}

		tiempoDesdeUltimoAtaque += deltaMilisegundos / 1000.0f;

		glm::vec3 posicionActual = *getNextTranslate();
		glm::vec3 posicionObjetivo = *objetivo->getNextTranslate();

		glm::vec3 direccion = posicionObjetivo - posicionActual;
		direccion.y = 0;
		float distancia = glm::length(direccion);

		if (distancia <= rangoAtaque) {
			if (tiempoDesdeUltimoAtaque >= tiempoEntreAtaques)
				setEstado(Estado::Atacando);
		}
		else {
			if (estado != Estado::RecibiendoDano) setEstado(Estado::Corriendo);
			if (distancia > 0.001f) {
				direccion /= distancia;
				posicionActual += direccion * velocidad * deltaMilisegundos / 1000.0f;
			}

			glm::vec3 separacion(0.0f);
			for (Enemigo* otro : otrosEnemigos) {
				if (otro == NULL || otro == this || otro->estaMuerto()) continue;
				glm::vec3 diferencia = posicionActual - *otro->getNextTranslate();
				diferencia.y = 0;
				float distanciaEntreEnemigos = glm::length(diferencia);
				if (distanciaEntreEnemigos > 0.001f && distanciaEntreEnemigos < 10.0f) {
					separacion += glm::normalize(diferencia) * (10.0f - distanciaEntreEnemigos);
				}
			}
			posicionActual += separacion * deltaMilisegundos / 1000.0f;
			posicionActual.y = alturaTerreno;
			setNextTranslate(&posicionActual);

			float angulo = std::atan2(direccion.x, direccion.z) * 180.0f / 3.14159265f;
			setNextRotY(angulo);
		}
	}

	void cargarAnimacion(const std::string& ruta, unsigned int indiceAnimacion) {
		try {
			std::vector<Animation> animaciones = Animation::loadAllAnimations(
				ruta,
				GetBoneInfoMap(),
				getBonesInfo(),
				GetBoneCount());

			for (Animation& animacion : animaciones)
				setAnimator(Animator(animacion));

			if (!animaciones.empty())
				setAnimation(indiceAnimacion < animaciones.size() ? indiceAnimacion : 0);
		}
		catch (...) {
			ERRORL("No se pudo cargar una animacion del enemigo", "ENEMIGO");
		}
	}

private:
	void cargarTodasLasAnimaciones() {
		cargarAnimacion("models/Enemigo/Enemy_Idle.fbx", 0);
		cargarAnimacion("models/Enemigo/Enemy_Attack.fbx", 0);
		cargarAnimacion("models/Enemigo/Enemy_Death.fbx", 0);
		setAnimation(0);
	}

	void reproducirAnimacionSegunEstado() {
		switch (estado) {
		case Estado::Corriendo:
		case Estado::RecibiendoDano:
			setAnimation(0);
			break;
		case Estado::Atacando:
			setAnimation(1);
			break;
		case Estado::Muerto:
			setAnimation(2);
			break;
		}
	}
};
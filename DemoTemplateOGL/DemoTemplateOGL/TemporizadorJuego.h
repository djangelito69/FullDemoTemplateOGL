#pragma once
#include "Base/Utilities.h"
#include <sstream>
#include <iomanip>

class TemporizadorJuego {
private:
	float duracionSegundos;
	float tiempoRestanteSegundos;
	bool pausado;
	bool terminado;

public:
	TemporizadorJuego(float duracionSegundos = 180.0f)
		: duracionSegundos(duracionSegundos > 0 ? duracionSegundos : 180.0f),
		tiempoRestanteSegundos(duracionSegundos), pausado(false), terminado(false) {}

	virtual ~TemporizadorJuego() {}

	float getTiempoRestante() const { return tiempoRestanteSegundos; }
	float getDuracion() const { return duracionSegundos; }
	bool estaPausado() const { return pausado; }
	bool estaTerminado() const { return terminado; }

	void setDuracion(float nuevaDuracion) {
		duracionSegundos = nuevaDuracion > 0 ? nuevaDuracion : 180.0f;
		reiniciar();
	}

	void setPausado(bool nuevoEstado) { pausado = nuevoEstado; }

	virtual void actualizar(double deltaMilisegundos) {
		if (pausado || terminado) return;

		tiempoRestanteSegundos -= static_cast<float>(deltaMilisegundos / 1000.0);
		if (tiempoRestanteSegundos <= 0.0f) {
			tiempoRestanteSegundos = 0.0f;
			terminado = true;
			pausado = true;
		}
	}

	virtual void reiniciar() {
		tiempoRestanteSegundos = duracionSegundos;
		pausado = false;
		terminado = false;
	}

	void pausar() { pausado = true; }
	void continuar() {
		if (!terminado) pausado = false;
	}

	std::wstring getTexto() const {
		int segundos = static_cast<int>(tiempoRestanteSegundos);
		if (segundos < 0) segundos = 0;
		int minutos = segundos / 60;
		segundos %= 60;

		std::wstringstream texto;
		texto << L"TIEMPO: "
			<< std::setw(2) << std::setfill(L'0') << minutos
			<< L":"
			<< std::setw(2) << std::setfill(L'0') << segundos;
		return texto.str();
	}
};
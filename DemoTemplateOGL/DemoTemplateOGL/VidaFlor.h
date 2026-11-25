#pragma once

class VidaFlor {
private:
	int vida;
	int vidaMaxima;
	int danoPorAtaque;

public:
	VidaFlor(int vidaMaxima = 500, int danoPorAtaque = 20)
		: vidaMaxima(vidaMaxima > 0 ? vidaMaxima : 500),
		danoPorAtaque(danoPorAtaque >= 0 ? danoPorAtaque : 20) {
		vida = this->vidaMaxima;
	}

	virtual ~VidaFlor() {}

	int getVida() const { return vida; }
	int getVidaMaxima() const { return vidaMaxima; }
	int getDanoPorAtaque() const { return danoPorAtaque; }

	void setVida(int nuevaVida) {
		if (nuevaVida < 0) nuevaVida = 0;
		if (nuevaVida > vidaMaxima) nuevaVida = vidaMaxima;
		vida = nuevaVida;
	}

	void setVidaMaxima(int nuevaVidaMaxima) {
		vidaMaxima = nuevaVidaMaxima > 0 ? nuevaVidaMaxima : 1;
		if (vida > vidaMaxima) vida = vidaMaxima;
	}

	void setDanoPorAtaque(int dano) {
		danoPorAtaque = dano >= 0 ? dano : 0;
	}

	virtual int recibirDano() {
		return recibirDano(danoPorAtaque);
	}

	virtual int recibirDano(int cantidad) {
		if (estaDestruida() || cantidad <= 0) return vida;
		setVida(vida - cantidad);
		return vida;
	}

	virtual void restaurarVida() {
		vida = vidaMaxima;
	}

	bool estaDestruida() const {
		return vida <= 0;
	}
};
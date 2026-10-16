#pragma once

#include "Base/Billboard.h"

class spritesec : public Billboard {
private:
	std::vector<std::wstring> texturas;
	std::size_t indiceTextura = 0;

public:
	spritesec(const std::vector<std::wstring>& texturas, float ancho, float alto,
		float x, float y, float z, Camera* camera)
		: Billboard(const_cast<WCHAR*>(texturas.at(0).c_str()), ancho, alto, x, y, z, camera),
		texturas(texturas) {
	}

	void siguienteTextura() {
		if (texturas.size() < 2)
			return;

		indiceTextura = (indiceTextura + 1) % texturas.size();
		setTexture(const_cast<WCHAR*>(texturas[indiceTextura].c_str()));
	}

	std::size_t getIndiceTextura() const {
		return indiceTextura;
	}
};
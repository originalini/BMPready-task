#include "BMPready.h" 

int my_sign(int num) {
	return (num > 0) - (num < 0);
}

BMPready::BMPready() : bitMapFileheader{}, bitMapInfoHeader{}, path{}, pixel_arr{} {}

BMPready::~BMPready() {
	if (fileBMP.is_open()) {
		fileBMP.close();
	}

	if (newFileBMP.is_open()) {
		newFileBMP.close();
	}
}

int BMPready::getHeight() {
	return bitMapInfoHeader.biHeight;
}

int BMPready::getWidth() {
	return bitMapInfoHeader.biWidth;
}

void BMPready::drawline(int x1,int y1,int x2,int y2) {
	int dx = x2 - x1; // направление движения вправо или влево
	int dy = y2 - y1; // направление движения верх или вниз

	int sx = my_sign(dx); // куда именно делаем шак вправо или влево
	int sy = my_sign(dy); // куда именно делаем шаг вверх или вниз

	dx = std::abs(dx); // абсолютные величины значений dx грубо говоря модуль числа
	dy = std::abs(dy); // абсолютные величины значений dy грубо говоря модуль числа

	int x = x1;
	int y = y1;

	int err = dx - dy; // это ошибка

	while (true) {
		int index = y * bitMapInfoHeader.biWidth + x;
		pixel_arr[index] = { 0,0,0 };
		
		if (x == x2 && y == y2) {
			break;
		}

		if (dx > dy) {
			x += sx;
			err -= dy;

			if (err < 0) {
				y += sy;
				err += dx;
			}
		}
		else {
			y += sy;
			err -= dx;

			if (err < 0) {
				x += sx;
				err += dy;
			}
		}
	}
}

bool BMPready::loadedFile(std::string path) {
	std::vector<unsigned char> ColorFile;

	this->path = path;
	fileBMP.open(path, std::fstream::in | std::fstream::binary);
	if (fileBMP.is_open()) {
		fileBMP.read((char*)&bitMapFileheader, sizeof(BitMapFileHeader));
		fileBMP.read((char*)&bitMapInfoHeader, sizeof(BitMapInfoHeader));

		if (bitMapInfoHeader.biBitCount != 24) {
			std::cout << "Error: Unsupported bit count: " << bitMapInfoHeader.biBitCount << std::endl;
			fileBMP.close();
			return false;
		}

		ColorFile.resize(bitMapInfoHeader.biSizeImage);
		pixel_arr.resize(ColorFile.size() / 3);

		fileBMP.seekg(bitMapFileheader.bfOffBits);
		fileBMP.read((char*)ColorFile.data(), bitMapInfoHeader.biSizeImage);

		// Закидываем цвета в стркутуру ColorRGB
		for (int i = 0, j = 0; j < (ColorFile.size() / 3) && i < ColorFile.size(); ++j, i += 3) {
			pixel_arr[j].B = ColorFile[i];
			pixel_arr[j].G = ColorFile[i + 1];
			pixel_arr[j].R = ColorFile[i + 2];
		}

		fileBMP.close();

		return true;
	}
	else {
		return false;
	}
}

void BMPready::displayImage() {
	unsigned char black_symbol = '@'; // Символ для черного.
	unsigned char white_symbol = '.'; // Символ для белого.

	int width = this->bitMapInfoHeader.biWidth;
	int height = this->bitMapInfoHeader.biHeight;

	int j = 0;
	for (int y = height - 1; y >= 0; y-=1) {
		for (int x = 0; x < width; x+=1) {
			// Вычисляем индекс в одномерном массиве.
			int index = y * width + x;

			if (pixel_arr[index].R == 0 && pixel_arr[index].G == 0  && pixel_arr[index].B == 0) {
				std::cout << black_symbol;
			}
			else {
				std::cout << white_symbol;
			}
		}
		std::cout << "\n";
	}
	std::cout << std::endl << std::endl;
}

void BMPready::saveEditImage(std::string name_file) {
	newFileBMP.open(name_file, std::fstream::binary | std::fstream::out);

	if (newFileBMP.is_open()) {
		std::cout << "Changes saved" << "\n";
	}
	else {
		std::cout << "Changes not saved" << "\n";
	}

	newFileBMP.seekp(0);
	newFileBMP.write((char*)&bitMapFileheader, sizeof(BitMapFileHeader));

	newFileBMP.seekp(sizeof(BitMapFileHeader));
	newFileBMP.write((char*)&bitMapInfoHeader, sizeof(BitMapInfoHeader));

	newFileBMP.seekp(bitMapFileheader.bfOffBits);

	newFileBMP.write((char*)pixel_arr.data(), bitMapInfoHeader.biSizeImage);

	newFileBMP.close();
}
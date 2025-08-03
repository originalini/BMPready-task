#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#pragma pack(push, 1)

struct BitMapFileHeader {
	unsigned short		bfType;
	unsigned int		bfSize;
	unsigned short		bfReserved1;
	unsigned short		bfReserved2;
	unsigned int		bfOffBits;
};


struct BitMapInfoHeader {
	unsigned long		biSize;
	long				biWidth;
	long				biHeight;
	unsigned short		biPlanes;
	unsigned short		biBitCount;
	unsigned long		biCompression;
	unsigned long		biSizeImage;
	long				biXPelsPerMeter;
	long				biYPelsPerMeter;
	unsigned long		biClrUsed;
	unsigned long		biClrImportant;
};

struct ColorRGB {
	unsigned char B;
	unsigned char G;
	unsigned char R;
};

#pragma pack(pop)

class BMPready {
private:
	BitMapFileHeader bitMapFileheader;
	BitMapInfoHeader bitMapInfoHeader;

	std::string path; // Путь до файла
	std::vector<ColorRGB> pixel_arr; // Массив пикселей изображения

	std::fstream fileBMP; // Оригинальный файл.
	std::fstream newFileBMP; // Копия файла для нарисования креста.
public:
	BMPready();
	~BMPready();

	// Загрузка файла
	bool loadedFile(std::string path);

	// Вывод изображения в консоль
	void displayImage();

	// Рисование линии по двум точкам.
	void drawline(int x1, int y1, int x2, int y2);

	// Сохранение изменений файла.
	void saveEditImage(std::string name_Newfile);

	// Данные функции getWidth и getHeight нужны для рисования креста.
	int getWidth();
	int getHeight();
};

#include "BMPready.h"

int main() {
	BMPready BMPfile;

	std::string path; // Полный путь(с именем и форматом) до файла с картинкой.
	std::string name_newFile; // Имя нового файла для сохранения.

	while (true) {
		std::cout << "Enter the full file path: ";
		std::cin >> path;

		if (BMPfile.loadedFile(path)) {
			std::cout << "File loaded successfully" << "\n";
			break;
		}
		else {
			std::cout << "Error: Failed to load file" << "\n";
		}
	}
	int Height = BMPfile.getHeight();
	int Width = BMPfile.getWidth();


	BMPfile.displayImage();
	BMPfile.drawline(0, 0, (Width - 1), (Height - 1));
	BMPfile.drawline(0, (Height - 1), (Width - 1), 0);
	BMPfile.displayImage();
	
	std::cout << "Enter a file name to save the changes: ";
	std::cin >> name_newFile;
	BMPfile.saveEditImage(name_newFile);

	return 0;
}

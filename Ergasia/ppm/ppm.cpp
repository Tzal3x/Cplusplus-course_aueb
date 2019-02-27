#include "ppm.h"
#include <fstream>
#include <iostream>



using namespace imaging;



float * imaging::ReadPPM(const char * filename, int * w, int * h) {


	std::ifstream infile;

	infile.open(filename, std::ifstream::binary);

	if (!infile.is_open()) {
		std::cerr << "~[Error] /readPPM()/ : Failed to open PPM file!" << std::endl;
		return nullptr;
	}

	/*------------------*/
	int counter = 0;
	while (counter < 4) { /*Skiping Header and #comments*/

		if (infile.good()) {

			if (infile.peek() == '#') { //Skipping Comments
				infile.get();
				while (infile.peek() != '\n') {
					infile.get();
				}
				infile.get();
			}
			else if (infile.peek() == ' ' || infile.peek() == '\n') {
				counter++;
				infile.get();
			}
			else {
				infile.get();
			}
		}
		
	}

	
	/*---------Reading_Bytes-------------------------*/
	int size = (*w)*(*h) * 3;
	
	char * bytes = new char[size];
	infile.read(bytes, size);
	infile.close();

	if (!infile.good()) {
		std::cerr << "~[Error] /readPPM()/ : Error while reading Image's data , ["<<*w<<"] x ["<<*h<<"] x [3] size isn't compatible with the reading block !"<<std::endl;
		std::cerr << "               EOF reached at invalid position !" << std::endl;
		return nullptr;
	}

	if (infile.peek() != EOF) { /* Checking if next byte is EOF */
		std::cerr << "~[Error] /readPPM()/ : Error occured while reading Image's data , ["<<*w<<"] x ["<<*h<<"] x [3] size isn't compatible with the reading block !" << std::endl << std::endl;
		return nullptr;
	}

	/*---------Casting Char * bytes to float * Array-------------*/
	float* f_array = new float[size];

	for (int i = 0; i < size; i++) {
		
		f_array[i] = ((float)((unsigned char)bytes[i]))/255.0f;
	}

	//deleting
	delete[] bytes;
	//~


#ifdef _DEBUG //debugging
	std::cout << "-[Report] : Total Bytes read : " << size << std::endl << "-[Report] : Total Pixels read : " << size/3 << std::endl << std::endl;
#endif//


	return f_array;
}




bool imaging::WritePPM(const float * data, int w, int h, const char * filename) {

	if (data == nullptr) {
		std::cerr << "~[Error] /WritePPM()/ : float * data hasn't been initialised / <<points to nullptr>> !" << std::endl << std::endl;
		return false;
	}

	/* Intialising char * bytes Array that will help perform the writing procedure faster */
	char * bytes = new char[w*h * 3];
	for (int i = 0; i < w*h * 3; i++) {
		bytes[i] = (char)data[i];
	}

	std::ofstream outfile;
	outfile.open(filename, std::ios::binary);

	if (!outfile.is_open()) {
		std::cerr << "~[Error] /WritePPM()/ : Error occured while trying to open new File !" << std::endl << std::endl;
		return false;
	}

	/*Writing Header*/
	outfile << "P6"<<" "<< w << "\n" << h << " "<< "255\n";
	outfile.write(&bytes[0], w*h * 3);
	outfile.close();

	if (!outfile.good()) {
		std::cerr << "~[Error] /writePPM()/ : Error occured while writing data to the requested <ppm> file !" << std::endl;
		return false;
	}

	delete[] bytes;


	return true;
}

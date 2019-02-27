#pragma once
#include <iostream>
#include <fstream>
#include <string>
//#include <math.h>
#include "Vec3.h"
#include "ppm\ppm.h"
#include <vector>

namespace math {

	template<typename T> class Array {

	protected:
		std::vector< T > buffer; //! Holds the image data.

		unsigned int width, //! The width of the image (in pixels)
			height;		   //! The height of the image (in pixels)

	public:
		std::vector<T>* getRawDataPtr() {
			return &buffer;
		}



		//---------------------- get object ----------------------------------------------------------------------------------------------------------
		T getit(unsigned int x, unsigned int y) const {

			if (x*width + y >= height * width) {

				std::cerr << "~[Error] /getPixel()/ : Requested [" << x << "][" << y << "] {R,G,B} is out of expected [" << width << "] x [" << height << "] bounds !" << std::endl;
				return T(0.0f, 0.0f, 0.0f);
			}

			else if (buffer.empty()) {

				std::cerr << "~[Error] /getPixel()/ : Error occured while trying to access buffer[" << x << "][" << y << "] !\n      Color * buffer hasn't been initialised !" << std::endl << std::endl;
				system("pause");
				return T(0.0f, 0.0f, 0.0f);
			}

			return buffer[x*width + y];


		}

		//-------------------------------- set object --------------------------------------------------------------------------------------------------------------------------
		void setit(unsigned int x, unsigned int y, T & value) {

			if (x*width + y >= height * width) {

				std::cerr << "~[Error] /setPixel()/ : Requested [" << x << "][" << y << "] {R,G,B} is out of expected [" << width << "] x [" << height << "] bounds !" << std::endl;
				system("pause");
			}
			else if (buffer.empty()) { // .empty() epistrefei true an o vector einai kenos.
				std::cerr << "~[Error] /setPixel()/ : Error occured while trying to access buffer[" << x << "][" << y << "] !" << std::endl << std::endl;
				system("pause");

			}
			else {
				buffer[x*width + y] = value;
			}
		}

		//------------------------------- get object by coordinates -----------------------------------------------------------------------------------------------------------
		//T & operator () (size_t index,size_t index2) // x = index = platos/width ,  h = index2 = ypsos/height
		//{	
		//	int w = static_cast<int>(index); 
		//	int h = static_cast<int>(index2);

		//	if (  h*width+w >= width*height ) {
		//		cout << "~[Error] in () operator, you are out of bounds!" << endl;
		//		cout << "- width coordinate = " << index << endl;
		//		cout << "- height coordinate = " << index2 << endl;
		//		system("pause");
		//		exit(1);
		//	}

		//	return *(&buffer[0] + index*w +index2);

		//}
		
		//dikos tou
		T & operator () (size_t index0, size_t index1)
		{
			int w = static_cast<int>(index0);
			int h = static_cast<int>(index1);

			if (w*width + h >= height*width) {
				std::cerr << "~[Error] /operator()/ : Got out-of-bounds !" << std::endl;
				system("pause");
				exit(1);
			}
			return *(&buffer[0] + index0*width + index1);
		}


		/*! Returns the width of the image
		*/
		const unsigned int getWidth() const { return width; }

		/*! Returns the height of the image
		*/
		const unsigned int getHeight() const { return height; }


		//------------------------- setData ----------------------------------------------------------------------------------------------------------------------------------
		void setData(const std::vector<T> &data_ptr) {


			if (width*height == 0) {
				std::cerr << "~[Error] /setData()/ : Error while trying to pass data to Color * Image::Buffer , [" << width << "]x[" << height << "] <= 0 " << std::endl;
				system("pause");

			}
			else if (buffer.empty()) { /* Color * buffer should already be allocated */
				std::cerr << "~[Error] /setData()/ : Error while trying to initialise Color * Image::Buffer , Image::Buffer has not been allocated ! " << std::endl;
				system("pause");

			}

			else {

				for (unsigned int i = 0; i < width*height; i++) {
					buffer[i] = data_ptr[i];
				}

#ifdef _DEBUG //debugging
				std::cout << "-[Report] : Color * Buffer initialised succesfully!" << std::endl << std::endl;
#endif//
			}
		}




		/*-------Default_Constructor---------*/
		Array()
			: width(0), height(0)
		{
			
		}

		/*-------Constructor---------*/
		Array(unsigned int width, unsigned int height) {

			this->width = width;
			this->height = height;
		}


		/*-------Constructor---------*/
		Array(unsigned int width, unsigned int height, const T * data_ptr) {

			this->width = width;
			this->height = height;

			buffer.reserve(width*height);
			for (unsigned int i = 0; i < width*height; i++) {
				this->buffer[i] = data_ptr[i];
			}
			
		}



		/*-------Copy_Constructor--------*/
		Array(const Array &src)
			:width(src.width), height(src.height) ,buffer(src.buffer) {

			/*buffer.reserve(src.width*src.height);
			for (unsigned int i = 0; i < width*height; i++) {
				buffer.push_back(src.buffer[i]);
			}
			*/
		}


		/*--------Destructor---------*/
		~Array() {

			
			this->width = 0;
			this->height = 0;
			

			buffer.clear();

		}
	};

} // end math namespace









/*+++++++++++++++++++++++++++++++++++ PARAGWDAS THN IMAGE: ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
namespace imaging {

	class Image :public math::Array< math::Vec3<float> > {

		typedef math::Vec3<float> Color;


	public:

		bool Image::load(const std::string & filename, const std::string & format) {


			/*Checking if File has the 'ppm' Extension*/
			if (format != "ppm") {

				std::cerr << "~[Error] /load()/ : Filie is not a supportable Format [Supportable format : ppm ] !" << std::endl;
				system("pause");
				return false;
			}

			std::ifstream infile;
			infile.open(filename, std::ifstream::binary);

			/*Checking if file is open!*/
			if (!infile.is_open()) {

				std::cerr << "~[Error] /load()/ : Failed to open requested file , file maybe doesn't exist !" << std::endl;
				system("pause");
				return false;
			}

			/*----Wiping out Image's data before Initialization------- ???? --*/
			width = 0;
			height = 0;
			buffer.clear();



			/*Reading_Header*/
			unsigned int counter = 0;
			unsigned int pointer = 0;
			std::string line;

			char** header = new char*[4];


			/*Counting to 4 'spaces' or '\n' */
			std::cout << std::endl;

			while (counter < 4) {

				if (infile.good()) {

					if (infile.peek() == '#') { //Consuming Header comments

						infile.get();
						while (infile.peek() != '\n') {
							infile.get();
						}
						infile.get();
					}
					else if (infile.peek() != ' ' && infile.peek() != '\n') {
						line += infile.get();

					}
					else {
						infile.get();
						header[pointer] = new char[line.length() + 1];
						strcpy_s(header[pointer], line.length() + 1, line.c_str());

#ifdef _DEBUG //debugging
						std::cout << "-[Report] : Header[" << pointer << "] : " << header[pointer] << std::endl; /*Printing Header */
#endif//

						pointer++;
						counter++;
						line = "";
					}

				}
				else {
					std::cout << std::endl;
					std::cerr << "~[Error] /load()/ : Error Occured while reading Header" << std::endl;
					system("pause");
					return false;
				}

			}
			std::cout << std::endl;


			/*Checking if File is a P6 Format*/
			try {
				if (strcmp(header[0], "P6") != 0) {

					std::cerr << "~[Error] /load()/ : File is not a <P6> Format !" << std::endl;
					system("pause");
					return false;
				}
			}
			catch (...) {

				std::cerr << "~[Error] /load()/ : Exception Occured while trying to read Header" << std::endl;
				system("pause");
				return false;
			}


			/*Casting Header[] to Integer to validate it's data*/
			int iheader[3];
			for (unsigned int i = 0; i < 3; i++) {
				iheader[i] = 0;
			}


			int power;
			int temp_v;
			for (int i = 1; i < 4; i++) {

				iheader[i - 1] = 0;
				line = header[i];
				power = line.length() - 1;

				for (unsigned int j = 0; j < line.length(); j++) {
					temp_v = (*(header[i] + j) - '0')*(int)pow(10.0, power);
					iheader[i - 1] += temp_v;
					power--;
				}

			}

			/*Checking Header's informations validity*/
			for (unsigned int i = 0; i < 3; i++) {

				switch (i) {
				case 0: if (iheader[i] <= 0) { std::cerr << "~[Error] /load()/ : Image's Width didn't get initialised or is [0]!" << std::endl; return false; } break;
				case 1: if (iheader[i] <= 0) { std::cerr << "~[Error] /load()/ : Image's Height didn't get initialised or is [0]!" << std::endl; return false; }break;
				case 2: if (iheader[i] != 255) { std::cerr << "~[Error] /load()/ : Image's Max Value for Color isn't 255!" << std::endl; return false; } break;

				}

			}

			width = iheader[0];
			height = iheader[1];

#ifdef _DEBUG//debugging
			std::cout << "-[Report] : Image's width = " << width << std::endl << "-[Report] : Image's height = " << height << std::endl;
#endif//

			/* ~Deleting header**~ */
			for (unsigned int i = 0; i < 4; i++) {
				delete[] header[i];
			}
			delete[] header;
			// ~


			/*readPPM() now can get called*/
			float * read_array = imaging::ReadPPM(filename.c_str(), (iheader), (iheader + 1));


			if (read_array == nullptr) { /*Checking if ReadPPM() returned -> nullptr */
				std::cerr << "~[Error] /load()/ : Error occured while trying to read Image data" << std::endl;
				system("pause");
				return false;
			}


#ifdef _DEBUG//debugging
			std::cout << "-[Report] : float * Array succesfully initialised !" << std::endl;
#endif//


			/*------Initialising Image::Color * buffer / from float * read_array that holds the Image data------*/


			pointer = 0;
			counter = 0;
			int size = 3 * (width)*(height);
			buffer.reserve(width*height);

			float temp_ar[3]{ -1.0f,-1.0f,-1.0f };

			for (int i = 0; i < size; i++) {

				temp_ar[counter] = read_array[i];

				if (counter < 2) {
					counter++;
				}
				else {

					buffer.push_back( Color(temp_ar[0], temp_ar[1], temp_ar[2]) ) ;
					pointer++;
					counter = 0;
				}
			}

			/* Checking Color Array bounds */
			if (pointer - 1 >= width * height) {
				std::cerr << "~[Error] /load()/ : Image::Color * buffer got out of expected bounds !" << std::endl;
				std::cout << "~[Error] /load()/ : Image::Color * buffer's expected bounds : < " << width * height << std::endl;
				std::cout << "~[Error] /load()/ : Image::Color * buffer's actual bounds :" << pointer << std::endl;
				return false;
			}

#ifdef _DEBUG//debugging
			std::cout << "-[Report] : Image::Color * buffer's size : " << pointer << std::endl;
#endif//


			/* ~Deleting float * read_array~ */
			delete[] read_array;
			// ~


			/*Final path_returning true<Everything went good>*/
			return true;

		}


		
		
		
		
		
		
		
		bool Image::save(const std::string & filename, const std::string & format) {


			if (format != "ppm") {
				std::cerr << "~[Error] /save()/ : Error , Format is not a <ppm> , cannot save file !" << std::endl;
				return false;
			}

			else if (buffer.empty()) {
				std::cerr << "~[Error] /save()/ : Error , Image hasn't been initialised succesfully / << Points to nullptr >> !" << std::endl;
				return false;
			}

			/*Casting Color * buffer to float * array */
			float * f_array = new float[width*height * 3];

			unsigned int pointer = 0;
			for (unsigned int i = 0; i < width*height; i++) {

				for (unsigned int j = pointer; j < pointer + 3; j++) {
					f_array[j] = buffer[i][j - pointer] * 255.0f;
				}
				pointer += 3;

			}


			/*Editing Filename*/
			std::string newfilename = filename;
			size_t pos = newfilename.find(".");
			newfilename = filename.substr(0, pos) + "_filtered.ppm";


			/*Calling WritePPM() once the float * array is ready */
			bool save;
			save = imaging::WritePPM(f_array, width, height, &newfilename[0]);

			/* ~Deleting f_array~ */
			delete[] f_array;
			// ~


			if (save) {

#ifdef _DEBUG//debugging
				std::cout << "-[Report] : Saving file procedure as a <ppm> format , succesfully completed ! " << std::endl;
#endif//

				std::cout << "- Image dimensions are [" << width << "]x[" << height << "]" << std::endl;
				return true;
			}

			std::cerr << "~[Error] /save()/ : Saving file procedure failed !" << std::endl << std::endl << std::endl;
			return false;

		}


		
	}; // end of Image Class

} // end of imaging namespace
#include "Array.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "Filter.h"
#include <string>
#include <math.h>
using namespace std;

/* ~~~~~~~~~~~~~~~ SHMEIWSEIS : ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  1) Na prosthesoume elegxous.
	2) Na alaksoume to onoma ths eikonas se image0X_filtered.ppm ( to ekana sto diko mou, allakse to sto diko sou )
	3) Na allaksw thn filter ( Alex ) wste na doulevei ( allagh newimage se image ).
	4) To CIN PART gia na treksei thelei olo to directory ths eikonas!!! Dhladh C:\\ExampleImages\\Image0X.ppm */


float cast_to_number(string str) { // synarthsh pou metatrepei ena argv se float arithmo.
								   // METATROPH SE STRING 

	int power;
	float fresult = 0; // deutero result, to xrhsimopoiw sthn periptwsh float 
	float result = 0;
	int elegxos = 0; // 0 an einai thetikos , 1 an einai arnhtikos
	if (str[0] == '-') elegxos = 1;
	// Elegxos an einai swsta ta dedomena
	for (int i = elegxos; i < str.size(); i++) {
		if ((str[i] <'0' || str[i] > '9') && str[i] != '.') {
			std::cout << "~[Error]: Wrong argument! \n";
			system("pause");
			exit(1);
		}
		if (str[i] == '.' && i == 0) { // an to prwto einai teleia tha einai pali lathos
			std::cout << "~[Error]: Wrong argument! \n";
			system("pause");
			exit(1);
		}
	} 
	
	// ELEGXOS FLOAT OR INT
	if (binary_search(str.begin(), str.end(), '.')) { // if true tote argv = float
		
		size_t pos = str.find('.');
		/* ypologismos akeraiou merous */
		power = pos - 1 - elegxos;
		for (unsigned int i = elegxos; i < pos; i++) { // < pos  epeidh den thelw na perilamvanei thn '.' (To pos borei na parei thn timh 0)
			result += (str[i] - '0')*pow(10, power);
			power--;
		}
	
		/* ypologismos dekadikou merous */
		power = str.length() - pos - 2;
		for (unsigned int i = pos + 1; i < str.length(); i++) {
			fresult += (str[i] - '0')*pow(10, power);
			power--;
		}
		fresult = fresult / pow(10, (str.length() - pos - 1));

		// metatroph se arnhtiko analoga an einai h oxi (eksartatai apo thn timh ths metavlhths elegxos)
		if (elegxos) {
			return (-1)*(result + fresult);
		}
		else {
			return result + fresult;
		}

	}
	else { // else , it is an int

		/* ypologismos akeraiou merous */
		power = str.length()-1-elegxos;
		for (unsigned int i = elegxos; i < str.length(); i++) { // < pos  epeidh den thelw na perilamvanei thn '.' (To pos borei na parei thn timh 0)
			result += (str[i] - '0')*pow(10, power);
			power--;
		}
		if (elegxos) {
			return (-1)*(result);
		}
		else {
			return result;
		}

	}

}




int main(int argc,char* argv[]) {


	/* ------------------ COMMAND LINE PART: ---------------------------------*/

	if (argc > 1) {
		string filename = argv[argc - 1];
		size_t pos = filename.find('.');
		string format = filename.substr(pos + 1);
		transform(format.begin(), format.end(), format.begin(), ::tolower);

		imaging::FilterLinear  FFF1;
		imaging::FilterGamma   FFF2;
		imaging::FilterBlur	   FFF3;
		imaging::FilterLaplace FFF4;

		imaging::Image object;
		object.load(filename, format);


		for (unsigned int i = 1; i < argc - 1; i++) { // prospelash olwn twn arguments mexri thn proteleutaia. ( argc-1 epeidh den theloume to last arg = Image0sth )
			
			/* -------------------- GENIKA ------------------------------------------------------------------------------------------*/
			if (string(argv[i]) == "-f") continue;

			/* -------------------- LINEAR -----------------------------------------------------------------------------*/

			else if (string(argv[i]) == "linear") {
				FFF1.setLinear(cast_to_number(string(argv[i + 1])), cast_to_number(string(argv[i + 2])),
					cast_to_number(string(argv[i + 3])), cast_to_number(string(argv[i + 4])),
					cast_to_number(string(argv[i + 5])), cast_to_number(string(argv[i + 6])));

				object = (FFF1 << object);
				i += 6;
			}
			else if (string(argv[i]) == "gamma") {
				FFF2.setGamma(cast_to_number(string(argv[i + 1])));
				object = (FFF2 << object);
				i++;
			}
			else if (string(argv[i]) == "blur") {
				FFF3.setBlur(cast_to_number(string(argv[i + 1])));
				object = (FFF3 << object);
				i++;
			}
			else if (string(argv[i]) == "laplace") {
				object = (FFF4 << object);
			}
			else {
				cout << "~[Error]: there is no filter that matches the argument[" << i << "]! \n";
				system("pause");
				return 1;
			}


		} // Telos (for) prospelashs olwn twn arguments
		
		object.save(filename, format);

	} // end if command line




	/* ----------------------------------------------- CIN PART ----------------------------------------------------------------------------------------------- */
	else {
		std::cout << "--- Please enter the filters you want to apply on the image(.ppm):  \n";
		string line;
		vector<string> seira;
		getline(std::cin,line);

		int counter = 0; //prospelavnei ta seira.at
		seira.push_back(""); // to mhdenizw epeidh o prwtos xarakthras tha einai pada -f

		// Prosthiki twn arguments kati paremferes tou argv[] pou eixame prin.
		for (unsigned int i = 0;i < line.length();i++) { 
			if (line[i] == ' ') {
				seira.push_back("");
				counter++;
			}
			else {
				seira.at(counter) += line[i]; //seira.at() safer than seira[]
			}
		}
/*
#ifdef _DEBUG // testing if seira is ok
		cout << "seira values are:\n";
		for (unsigned int i = 0;i < seira.size();i++) {
			cout << "seira[" << i << "]=" << seira[i]<<endl;
		}
		system("pause");
#endif
*/
		seira.shrink_to_fit(); // gia na einai size=capacity , borei na einai peritto alla einai pio asfales na to xrhsimopoioume.
		// SHMEIWSH: O counter twra tha exei to size tou vector thewrhtika. opote:
		string filename = seira[seira.size()-1]; 
		size_t pos = filename.find('.');
		string format = filename.substr(pos + 1);
		transform(format.begin(), format.end(), format.begin(), ::tolower);
/*
#ifdef _DEBUG // testing if seira is ok
		cout << "filename = "<< filename <<endl;
		cout << "format = " << format <<endl;
		system("pause");
#endif
*/
		imaging::FilterLinear  FFF1;
		imaging::FilterGamma   FFF2;
		imaging::FilterBlur    FFF3;
		imaging::FilterLaplace FFF4;
		imaging::Image object;
		object.load(filename, format);

		for (unsigned int i = 0; i < seira.size()-1 ; i++) { // prospelash olwn twn arguments mexri thn proteleutaia. ( argc-1 epeidh den theloume to last arg = Image0sth )

			/* -------------------- GENIKA ------------------------------------------------------------------------------------------*/
			if (seira.at(i) == "-f") continue;


			/* -------------------- LINEAR -----------------------------------------------------------------------------*/

			else if (seira[i] == "linear") {
				FFF1.setLinear(cast_to_number(seira[i + 1]), cast_to_number(seira[i + 2]),
							   cast_to_number(seira[i + 3]), cast_to_number(seira[i + 4]),
							   cast_to_number(seira[i + 5]), cast_to_number(seira[i + 6]));

				object = (FFF1 << object);
				i += 6;
			}
			else if (seira[i] == "gamma") {
				FFF2.setGamma(cast_to_number(seira[i + 1]));
				object = (FFF2 << object);
				i++;
			}
			else if (seira[i] == "blur") {
				FFF3.setBlur(cast_to_number(seira[i + 1]));
				object = (FFF3 << object);
				i++;
			}
			else if (seira[i] == "laplace") {
				object = (FFF4 << object);
			}
			else {
				cout << "~[Error]: there is no filter that matches the argument[" << i << "]! \n";
				system("pause");
				return 1;
			}


		} // Telos (for) prospelashs olwn twn arguments

		object.save(filename, format);
		
	}// end of else: CIN PART
	
	system("pause");
	return 0;
}
#pragma once
#include "Array.h"
#include "Vec3.h"
#include <math.h>

namespace imaging {

	class Filter { // Klhronomeitai pada, pernaei thn orismenh eikona apo ena filtro kai apothikevei/epistrefei to apotelesma se diaforetiko-neo adikeimeno eikonas.
	public:
		virtual Image operator<<(const Image &image) = 0;
	};

	/*---------- Linear Filter --------------------------------------------------------------*/
	class FilterLinear :public Filter { // p'(x,y)=a*p(x,y)+c
	public:
		float a[3], c[3];

		Image operator<<(const Image &image) {
			unsigned int h = image.getHeight();
			unsigned int w = image.getWidth();

			typedef math::Vec3<float> Color;
			Color alpha(a[0], a[1], a[2]);
			Color ce(c[0], c[1], c[2]);

			Image newimage(image); // Tha exoun idio size ara den yparxei periptwsh na vgei ektos oriwn.


			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {
					newimage(i, j) = ((alpha * image.getit(i, j)) + ce); // Vec3 operator '=' got us covered.

					newimage(i, j) = newimage(i, j).clampToLowerBound(0.f);
					newimage(i, j) = newimage(i, j).clampToUpperBound(1.f);


				}// end for
			}// end for

			return newimage;

		}// end of operator <<
		
		void setLinear(float a1, float a2, float a3, float c1, float c2, float c3) {
			a[0] = a1;
			a[1] = a2;
			a[2] = a3;

			c[0] = c1;
			c[1] = c2;
			c[2] = c3;

		}

		FilterLinear() {}; // default constructor.

		FilterLinear(float a1, float a2, float a3, float c1, float c2, float c3) // Linear
		{
			a[0] = a1;
			a[1] = a2;
			a[2] = a3;

			c[0] = c1;
			c[1] = c2;
			c[2] = c3;

		}

	};// end FilterLinear

	/*------------------------------ GAMMA FILTER ----------------------------------------------------------------*/
	class FilterGamma :public Filter {

		float gamma;

	public:

		Image operator<<(const Image &image) {
			unsigned int h = image.getHeight();
			unsigned int w = image.getWidth();

			Image newimage(image); // Tha exoun idio size ara den yparxei periptwsh na vgei ektos oriwn.

			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {
					for (unsigned int k = 0; k < 3; k++) {
						newimage(i, j)[k] = std::pow(image.getit(i, j)[k], gamma); // Vec3 operator '=' got us covered.
					}
					newimage(i, j) = newimage(i, j).clampToLowerBound(0.f);
					newimage(i, j) = newimage(i, j).clampToUpperBound(1.f);
				}// end for
			}// end for

			return newimage;

		}// end of operator <<

		

		FilterGamma(){}
		FilterGamma(float g) // Gamma
		{
			if (g <0.5 || g>2.0) {
				std::cout << "Error: Wrong gamma value!";
				system("pause");
				exit(1);
			}
			gamma = g;
		}

		void setGamma(float g) {
			if (g <0.5 || g>2.0) {
				std::cout << "Error: Wrong gamma value!";
				system("pause");
				exit(1);
			}
			gamma = g;
		}

	};// end FilterGamma



	/*------------------------------------ BONUS -------------------------------------------------------*/


	class FilterBlur :public Filter, public math::Array<float> {
		int N;
	public:
		typedef math::Vec3<float> Color;


		imaging::Image operator<<(const imaging::Image &image) {
			unsigned int h = image.getHeight();
			unsigned int w = image.getWidth();
			imaging::Image newimage(image);


			Color sum;
			Color black(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					sum = black; // mhdenismos athroismatos
					for (int m = (-N / 2); m < (N / 2); m++) {
						for (int n = (-N / 2); n < (N / 2); n++) {

							if (i + m < 0 || i + m>h - 1 || j + n<0 || j + n>w - 1) { continue; }

							sum += newimage(i + m, j + n)*buffer[(m + (N / 2))*N + n + (N / 2)]; // gnwstos typos sxeshs pinaka se dianysma.
																								 //std::cout << sum[0] << " , " << sum[1] << " , " << sum[2] << std::endl;
																								 //system("pause");
						} // end of sum n
					} // end of sum m.
					newimage(i, j) = sum;

					newimage(i, j) = newimage(i, j).clampToLowerBound(0.f);
					newimage(i, j) = newimage(i, j).clampToUpperBound(1.f);
					//std::cout << newimage(i, j)[0] << " , " << newimage(i, j)[1] << " , " << newimage(i, j)[2] << std::endl;
					//system("pause");
				} // end for pixel coord.
			} // end for pixel coord.

			return newimage;

		} // end of operator <<

		void setBlur(unsigned int Ni) {
			N = Ni;
			buffer.clear(); // se periptwsh pou exei kalestei o constructor FilterBlur pio prin
			buffer.reserve(N*N);
			for (int i = 0; i < N*N; i++) {
				buffer.push_back(1 / pow(N, 2));
			}
		}

		FilterBlur(){}
		FilterBlur(unsigned int Ni) 
		{
			N = Ni;
			buffer.reserve(N*N);
			for (int i = 0; i < N*N; i++) {
				buffer.push_back(1 / pow(N, 2));
			}
		}


	}; // end FilterBlur

	/*-------------------------------------------- LAPLACE ------------------------------------------------------------------------*/

	class FilterLaplace : public FilterBlur {
	
	public:
		typedef math::Vec3<float> Color;

		imaging::Image operator<<(const imaging::Image &image) {
			unsigned int h = image.getHeight();
			unsigned int w = image.getWidth();
			imaging::Image newimage(image);

			Color sum;
			Color black(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					sum = black; // mhdenismos athroismatos
					for (int m = -1; m <= 1; m++) {
						for (int n = -1; n <= 1; n++) {

							if (i + m < 0 || i + m > h-1 || j + n<0 || j + n > w-1 ) { continue; }

							sum += newimage(i + m, j + n)*buffer[(m + 1) * 3 + n + 1];			 // gnwstos typos sxeshs pinaka se dianysma.
																								 // std::cout << sum[0] << " , " << sum[1] << " , " << sum[2] << std::endl;
																								 
						} // end of sum n
					} // end of sum m.
					
					/*newimage(i, j) = sum.clampToLowerBound(0.f);
					newimage(i,j) = newimage(i, j).clampToUpperBound(1.f);*/
					/*--Enalaktika o parapanw elegxos---------------------*/
					Color white(1.f, 1.f, 1.f);
					for (unsigned int count = 0; count < 3; count++) {
						newimage(i, j)[count] = white[count] < sum[count] ? white[count] : sum[count];
					}
					for (unsigned int count = 0; count < 3; count++) {
						newimage(i, j)[count] = black[count] > newimage(i,j)[count] ?   black[count]: newimage(i, j)[count] ;
					}

					//std::cout << newimage(i, j)[0] << " , " << newimage(i, j)[1] << " , " << newimage(i, j)[2] << std::endl;
					//system("pause");
				} // end for pixel coord.
			} // end for pixel coord.
			return newimage;
		}

		FilterLaplace(){
			
			buffer.reserve(3 * 3);
			buffer.push_back(0);
			buffer.push_back(1);
			buffer.push_back(0);

			buffer.push_back(1);
			buffer.push_back(-4);
			buffer.push_back(1);

			buffer.push_back(0);
			buffer.push_back(1);
			buffer.push_back(0);

		}

	};

	

}
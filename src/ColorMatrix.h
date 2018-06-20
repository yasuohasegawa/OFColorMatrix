#pragma once
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include "ofMain.h"

#define RAD PI /180.0
#define ONETHIRD 1.0/3.0
#define RANDOM() static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
#define SHADER_MATCOL_PROP "colorMats"

// Code is ported from "http://www.quasimondo.com/colormatrix/ColorMatrix.as".

namespace OfEffect {
	class ColorMatrix
	{
	private:
		
	public:
		ColorMatrix();
		~ColorMatrix();

		void reset();
		void invert();
		void adjustSaturation(float s);
		void adjustContrast(float r, float g, float b);
		void adjustBrightness(float r, float g, float b);
		void toGreyscale(float r, float g, float b);
		void adjustHue(float degrees);
		void rotateHue(float degrees);
		void luminance2Alpha();

		void adjustAlphaContrast(float amount);

		void colorize(int rgb, float amount);

		void average(float r, float g, float b);
		void threshold(float threshold, float factor);
		void threshold_rgb(float threshold, float factor);
		void desaturate();

		void randomize(float amount);
		void thresholdAlpha(float threshold, float factor);
		void averageRGB2Alpha();
		void invertAlpha();
		void rgb2Alpha(float r, float g, float b);
		void setAlpha(float alpha);

		void concat(vector<double> mat);
		void rotateRed(float degrees);
		void rotateGreen(float degrees);
		void rotateBlue(float degrees);
		void rotateColor(float degrees, int x, int y);
		void shearRed(float green, float blue);
		void shearGreen(float red, float blue);
		void shearBlue(float red, float green);
		void shearColor(int x, int y1, float d1, int y2, float d2);

		void RGB2YUV();
		void YUV2RGB();
		void RGB2YIQ();

		void initHue();

		void transformVector(vector<double> values);
		void apply(ofShader shader);
		vector<float> getResult();


		float LUMA_R = 0.212671;
		float LUMA_G = 0.71516;
		float LUMA_B = 0.072169;


		// There seem different standards for converting RGB
		// values to Luminance. This is the one by Paul Haeberli:
		float LUMA_R2 = 0.3086;
		float LUMA_G2 = 0.6094;
		float LUMA_B2 = 0.0820;

		vector<double> matrix;

		OfEffect::ColorMatrix *preHue;
		OfEffect::ColorMatrix *postHue;
		bool hueInitialized;
	};
}
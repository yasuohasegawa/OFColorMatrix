#include "ColorMatrix.h"

// constructor
OfEffect::ColorMatrix::ColorMatrix()
{
	matrix = {
		1,0,0,0,0,
		0,1,0,0,0,
		0,0,1,0,0,
		0,0,0,1,0
	};
}

// destructor
OfEffect::ColorMatrix::~ColorMatrix()
{
	matrix.clear();
}

void OfEffect::ColorMatrix::reset() {
	matrix[0] = 1;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 0;
	matrix[4] = 0;
	matrix[5] = 0;
	matrix[6] = 1;
	matrix[7] = 0;
	matrix[8] = 0;
	matrix[9] = 0;
	matrix[10] = 0;
	matrix[11] = 0;
	matrix[12] = 1;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 0;
	matrix[16] = 0;
	matrix[17] = 0;
	matrix[18] = 1;
	matrix[19] = 0;
}

void OfEffect::ColorMatrix::invert() {
	concat({
		-1 ,  0,  0, 0, 1.0,
		0 , -1,  0, 0, 1.0,
		0 ,  0, -1, 0, 1.0,
		0,   0,  0, 1,   0
		});
}

void OfEffect::ColorMatrix::adjustSaturation(float s) {
	float sInv;
	float irlum;
	float iglum;
	float iblum;

	sInv = (1 - s);
	irlum = (sInv* LUMA_R);
	iglum = (sInv* LUMA_G);
	iblum = (sInv* LUMA_B);

	concat({(irlum + s), iglum, iblum, 0, 0,
		irlum, (iglum + s), iblum, 0, 0,
		irlum, iglum, (iblum + s), 0, 0,
		0, 0, 0, 1.0, 0});
}

void OfEffect::ColorMatrix::adjustContrast(float r, float g = NAN, float b = NAN) {
	if (isnan(g)) g = r;
	if (isnan(b)) b = r;
	r += 1.0;
	g += 1.0;
	b += 1.0;

	concat({ r, 0, 0, 0, (128.0 * (1.0 - r)) / 255.0,
		0, g, 0, 0, (128.0 * (1.0 - g)) / 255.0,
		0, 0, b, 0, (128.0 * (1.0 - b)) / 255.0,
		0, 0, 0, 1.0, 0.0 });
}

void OfEffect::ColorMatrix::adjustBrightness(float r, float g = NAN, float b = NAN) {
	if (isnan(g)) g = r;
	if (isnan(b)) b = r;
	concat({ 1.0, 0, 0, 0, r / 255.0,
		0, 1.0, 0, 0, g / 255.0,
		0, 0, 1.0, 0, b / 255.0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::toGreyscale(float r, float g, float b) {
	concat({r, g, b, 0, 0,
		r, g, b, 0, 0,
		r, g, b, 0, 0,
		0, 0, 0, 1.0, 0});
}

void OfEffect::ColorMatrix::initHue() {
	float greenRotation = 39.182655;

	if (!hueInitialized)
	{
		hueInitialized = true;
		preHue = new ColorMatrix();
		preHue->rotateRed(45.0);
		preHue->rotateGreen(-greenRotation);

		vector<double> lum = { LUMA_R2, LUMA_G2, LUMA_B2, 1.0f };

		preHue->transformVector(lum);

		float red = lum[0] / lum[2];
		float green = lum[1] / lum[2];

		preHue->shearBlue(red, green);

		postHue = new ColorMatrix();
		postHue->shearBlue(-red, -green);
		postHue->rotateGreen(greenRotation);
		postHue->rotateRed(-45.0);
	}
}

void OfEffect::ColorMatrix::adjustHue(float degrees) {
	degrees *= RAD;
	float _cos = cos(degrees);
	float _sin = sin(degrees);
	concat({((LUMA_R + (_cos * (1.0 - LUMA_R))) + (_sin * -(LUMA_R))), ((LUMA_G + (_cos * -(LUMA_G))) + (_sin * -(LUMA_G))), ((LUMA_B + (_cos * -(LUMA_B))) + (_sin * (1.0 - LUMA_B))), 0, 0,
		((LUMA_R + (_cos * -(LUMA_R))) + (_sin * 0.143)), ((LUMA_G + (_cos * (1.0 - LUMA_G))) + (_sin * 0.14)), ((LUMA_B + (_cos * -(LUMA_B))) + (_sin * -0.283)), 0, 0,
		((LUMA_R + (_cos * -(LUMA_R))) + (_sin * -((1.0 - LUMA_R)))), ((LUMA_G + (_cos * -(LUMA_G))) + (_sin * LUMA_G)), ((LUMA_B + (_cos * (1.0 - LUMA_B))) + (_sin * LUMA_B)), 0, 0,
		0, 0, 0, 1.0, 0});
}

void OfEffect::ColorMatrix::rotateHue(float degrees) {
	initHue();
	concat(preHue->matrix);
	rotateBlue(degrees);
	concat(postHue->matrix);
}

void OfEffect::ColorMatrix::luminance2Alpha() {
	concat({ 0, 0, 0, 0, 1.0,
		0, 0, 0, 0, 1.0,
		0, 0, 0, 0, 1.0,
		LUMA_R, LUMA_G, LUMA_B, 0, 0 });
}

void OfEffect::ColorMatrix::adjustAlphaContrast(float amount) {
	amount += 1;
	concat({ 1.0, 0, 0, 0, 0,
		0, 1.0, 0, 0, 0,
		0, 0, 1.0, 0, 0,
		0, 0, 0, amount, (128.0 * (1.0 - amount)) / 255.0 });
}

void OfEffect::ColorMatrix::colorize(int rgb, float amount = 1) {
	float r;
	float g;
	float b;
	float inv_amount;

	r = (((rgb >> 16) & 0xFF) / 0xFF);
	g = (((rgb >> 8) & 0xFF) / 0xFF);
	b = ((rgb & 0xFF) / 0xFF);
	inv_amount = (1.0 - amount);

	concat({ (inv_amount + ((amount * r) * LUMA_R)), ((amount * r) * LUMA_G), ((amount * r) * LUMA_B), 0, 0,
		((amount * g) * LUMA_R), (inv_amount + ((amount * g) * LUMA_G)), ((amount * g) * LUMA_B), 0, 0,
		((amount * b) * LUMA_R), ((amount * b) * LUMA_G), (inv_amount + ((amount * b) * LUMA_B)), 0, 0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::average(float r = ONETHIRD, float g = ONETHIRD, float b = ONETHIRD) {
	concat({ r, g, b, 0, 0,
		r, g, b, 0, 0,
		r, g, b, 0, 0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::threshold(float threshold, float factor = 255) {
	concat({ (LUMA_R * factor), (LUMA_G * factor), (LUMA_B * factor), 0, ((-(factor - 1.0) * threshold)/255.0),
		(LUMA_R * factor), (LUMA_G * factor), (LUMA_B * factor), 0, ((-(factor - 1.0) * threshold) / 255.0),
		(LUMA_R * factor), (LUMA_G * factor), (LUMA_B * factor), 0, ((-(factor - 1.0) * threshold) / 255.0),
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::threshold_rgb(float threshold, float factor = 255.0) {
	concat({ factor, 0, 0, 0, (-(factor - 1.0) * threshold/255.0) / 255.0,
		0, factor, 0, 0, (-(factor - 1.0) * threshold) / 255.0,
		0, 0, factor, 0, (-(factor - 1.0) * threshold) / 255.0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::desaturate() {
	concat({ LUMA_R, LUMA_G, LUMA_B, 0, 0,
		LUMA_R, LUMA_G, LUMA_B, 0, 0,
		LUMA_R, LUMA_G, LUMA_B, 0, 0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::randomize(float amount = 1.0) {
	float inv_amount = (1.0 - amount);
	float r1 = (inv_amount + (amount * (RANDOM() - RANDOM())));
	float g1 = (amount * (RANDOM() - RANDOM()));
	float b1 = (amount * (RANDOM() - RANDOM()));
	float o1 = ((amount * 0xFF) * (RANDOM() - RANDOM()));
	float r2 = (amount * (RANDOM()) - RANDOM());
	float g2 = (inv_amount + (amount * (RANDOM() - RANDOM())));
	float b2 = (amount * (RANDOM() - RANDOM()));
	float o2 = ((amount * 0xFF) * (RANDOM() - RANDOM()));
	float r3 = (amount * (RANDOM() - RANDOM()));
	float g3 = (amount * (RANDOM() - RANDOM()));
	float b3 = (inv_amount + (amount * (RANDOM() - RANDOM())));
	float o3 = ((amount * 0xFF) * (RANDOM()) - RANDOM());

	concat({ r1, g1, b1, 0, o1 / 255.0,
		r2, g2, b2, 0, o2 / 255.0,
		r3, g3, b3, 0, o3/255.0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::thresholdAlpha(float threshold, float factor = 255.0) {
	concat({ 1.0, 0, 0, 0, 0,
		0, 1.0, 0, 0, 0,
		0, 0, 1.0, 0, 0,
		0, 0, 0, factor, (-factor * threshold) / 255.0 });
}

void OfEffect::ColorMatrix::averageRGB2Alpha() {
	concat({ 0, 0, 0, 0, 1.0,
		0, 0, 0, 0, 1.0,
		0, 0, 0, 0, 1.0,
		ONETHIRD, ONETHIRD, ONETHIRD, 0, 0 });
}

void OfEffect::ColorMatrix::invertAlpha() {
	concat({ 1.0, 0, 0, 0, 0,
		0, 1.0, 0, 0, 0,
		0, 0, 1.0, 0, 0,
		0, 0, 0, -1.0, 1.0 });
}

void OfEffect::ColorMatrix::rgb2Alpha(float r, float g, float b) {
	concat({ 0, 0, 0, 0, 1.0,
		0, 0, 0, 0, 1.0,
		0, 0, 0, 0, 1.0,
		r, g, b, 0, 0 });
}

void OfEffect::ColorMatrix::setAlpha(float alpha) {
	concat({ 1.0, 0, 0, 0, 0,
		0, 1.0, 0, 0, 0,
		0, 0, 1.0, 0, 0,
		0, 0, 0, alpha, 0 });
}

void OfEffect::ColorMatrix::concat(vector<double> mat) {
	vector<double> temp(20);
	int i = 0;
	int x, y;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 5; x++)
		{
			temp[(i + x)] = mat[i] * matrix[x] +
				mat[(i + 1)] * matrix[(x + 5)] +
				mat[(i + 2)] * matrix[(x + 10)] +
				mat[(i + 3)] * matrix[(x + 15)] +
				(x == 4 ? mat[(i + 4)] : 0);
		}
		i += 5;
	}

	matrix = temp;
}

void OfEffect::ColorMatrix::rotateRed(float degrees) {
	rotateColor(degrees, 2, 1);
}

void OfEffect::ColorMatrix::rotateGreen(float degrees) {
	rotateColor(degrees, 0, 2);
}

void OfEffect::ColorMatrix::rotateBlue(float degrees) {
	rotateColor(degrees, 1, 0);
}

void OfEffect::ColorMatrix::rotateColor(float degrees, int x, int y) {
	degrees *= RAD;
	vector<double> mat = {
			1, 0, 0, 0, 0,
			0, 1, 0, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, 1, 0};
	mat[x + x * 5] = mat[y + y * 5] = cos(degrees);
	mat[y + x * 5] = sin(degrees);
	mat[x + y * 5] = -sin(degrees);
	concat(mat);
}

void OfEffect::ColorMatrix::shearRed(float green, float blue) {
	shearColor(0, 1, green, 2, blue);
}

void OfEffect::ColorMatrix::shearGreen(float red, float blue) {
	shearColor(1, 0, red, 2, blue);
}

void OfEffect::ColorMatrix::shearBlue(float red, float green) {
	shearColor(2, 0, red, 1, green);
}

void OfEffect::ColorMatrix::shearColor(int x, int y1, float d1, int y2, float d2) {
	vector<double> mat = {
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0 };
	mat[y1 + x * 5] = d1;
	mat[y2 + x * 5] = d2;
	concat(mat);
}

void OfEffect::ColorMatrix::RGB2YUV() {
	concat({ 0.29900, 0.58700, 0.11400, 0, 0,
		-0.16874, -0.33126, 0.50000, 0, 128.0 /255.0,
		0.50000, -0.41869, -0.08131, 0, 128.0 / 255.0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::YUV2RGB() {
	concat({ 1.0, -0.000007154783816076815, 1.4019975662231445, 0, -179.45477266423404 / 255.0,
		1.0, -0.3441331386566162, -0.7141380310058594, 0, 135.45870971679688 / 255.0,
		1.0, 1.7720025777816772, 0.00001542569043522235, 0, -226.8183044444304 / 255.0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::RGB2YIQ() {
	concat({ 0.2990, 0.5870, 0.1140, 0, 0,
		0.595716, -0.274453, -0.321263, 0, 128.0 / 255.0,
		0.211456, -0.522591, -0.311135, 0, 128.0 / 255.0,
		0, 0, 0, 1.0, 0 });
}

void OfEffect::ColorMatrix::transformVector(vector<double> values) {
	if (values.size() != 4) return;

	float r = values[0] * matrix[0] + values[1] * matrix[1] + values[2] * matrix[2] + values[3] * matrix[3] + matrix[4];
	float g = values[0] * matrix[5] + values[1] * matrix[6] + values[2] * matrix[7] + values[3] * matrix[8] + matrix[9];
	float b = values[0] * matrix[10] + values[1] * matrix[11] + values[2] * matrix[12] + values[3] * matrix[13] + matrix[14];
	float a = values[0] * matrix[15] + values[1] * matrix[16] + values[2] * matrix[17] + values[3] * matrix[18] + matrix[19];

	values[0] = r;
	values[1] = g;
	values[2] = b;
	values[3] = a;
}

void OfEffect::ColorMatrix::apply(ofShader shader) {
	shader.setUniform1fv(SHADER_MATCOL_PROP, &getResult()[0], matrix.size());
}

vector<float> OfEffect::ColorMatrix::getResult() {
	std::vector<float> mat_float(matrix.begin(), matrix.end());
	return mat_float;
}
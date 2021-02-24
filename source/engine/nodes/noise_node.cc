#include <noise_node.h>


noise_node::noise_node()
{
	load_default_params("noise");

	int seed = 0;
	p.resize(256);

	// Llene p con valores de 0 a 255
	std::iota(p.begin(), p.end(), 0);

	// Inicializar un motor aleatorio con semilla
	std::default_random_engine engine(seed);

	// Mezclar usando el motor aleatorio anterior
	std::shuffle(p.begin(), p.end(), engine);

	// Duplica el vector de permutación
	p.insert(p.end(), p.begin(), p.end());
}

noise_node::~noise_node() { }

double noise_node::noise(double x, double y, double z) 
{
	// Encuentra el cubo unitario que contiene el punto
	int X = (int) floor(x) & 255;
	int Y = (int) floor(y) & 255;
	int Z = (int) floor(z) & 255;

	// Encuentra el relativo x, y, z del punto en el cubo
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Encuentra el relativo x, y, z del punto en el cubo
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Coordenadas hash de las 8 esquinas del cubo
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Agrega resultados combinados de 8 esquinas del cubo
	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)), lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)), lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));
	return (res + 1.0)/2.0;
}

double noise_node::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double noise_node::lerp(double t, double a, double b) {
	return a + t * (b - a); 
}

double noise_node::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	// Convierta los 4 bits inferiores de hash en 12 direcciones de gradiente
	double u = h < 8 ? x : y,
		   v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

void noise_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	float size = get(params, "size").toDouble();
	float zoffset = get(params, "zoffset").toDouble();
	float gain = get(params, "gain").toDouble();
	float gamma = get(params, "gamma").toDouble();

	cv::Mat3f noise_image(720, 1280);

    for( int y = 0; y < noise_image.rows; y++ ) {
        for( int x = 0; x < noise_image.cols; x++ ) {
			cv::Vec3f &pixel = noise_image.at<cv::Vec3f>(y, x);

			float nx = float(x) / size;
			float ny = float(y) / size;

			double n = noise(nx, ny, zoffset);

			float value = floor(255 * n);
			pixel = {value, value, value};
		}
	}

	(*image) = noise_image;
	image->convertTo(*image, CV_32F);
}

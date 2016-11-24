#ifndef M_ROBOT
#define M_ROBOT

using namespace std;

class Robot
{
public:
	Robot(){}
	~Robot(){}
public:
	void img2robot(int x_im, int y_im, int& X, int& Y);
private:
	// X = a1 + a2 * x_im + a3 * y_im
	float a1 = -479.945160192;
	float a2 = 1.18393057953;
	float a3 = 0.00608376171349;
	// Y = b1 + b2 * x_im + b3 * y_im
	float b1 = 784.002566698;
	float b2 = 0.0859078821404;
	float b3 = -1.14523556393;
};

#endif

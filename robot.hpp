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
	float a1 = -477;
	float a2 = 1.15;
	float a3 = 0.05;
	// Y = b1 + b2 * x_im + b3 * y_im
	float b1 = 683.0;
	float b2 = 0.048;
	float b3 = -1.115;
};

#endif

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
	//Second calibration
	// X = a1 + a2 * x_im + a3 * y_im
	float a1 = -461.863;
	float a2 = 1.10843;
	float a3 = 0.08355;
	// Y = b1 + b2 * x_im + b3 * y_im
	float b1 = 680.783;
	float b2 = 0.0468;
	float b3 = -1.0868;
};

#endif

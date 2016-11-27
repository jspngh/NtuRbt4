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
	// X = x_im * a - b
	float a = 1.128;
	float b = 456.8;
	// Y = c - (y_im * a - c)
	float c = 352.2;
};

#endif

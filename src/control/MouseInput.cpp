#include <eeros/control/MouseInput.hpp>

using namespace eeros::control;

MouseInput::MouseInput(std::string dev) : mouse(dev) {
	setInitPos(0, 0, 0, 0);
	first = true;
}

MouseInput::~MouseInput() { }

void MouseInput::run() {
	if (first) {
		x = axisScale_x * - mouse.current.axis.y;
		y = axisScale_y * - mouse.current.axis.x;
		z = axisScale_z * - mouse.current.axis.z;
		r = axisScale_r * - mouse.current.axis.r;
		first = false;
	}

	double vx = axisScale_x * mouse.current.axis.y;
	if ((x + vx) < min_x)
		x = (min_x - vx);
	else if ((x + vx) > max_x)
		x = (max_x - vx);
	vx += x;

	double vy = axisScale_y * mouse.current.axis.x;
	if ((y + vy) < min_y)
		y = (min_y - vy);
	else if ((y + vy) > max_y)
		y = (max_y - vy);
	vy += y;

	double vz = axisScale_z * mouse.current.axis.z;
	if ((z + vz) < min_z)
		z = (min_z - vz);
	else if ((z + vz) > max_z)
		z = (max_z - vz);
	vz += z;
	
	double vr = axisScale_r * mouse.current.axis.r;
	if ((r + vr) < min_r)
		r = (min_r - vr);
	else if ((r + vr) > max_r)
		r = (max_r - vr);
	vr += r;

	uint64_t time = eeros::System::getTimeNs();
	out.getSignal().setValue(Vector4{ vx, vy, vz, vr });
	out.getSignal().setTimestamp(time);
	
	buttonOut.getSignal().setValue(Matrix<3,1,bool>{mouse.current.button.left, mouse.current.button.middle, mouse.current.button.right});
	buttonOut.getSignal().setTimestamp(time);
}

Output<Matrix<3,1,bool>>& MouseInput::getButtonOut() {
	return buttonOut;
}

void MouseInput::setInitPos(double x, double y, double z, double r) {
	reset(x, y, z, r);
	out.getSignal().setValue(Matrix<4>{ x, y, z, r });
}

void MouseInput::setInitPos(Matrix<4> pos) {
	setInitPos(pos[0], pos[1], pos[2], pos[3]);
}

void MouseInput::reset(double x, double y, double z, double r) {
        this->x = (x - axisScale_x * mouse.current.axis.y);
        this->y = (y - axisScale_y * mouse.current.axis.x);
        this->z = (z - axisScale_z * mouse.current.axis.z);
        this->r = (r - axisScale_r * mouse.current.axis.r);
}

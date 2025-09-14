#include "main.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char **argv)
{
    // if(argc < 2) {
    //     cout << "Usage [obj file]" << endl;
    //     return 0;
    // }

    frame = 0;
	seconds = time(NULL);
	fpstracker = 0;

	if (samplingTest_Init()) {
	    samplingTest_Loop();
	}

	return 0;
}

void samplingTest_Loop()
{
    while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		time_t seconds2 = time(NULL);

		frame++;
		fpstracker++;

		if (seconds2 - seconds >= 1) {

			fps = fpstracker / (seconds2 - seconds);
			fpstracker = 0;
			seconds = seconds2;
		}

		string title = "Particle Simulation | " + utilityCore::convertIntToString((int)fps) + " FPS";
		glfwSetWindowTitle(window, title.c_str());
		updateCamera();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
		}
	glfwDestroyWindow(window);
	glfwTerminate();

}


bool samplingTest_Init()
{
    glfwSetErrorCallback(errorCallback);
    if(!glfwInit()){
        cout << "GLFW not initialized" << endl;
        return false;
    }

    width = 800;
    height = 800;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Particle Simulation", NULL, NULL);

    if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return false;
	}

    glfwMakeContextCurrent(window);		//Initialize GLEW
	glfwSetKeyCallback(window, keyCallback);

	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseMotionCallback);
	glfwSetScrollCallback(window, mouseWheelCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    return true;
}
//------------------------------
//-------GLFW CALLBACKS---------
//------------------------------

void errorCallback(int error, const char *description) {
    fputs(description, stderr);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		}
	}
}
enum ControlState { NONE = 0, ROTATE, TRANSLATE };
ControlState mouseState = NONE;
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			mouseState = ROTATE;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			mouseState = TRANSLATE;
		}

	}
	else if (action == GLFW_RELEASE)
	{
		mouseState = NONE;
	}
	//printf("%d\n", mouseState);
}

double lastx = (double)width / 2;
double lasty = (double)height / 2;
void mouseMotionCallback(GLFWwindow* window, double xpos, double ypos)
{
	const double s_r = 0.01;
	const double s_t = 0.01;

	double diffx = xpos - lastx;
	double diffy = ypos - lasty;
	lastx = xpos;
	lasty = ypos;

	if (mouseState == ROTATE)
	{
		//rotate
		x_angle += (float)s_r * diffy;
		y_angle += (float)s_r * diffx;
	}
	else if (mouseState == TRANSLATE)
	{
		//translate
		x_trans += (float)(s_t * diffx);
		y_trans += (float)(-s_t * diffy);
	}
}

void mouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	const double s_s = 0.1;

	z_trans += -s_s * yoffset;


	//cout << cameraPosition.x << "," << cameraPosition.y << "," << cameraPosition.z << endl;
	//cout << cameraPosition.length()<<endl;
}

void updateCamera()
{
	//tmp
	glm::mat4 R =  glm::rotate(ones_matrix, x_angle, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(ones_matrix, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 T = glm::translate(ones_matrix, glm::vec3(x_trans, y_trans, z_trans));

	glm::vec4 tmp = R * T * glm::vec4(0.0, 0.0, 0.0, 1.0);
	tmp /= tmp.w;
	cameraPosition = glm::vec3(tmp);

	view = glm::translate(ones_matrix, glm::vec3(-x_trans, -y_trans, -z_trans)) * glm::transpose(R);

	projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

}

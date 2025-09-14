#include "main.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GRID_LENGTH_DEFAULT (0.273438f)

//#define OBJ_ARR_SIZE 9
#define OBJ_ARR_SIZE 3

#define GRAVITY (glm::vec3(0.0f,-9.8f,0.0f))
#define FPS 60.0f
#define FRAME_TIME 1.0f/FPS
#define SIMU_STEP 10
#define DELTA_T FRAME_TIME/SIMU_STEP

//-------------------------------
//-------------MAIN--------------
//-------------------------------

RigidBody rigid_body[OBJ_ARR_SIZE];
float uniform_grid_length;
GLfloat *v_buffer_ptr;
int buffer_size;
int main(int argc, char **argv)
{
    if(argc < 2) {
        cout << "Usage [obj file]" << endl;
        return 0;
    }

    frame = 0;
	seconds = time(NULL);
	fpstracker = 0;

	if (samplingTest_Init()) {

    	// Rigid body Suzanne
    	rigid_body[0].setPhase(0);
    	rigid_body[0].setTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
    	rigid_body[0].setRotation(glm::rotate(ones_matrix, 5.0f*(float)PI/180.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    	// rigid_body[0].setInitVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    	rigid_body[0].setInitVelocity(glm::vec3(0.0f, 9.8f, 0.0f));
    	rigid_body[0].setMassScale(1.0f);
    	rigid_body[0].setType(SOLID);
    	rigid_body[0].setColor(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
    	rigid_body[0].initObj(argv[1]);
    	rigid_body[0].initParticles(10);

	 //    int c = 0;
		// uniform_grid_length = GRID_LENGTH_DEFAULT;
		// // water fill
		// c = 0;
		// rigid_body[c].setPhase(c);

		// rigid_body[c].setTranslate(glm::vec3(-4.0f, 8.0f, 0.0f));
		// //rigid_body[c].setTranslate(glm::vec3(1.0f, 5.0f, 0.0f));
		// //rigid_body[c].setRotation(glm::rotate(5.0f*(float)PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
		// rigid_body[c].setInitVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		// //rigid_body[c].setInitVelocity(glm::vec3(0.0f, 9.8f, 0.0f));
		// rigid_body[c].setMassScale(1.0f);
		// rigid_body[c].setType(SOLID);
		// rigid_body[c].setColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		// rigid_body[c].initObj(argv[1]);
		// rigid_body[c].initParticles(uniform_grid_length);

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

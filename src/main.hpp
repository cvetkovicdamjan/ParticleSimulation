#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "RigidBody.h"
#include <util/utilityCore.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

//-------------------------------
//------------GL STUFF-----------
//-------------------------------
int frame;
int fpstracker;
double seconds;
int fps = 0;

GLFWwindow *window;

//-------------------------------
//----------CUDA STUFF-----------
//-------------------------------

int width = 800;
int height = 800;

//-------------------------------
//-------------MAIN--------------
//-------------------------------


int main(int argc, char **argv);


//------------------------------
//-------GLFW CALLBACKS---------
//------------------------------
void mainLoop();
void errorCallback(int error, const char *description);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

//Mouse Control
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseMotionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);


void updateCamera();
glm::mat4 ones_matrix = glm::mat4(1.0f);
float scale = 1.0f;
float x_trans = 0.0f, y_trans = 0.0f, z_trans = 20.0f;
float x_angle = 0.0f, y_angle = 0.0f;



glm::mat4 model = glm::scale(ones_matrix, glm::vec3(scale, scale, scale));
glm::mat4 view;

glm::mat4 projection;
glm::vec3 cameraPosition(x_trans, y_trans, z_trans);

bool samplingTest_Init();
void samplingTest_Loop();

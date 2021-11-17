#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "include/common/math_utils.h"
#include "include/common/camera.h"
#include "include/common/cube.h"
#include "include/common/boundingbox.h"
#include "include/common/lightsource.h"
#include "include/common/volumerender.h"
#include "include/common/arcball.h"
#include "include/common/ui.h"

using namespace std;

/********************************************************************/
/*   Variables */

ui::UI* gui;
ArcBall *arcball;

char *theProgramTitle = "Volume rendering";
int theWindowWidth = 700, theWindowHeight = 700;
int theWindowPositionX = 40, theWindowPositionY = 40;
bool isFullScreen = false;
bool isAnimating = true;
float rotation = 0.0f;
float cubeSize = 0.15f;
float scaleBy = 0.05f;

int numVerts;
int numFaces;
int vertsPerFace;
float x1, x2, y_1, y2, z1, z2;
GLuint cWorldLoc;
Vector3f meshCenter;
float FOV = 90.0f, zNear = 1.0f, zFar = 100.0f;
PersProjInfo persProjInfo = {FOV, theWindowWidth, theWindowHeight, zNear, zFar};
GLuint ShaderProgram, texProgram;
Cube *boundingBox = 0;
LightSource *lightsrc = 0;
VolumeRender *volumeRender = 0;

// camera
Camera camera(theWindowWidth, theWindowHeight);

/* Constants */
const int ANIMATION_DELAY = 20; /* milliseconds between rendering */
char *rawFile = "../../data/tooth_103x94x161_uint8.raw";

/********************************************************************
  Utility functions
 */

/* post: compute frames per second and display in window's title bar */
void computeFPS()
{
	static int frameCount = 0;
	static int lastFrameTime = 0;
	static char *title = NULL;
	int currentTime;

	if (!title)
		title = (char *)malloc((strlen(theProgramTitle) + 20) * sizeof(char));
	frameCount++;
	currentTime = glutGet((GLenum)(GLUT_ELAPSED_TIME));
	if (currentTime - lastFrameTime > 1000)
	{
		sprintf(title, "%s [ FPS: %4.2f ]",
				theProgramTitle,
				frameCount * 1000.0 / (currentTime - lastFrameTime));
		glutSetWindowTitle(title);
		lastFrameTime = currentTime;
		frameCount = 0;
	}
}

/********************************************************************
 Callback Functions
 These functions are registered with the glut window and called 
 when certain events occur.
 */

void onInit(int argc, char *argv[])
/* pre:  glut window has been initialized
   post: model has been initialized */
{
	/* by default the back ground color is black */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	volumeRender = new VolumeRender(rawFile);
	boundingBox = new BoundingBox();
	lightsrc = new LightSource();
	lightsrc->setPosition(Vector3f(-0.3f, 1.5f, 0.0f));
	volumeRender->setLightSrc(lightsrc);
	volumeRender->setCameraPos(camera.getPos());

	glEnable(GL_DEPTH_TEST);

    // cull back faces
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

	gui = new ui::UI();
	gui->setVolumeRender(volumeRender);
	arcball = new ArcBall(theWindowWidth, theWindowHeight, 5.0f);
}

static void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gui->newFrame();

	camera.onRender();
	Matrix4f Proj;
	Proj.InitPersProjTransform(persProjInfo);
	Matrix4f MVP;
	MVP = Proj * camera.getMatrix() * arcball->getRotationMatrix();

	volumeRender->render(MVP, arcball->getRotationMatrix());
	boundingBox->render(MVP);

	// move light source along y-axis
	// lightsrc->setPosition(Vector3f(lightsrc->getPosition().x,
	// 								lightsrc->getPosition().y - 0.05 * sin(25*rotation),
	// 								lightsrc->getPosition().z));


	lightsrc->render(Proj * camera.getMatrix());

	gui->widget();
	gui->render();

	// glDisableVertexAttribArray(0);

	/* check for any errors when rendering */
	GLenum errorCode = glGetError();
	if (errorCode == GL_NO_ERROR)
	{
		/* double-buffering - swap the back and front buffers */
		glutSwapBuffers();
	}
	else
	{
		fprintf(stderr, "OpenGL rendering error %d\n", errorCode);
	}
}

/* pre:  glut window has been resized
 */
static void onReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	if (!isFullScreen)
	{
		theWindowWidth = width;
		theWindowHeight = height;
	}
	// update scene based on new aspect ratio....
}

/* pre:  glut window is not doing anything else
   post: scene is updated and re-rendered if necessary */
static void onIdle()
{
	static int oldTime = 0;
	if (isAnimating)
	{
		int currentTime = glutGet((GLenum)(GLUT_ELAPSED_TIME));
		/* Ensures fairly constant framerate */
		if (currentTime - oldTime > ANIMATION_DELAY)
		{
			// do animation....
			rotation += 0.001;

			oldTime = currentTime;
			/* compute the frame rate */
			computeFPS();
			/* notify window it has to be repainted */
			glutPostRedisplay();
		}
	}
}

/* pre:  mouse is dragged (i.e., moved while button is pressed) within glut window
   post: scene is updated and re-rendered  */
static void onMouseMotion(int x, int y)
{
	/* notify window that it has to be re-rendered */
	// camera.handleMouse(x, y);
	ImGui_ImplGLUT_MotionFunc(x, y);
	arcball->handleCursor(x, y);
	glutPostRedisplay();
}

/* pre:  mouse button has been pressed while within glut window
   post: scene is updated and re-rendered */
static void onMouseButtonPress(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Left button pressed
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		// Left button un pressed
	}

	ImGui_ImplGLUT_MouseFunc(button, state, x, y);
	arcball->handleMouseButton(button, state, x, y);

	/* notify window that it has to be re-rendered */
	glutPostRedisplay();
}

/* pre:  key has been pressed
   post: scene is updated and re-rendered */

static void onAlphaNumericKeyPress(unsigned char key, int x, int y)
{
	camera.handleKeyboard(key);
	volumeRender->handleKeyboard(key);

	switch (key)
	{
		/* toggle animation running */
	case 'z':
		isAnimating = !isAnimating;
		break;
		/* reset */
	case 'r':
		rotation = 0;
		break;
		/* quit! */
	case 27:
		exit(0);
	}
	ImGui_ImplGLUT_KeyboardFunc(key, x, y);
	/* notify window that it has to be re-rendered */
	glutPostRedisplay();
}

/* pre:  arrow or function key has been pressed
   post: scene is updated and re-rendered */
static void onSpecialKeyPress(int key, int x, int y)
{
	camera.handleKeyboard(key);
	volumeRender->handleKeyboard(key);

	/* please do not change function of these keys */
	switch (key)
	{
		/* toggle full screen mode */
	case GLUT_KEY_F1:
		isFullScreen = !isFullScreen;
		if (isFullScreen)
		{
			theWindowPositionX = glutGet((GLenum)(GLUT_WINDOW_X));
			theWindowPositionY = glutGet((GLenum)(GLUT_WINDOW_Y));
			glutFullScreen();
			glViewport(0, 0, 1920, 1080);
			arcball->updateWindowSize(1920, 1080);
		}
		else
		{
			glViewport(0, 0, theWindowWidth, theWindowHeight);
			arcball->updateWindowSize(theWindowWidth, theWindowHeight);
			glutPositionWindow(theWindowPositionX, theWindowPositionY);
		}
		break;
	}
	ImGui_ImplGLUT_SpecialFunc(key, x, y);
	/* notify window that it has to be re-rendered */
	glutPostRedisplay();
}

/* pre:  glut window has just been iconified or restored 
   post: if window is visible, animate model, otherwise don't bother */
static void onVisible(int state)
{
	if (state == GLUT_VISIBLE)
	{
		/* tell glut to show model again */
		glutIdleFunc(onIdle);
	}
	else
	{
		glutIdleFunc(NULL);
	}
}

// handles mouse movemnt without pressing any button
static void PassiveMouseCB(int x, int y)
{
	// camera.handleMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	/* tell glut how to display model */
	glutDisplayFunc(onDisplay);
	/* tell glut how to respond to changes in window size */
	glutReshapeFunc(onReshape);
	/* tell glutwhat to do when it would otherwise be idle */
	glutIdleFunc(onIdle);
	/* tell glut how to handle changes in window visibility */
	glutVisibilityFunc(onVisible);
}

static void OverrideImguiCallbacks() {
	/* tell glut how to handle the mouse */
	glutMotionFunc(onMouseMotion);
	glutMouseFunc(onMouseButtonPress);
		/* tell glut how to handle key presses */
	glutKeyboardFunc(onAlphaNumericKeyPress);
	glutSpecialFunc(onSpecialKeyPress);
	glutPassiveMotionFunc(PassiveMouseCB);
}

int main(int argc, char **argv)
{

	// command line arguments
	if (argc >= 2)
	{
		rawFile = argv[1];
		if (argv[2])
			scaleBy = atof(argv[2]);
	}

	glutInit(&argc, argv);

	/* request initial window size and position on the screen */
	glutInitWindowSize(theWindowWidth, theWindowHeight);
	glutInitWindowPosition(theWindowPositionX, theWindowPositionY);
	/* request full color with double buffering and depth-based rendering */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	/* create window whose title is the name of the executable */
	glutCreateWindow(theProgramTitle);

	InitializeGlutCallbacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	/* initialize model */
	onInit(argc, argv);

	OverrideImguiCallbacks();

	/* give control over to glut to handle rendering and interaction  */
	glutMainLoop();

	/* program should never get here */

	return 0;
}

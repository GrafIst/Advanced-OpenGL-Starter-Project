#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <fstream>

#include <string>
#include<cstdlib>
#include <math.h>   

using namespace std;
//#define GLEW_STATIC

string LoadShader(string fileName);

float GenerateSeed(float seed);

float getAngleFromCoordinate(float x, float y);


struct Vector2 {
	float x;
	float y;
};

Vector2 getCoordinateFromAngle(float angle, float distance);


int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!\n";
	}
	///////////SETTING UP SDL/////////////
	//Create a simple window
	int width = 800;
	int height = 800;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);
	//SDL_WINDOW_OPENGL is a u32 flag !


	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}


	//Set the viewing frame through which we will see the objects
	glViewport(0, 0, width, height);

	//Put the color you want here for the background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	float size = 0.15f;
	//float size2 = 0.17f;

	float tempangle = getAngleFromCoordinate(size, size);

	cout << tempangle * (2 * M_PI) << endl;

	tempangle += 90.f;

	cout << getCoordinateFromAngle(tempangle, size).x << endl;
	cout << getCoordinateFromAngle(tempangle, size).y << endl;


	float vertices[] = {
		// positions             // colors
			 size, -size, 0.0f,  1.0, 0.0f, 0.0f,
			-size, -size, 0.0f,  0.0f, 1.0, 0.0f,
			 size,  size, 0.0f,  1.0f, 1.0f, 1.0f,
			 -size,  size, 0.0f, 1.0f, 1.0f, 1.0f


			 /*size2, -size2, 0.0f,  1.0, 0.0f, 0.0f,
			-size2, -size2, 0.0f,  0.0f, 1.0, 0.0f,
			 size2,  size2, 0.0f,  1.0f, 1.0f, 1.0f,
			 -size2,  size2, 0.0f, 1.0f, 1.0f, 1.0f,*/
	};


	//Create an ID to be given at object generation
	unsigned int vbo;

	//Pass how many buffers should be created and the reference of the ID to get the value set
	glGenBuffers(1, &vbo);



	string vs = LoadShader("vertex.shader");
	const char* vertexShaderSource = vs.c_str();
	string fs = LoadShader("fragment.shader");
	const char* fragmentShaderSource = fs.c_str();


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);


	//now that we have a vertex shader, let’s put the code text inside
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);


	//aaaaand… Compile !
	glCompileShader(vertexShader);


	//Do the same with the fragment shader !
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();



	//now attach shaders to use to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//and link it 
	glLinkProgram(shaderProgram);

	//now that the program is complete, we can use it 
	glUseProgram(shaderProgram);



	//Create one ID to be given at object generation
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Finally send the vertices array in the array buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);




	//Use depth management
	glEnable(GL_DEPTH_TEST);

	//0 is our origin, the higher the z, the farther the object
	glDepthFunc(GL_LESS);

	int vertexColorLocation = glGetUniformLocation(shaderProgram, "dir");
	//cout << vertexColorLocation << endl;
	glUniform1f(vertexColorLocation, 1.f);

	bool isRunning = true;


	float offsetX = 0;
	float dirX = 1;

	float offsetY = 0;
	float dirY = 1;

	float tempOffset = 0;


	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;


	float speed = 0.0001;

	float r = 1.f;
	float g = 0;
	float b = 0;

	while (isRunning) {
		// Inputs
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		//Shader to use next
		glUseProgram(shaderProgram);

		//VAO to use next
		glBindVertexArray(vao);

		// Get the time in seconds 
		float timeValue = (float)SDL_GetTicks() / 1000;

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());


		if (offsetX + size >1.f) {
			dirX = -1.f;
			r = GenerateSeed(2.5);
			g = GenerateSeed(7.5);
			b = GenerateSeed(1.5);
		}
		else if(offsetX- size <-1.f) {
			dirX = 1.f;
			r = GenerateSeed(20.5);
			g = GenerateSeed(70.5);
			b = GenerateSeed(10.5);
		}

		if (offsetY + size > 1.f) {
			dirY = -1.f;
			r = GenerateSeed(200.5);
			g = GenerateSeed(700.5);
			b = GenerateSeed(100.5);
		}
		else if (offsetY - size < -1.f) {
			dirY = 1.f;
			r = GenerateSeed(2000.5);
			g = GenerateSeed(7000.5);
			b = GenerateSeed(1000.5);
		}

		//speed = 0.001;

		//calculate a rotation based on the current value




		offsetX += (dirX* deltaTime * speed*0.7f);
		offsetY += (dirY * deltaTime * speed);


		//cout << offsetX << endl;

		//tempOffset += timeValue;
		
		float offset = (sin(timeValue) / 3.0f);
		float offset2 = (cos(timeValue) / 0.2f);


		speed += (offset*offset2)*0.00005;

		//cout << speed << endl;

		speed = speed < 0.0002 ? 0.0002f : speed;

		

		int vertexColorLocationA = glGetUniformLocation(shaderProgram, "offsetX");
		
		glUniform1f(vertexColorLocationA, offsetX);

		int vertexColorLocationB = glGetUniformLocation(shaderProgram, "offsetY");

		glUniform1f(vertexColorLocationB, offsetY);

		int ColorRLoc = glGetUniformLocation(shaderProgram, "r");
		int ColorGLoc = glGetUniformLocation(shaderProgram, "g");
		int ColorBLoc = glGetUniformLocation(shaderProgram, "b");

		glUniform1f(ColorRLoc, r);
		glUniform1f(ColorGLoc, g);
		glUniform1f(ColorBLoc, b);


		int timeLocation = glGetUniformLocation(shaderProgram, "time");
		glUniform1f(timeLocation, timeValue);

		int sizeLoc = glGetUniformLocation(shaderProgram, "size");
		glUniform1f(timeLocation, size);

		//OMG WE FINALLY DRAW ! We use the GL_TRIANGLES primitive type
		//We draw from vertex 0 and we will be drawing 3 vertices
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//glDrawArrays(GL_TRIANGLE_STRIP, 4, 3);

		SDL_GL_SwapWindow(Window); // Swapbuffer
	}
	// Quit
	SDL_DestroyWindow(Window);
	SDL_GL_DeleteContext(Context);


	//cin.get();
	return 0;
}


string LoadShader(string fileName) {
	ifstream myFile;
	myFile.open(fileName);
	if (myFile.fail()) {
		cerr << "Error - failed to open " << fileName << endl;
	}
	string fileText = "";
	string line = "";
	while (getline(myFile, line)) {
		fileText += line + '\n';
	}

	myFile.close();
	return fileText;
}

float GenerateSeed(float seed) {
	// Providing a seed value
	srand((unsigned)time(NULL)*1000*seed);

	// Get a random number
	float random = (rand()%100)/100.f;

	// Print the random number
	//cout << random << endl;
	return random;
}

float getAngleFromCoordinate(float x, float y) {
	cout << x << endl;
	return atan(y / x);
	
}

Vector2 getCoordinateFromAngle(float angle, float distance) {
	angle *= M_PI / 180;
	return { distance * cos(angle),distance * sin(angle) };
}


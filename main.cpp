/*
 * main.cpp
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "Particle.h"
#include "ParticleManager.h"
#include "GLFW/glfw3.h"


static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "Key: " << (int)(key == GLFW_KEY_ESCAPE) << "\n";
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

float rrang(float _min, float _max) {
	return (double)rand() * (_max - _min) / RAND_MAX + _min;
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

	ParticleManager pManager = ParticleManager();
	for(unsigned int i=0; i<1000; i++) {
		pManager.addParticle(new Particle(Eigen::Vector3f(rrang(-1, 1),rrang(-1, 1),rrang(-1, 1)),
				Eigen::Vector3f(0,0,0), 1.0f));
	}


    while (!glfwWindowShouldClose(window)) {
    	std::cout << "step" << std::endl;
    	for(unsigned int i=0; i<pManager.particles.size(); i++) {
			std::vector<Particle*> neigbours = pManager.getParticles(pManager.particles[i]);
			for(unsigned int j=0; j<neigbours.size(); j++) {
				if (i != j) {
					pManager.particles[i]->interact(pManager.particles[j]);
				}
			}

		}
    	for(unsigned int i=0; i<pManager.particles.size(); i++) {
    		pManager.particles[i]->step();
    	}

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPointSize(2.0f);
        glColor3f(1.f, 1.f, 1.f);
        glBegin(GL_POINTS);

        for(unsigned int i=0; i<pManager.particles.size(); i++) {
        	Particle *p = pManager.particles[i];
        	glVertex3f((float)p->location[0], (float)p->location[1], (float)p->location[2]);

        }

        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}




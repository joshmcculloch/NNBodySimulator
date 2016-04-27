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
#include "GL/glu.h"
#include "constants.h"

float delta_time = 1;
float current_time = 0;

static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "Key: " << (int)(key == GLFW_KEY_ESCAPE) << "\ni";
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        delta_time *= 1.5;
        std::cout << "Time Step: " << delta_time << std::endl;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        delta_time /= 1.5;
        std::cout << "Time Step: " << delta_time << std::endl;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "Current Time: " << (current_time/(3600*24)) << " Days" << std::endl;
    }
}

float rrang(double _min, double _max) {
	return (double)rand() * (_max - _min) / (double)RAND_MAX + _min;
}

void drawFloor() {
	glColor3f(0.3f, 0.3f, 0.3f);
	glBegin(GL_LINES);

	for(int x=-1000*1000*400; x<=1000*1000*400; x+=1000*1000*50) {
		glVertex3f(x,0,-1000*1000*400);
		glVertex3f(x,0,1000*1000*400);
	}
	for(int z=-1000*1000*400; z<=1000*1000*400; z+=1000*1000*50) {
		glVertex3f(-1000*1000*400,0,z);
		glVertex3f(1000*1000*400,0,z);
	}

	glEnd();
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
    glEnable(GL_DEPTH_TEST);

	ParticleManager pManager = ParticleManager();


	//Add Earth
	Eigen::VectorXd location(3);
	location << 0,0,0;
	Eigen::VectorXd velocity(3);
	velocity << 0,0,0;
	pManager.addParticle(new Particle(location, velocity, 5972370000000000000000000.0, 5514));

	//Add Moon
	Eigen::VectorXd locationM(3);
	//locationM << 362600000,0,0;
	locationM <<   405400000,0,0;
	Eigen::VectorXd velocityM(3);
	velocityM << 0,681.5,-681.5;//0,964.0,0;
	pManager.addParticle(new Particle(locationM, velocityM, 73420000000000000000000.0, 3344));

	//Offset moons momentum
	double momentum = pManager.particles[1]->mass * pManager.particles[1]->velocity[1];
	pManager.particles[0]->velocity[1] = -momentum / pManager.particles[0]->mass;
	momentum = pManager.particles[1]->mass * pManager.particles[1]->velocity[2];
	pManager.particles[0]->velocity[2] = -momentum / pManager.particles[0]->mass*5;

/*
	for(unsigned int i=0; i<1000; i++) {
		Eigen::VectorXd location(3);
		location << rrang(-300000000, 300000000),rrang(-300000000, 300000000),rrang(-300000000, 300000000);
		Eigen::VectorXd velocity(3);
		velocity << rrang(-2000,2000),rrang(-2000,2000),rrang(-2000,2000);
		pManager.addParticle(new Particle(location, velocity, 73420000000000000000000.0, 33440));
	}*/

    while (!glfwWindowShouldClose(window)) {
    	//std::cout << delta_time << std::endl;
      	for(unsigned int i=0; i<pManager.particles.size(); i++) {
    		if (pManager.particles[i]->active) {
				std::vector<Particle*> neigbours = pManager.getParticles(pManager.particles[i]);
				for(unsigned int j=0; j<neigbours.size(); j++) {
					if (i != j && pManager.particles[j]->active) {
						pManager.particles[i]->interact(pManager.particles[j],delta_time);

					}
				}
    		}

		}

      	Eigen::VectorXd cm(3);
      	cm << 0,0,0;
      	double tmass = 0;

    	for(unsigned int i=0; i<pManager.particles.size(); i++) {
    		if(pManager.particles[i]->active) {
    			cm += pManager.particles[i]->location*pManager.particles[i]->mass;
    			tmass += pManager.particles[i]->mass;
    			pManager.particles[i]->step(delta_time);
    		}
    	}

    	cm /= tmass;
    	current_time += delta_time;

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio*1000*500000, ratio*1000*500000, -1000*500000, 1000*500000, -1000*500000, 1000*500000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(10,10,10,0,0,0,0,1,0);

        GLUquadric *gluQuad = gluNewQuadric();
        glTranslatef(-(float)cm[0],-(float)cm[1],-(float)cm[2]);
        drawFloor();




        for(unsigned int i=0; i<pManager.particles.size(); i++) {
        	Particle *p = pManager.particles[i];
        	if (p->active) {
				glPushMatrix();
					glTranslatef((float)p->location[0], (float)p->location[1], (float)p->location[2]);
					if (p->radius > 10) {
						glColor3f(1.f, 1.f, 1.f);
						gluSphere(gluQuad, p->radius, 10, 10);
					} else  {
						glColor3f(0.f, 1.f, 0.f);
						gluSphere(gluQuad, 10, 10, 10);
					}
				glPopMatrix();
        	}
        }

        glColor3f(1.0f, 0,0);
        glBegin(GL_LINES);
        for(unsigned int i=0; i<pManager.particles.size(); i++) {
        	Particle *p = pManager.particles[i];
        	if (p->active) {
				glVertex3f((float)p->location[0],(float)p->location[1],(float)p->location[2]);
				glVertex3f((float)p->location[0],(float)0,(float)p->location[2]);
        	}
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}




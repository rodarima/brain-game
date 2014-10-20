#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#define NEURON_SIZE	3
#define FRAME_SLEEP	3
#define AXON_COLOR	glColor3f(1, 1, 1)
#define NEURON_1_COLOR	glColor3f(1, 1, 1)
#define NEURON_2_COLOR	glColor3f(0, 1, 0)
#define NEURON_3_COLOR	glColor3f(1, 0, 0)

struct point_t
{
	double x;
	double y;
};

struct neuron_t
{
	struct point_t p;
	int state;
	int next_state;
};

struct axon_t
{
	struct neuron_t *s, *d;
};

double game_speed = 1.0;
struct list_t neurons;
struct list_t axons;

float r = 0.0;
float scale = 1.0;
double center_x = 0, center_y = 0;
double width = 500, height = 500;

void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 500*scale, 500*scale, 0, -1, 1);
}

int cmp_neuron(const void* a, const void *b)
{
	if(a == b) return 0;
	return 1;
}

void neuron_next_state(struct neuron_t *neuron)
{
	if(neuron->state != 0)
	{
		neuron->next_state = (neuron->state+1) % 3;
		return;
	}

	struct list_node_t *node;
	struct axon_t *axon;
	int power = 0;
	int nneurons = 0;
	struct list_t nlist;
	list_empty(&nlist);
	for(node = axons.start; node != NULL; node = node->next)
	{
		axon = (struct axon_t *) node->ptr;
		if(axon->d == neuron)
		{
			if(list_find(&nlist, axon->s, cmp_neuron) == NULL)
			{
				/* Nueva neurona */
				list_add(&nlist, axon->s);

				nneurons++;
			}
			if(axon->s->state == 1)
			{
				power++;
			}
			else if(axon->s->state == 2)
			{
				power--;
			}
		}
	}
	list_clear(&nlist);
	if(nneurons == 0) nneurons = 1;
	int nec_power = (int)ceil(((double)nneurons)/2);
	//printf("POWER FOR %p = %d/%d\n", neuron, power, nec_power);
	if(power >= nec_power)
	{
		neuron->next_state = 1;
	}

}

void update_scenario()
{
	struct list_node_t *node;
	struct neuron_t *neuron;
	
	for(node = neurons.start; node != NULL; node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		neuron_next_state(neuron);
	}
	
	for(node = neurons.start; node != NULL; node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		neuron->state = neuron->next_state;
	}

}

int frames = 0;
void display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(
		center_x - ( width / 2.0 ),
		center_x + ( width / 2.0 ),
		center_y - ( height / 2.0 ),
		center_y + ( height / 2.0 ),
		-1,
		1
	);

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);


	struct list_node_t *node;
	struct neuron_t *neuron;
	float x, y;
	struct axon_t *axon;
	int x0, y0, x1, y1;
	for(node = axons.start; node != NULL; node = node->next)
	{
		AXON_COLOR;
		glLoadIdentity();
		axon = (struct axon_t *) node->ptr;
		x0 = axon->s->p.x;
		y0 = axon->s->p.y;
		x1 = axon->d->p.x;
		y1 = axon->d->p.y;

		glLoadIdentity();
		glBegin(GL_LINES);
			glVertex2f(x0, y0);
			glVertex2f(x1, y1);
		glEnd();
	}
	for(node = neurons.start; node != NULL; node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		x = neuron->p.x;
		y = neuron->p.y;

		if(neuron->state == 0)
			NEURON_1_COLOR;
		else if(neuron->state == 1)
			NEURON_2_COLOR;
		else
			NEURON_3_COLOR;
		
		glLoadIdentity();
		//	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		//	glTranslatef((float)x, (float)y, 0.0f);
		//
		float x1 = x-NEURON_SIZE, x2 = x+NEURON_SIZE, y1 = y-NEURON_SIZE, y2 = y+NEURON_SIZE;
		glBegin(GL_QUADS);
			glVertex2f(x1, y1);
			glVertex2f(x2, y1);
			glVertex2f(x2, y2);
			glVertex2f(x1, y2);
		glEnd();
	}
/*
    glColor3ub( 255, 0, 0 );
    glBegin( GL_TRIANGLES );
    glVertex2i( 0, 0 );
    glVertex2i( 150, 0 );
    glVertex2i( 0, 150 );
    glVertex2i( 0, 0 );
    glVertex2i( -150, 0 );
    glVertex2i( 0, -150 );
    glEnd();
*/
	glFlush();
	glutSwapBuffers();

	r+= 10.0;
	if(r>360.0)
	{
		r = 0.0;
	}

	frames++;
	if(frames > round(FRAME_SLEEP/game_speed))
	{
		update_scenario();
		frames = 0;
	}
}

void move_neuron(double rx, double ry, double x, double y)
{
	printf("MOVE: (%f,%f) -> (%f,%f)\n", rx, ry, x, y);
	struct list_node_t *node;
	struct neuron_t *neuron;
	double nx, ny;
	for(node = neurons.start; node != NULL; node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		nx = neuron->p.x;
		ny = neuron->p.y;

		if((nx-NEURON_SIZE <= rx) && (nx+NEURON_SIZE >= rx))
		{
			if((ny-NEURON_SIZE <= ry) && (ny+NEURON_SIZE >= ry))
			{
				neuron->p.x = x;
				neuron->p.y = y;
				break;
			}
		}
	}

}

void remove_neuron_axons(struct neuron_t *neuron)
{
	struct list_node_t *node, *tmp = NULL;
	struct axon_t *axon;
	node = axons.start;
	while(node)
	{
		axon = (struct axon_t *) node->ptr;
		if((axon->s == neuron) || (axon->d == neuron))
		{
			printf("REMOVING AXON %p\n", axon);
			tmp = node->next;
			list_remove_node(&axons, node);
			node = tmp;
			free(axon);
		}
		else
		{
			node = node->next;
		}
	}

}

struct neuron_t *find_neuron(double x, double y, double radio)
{
	struct list_node_t *node;
	struct neuron_t *neuron;
	double nx, ny;
	for(node = neurons.start; node != NULL; node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		nx = neuron->p.x;
		ny = neuron->p.y;

		if((nx-radio <= x) && (nx+radio >= x))
		{
			if((ny-radio <= y) && (ny+radio >= y))
			{
				return neuron;
			}
		}
	}
	return NULL;
}

void disconnect_neuron(double x, double y)
{
	struct list_node_t *node;
	struct neuron_t *neuron;
	double nx, ny;
	for(node = neurons.start; node != NULL; node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		nx = neuron->p.x;
		ny = neuron->p.y;

		if((nx-NEURON_SIZE <= x) && (nx+NEURON_SIZE >= x))
		{
			if((ny-NEURON_SIZE <= y) && (ny+NEURON_SIZE >= y))
			{
				remove_neuron_axons(neuron);
				list_remove_node(&neurons, node);
				free(neuron);
				printf("free(%p)\n", neuron);
				return;
			}
		}
	}
}

void connect_neuron(double x0, double y0, double x1, double y1)
{
	printf("CONNECT: (%f,%f) -> (%f,%f)\n", x0, y0, x1, y1);
	struct list_node_t *node;
	struct neuron_t *s, *d, *neuron;
	double nx, ny;
	int n0 = 0, n1 = 0;

	for(node = neurons.start; node != NULL; node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		nx = neuron->p.x;
		ny = neuron->p.y;
		if(!n0)
		{
			if((nx-NEURON_SIZE <= x0) && (nx+NEURON_SIZE >= x0))
			{
				if((ny-NEURON_SIZE <= y0) && (ny+NEURON_SIZE >= y0))
				{
					n0 ^= 1;
					s = neuron;
				}
			}
		}
		if(!n1)
		{
			if((nx-NEURON_SIZE <= x1) && (nx+NEURON_SIZE >= x1))
			{
				if((ny-NEURON_SIZE <= y1) && (ny+NEURON_SIZE >= y1))
				{
					n1 ^= 1;
					d = neuron;
				}
			}
		}
		if(n0 && n1) break;
	}
	if(!(n0 && n1))
	{
		printf("CONNECTION FAILED!\n");
		return;
	}
	if(s == d) return;

	struct axon_t *a = malloc(sizeof(struct axon_t));
	if(!a)
	{
		perror("malloc");
		exit(1);
	}

	a->s = s;
	a->d = d;
	
	list_add(&axons, a);
}

double mouse0_x = 0, mouse0_y = 0;
double mouse1_x = 0, mouse1_y = 0;

void get_mouse_absolute(int mx, int my, double *rx, double *ry)
{
	// flip mouse y axis so up is +y
	my = glutGet( GLUT_WINDOW_HEIGHT ) - my;

	// convert mouse coords to (-1/2,-1/2)-(1/2, 1/2) box
	double x = ( mx / (double)glutGet( GLUT_WINDOW_WIDTH ) ) - 0.5;
	double y = ( my / (double)glutGet( GLUT_WINDOW_HEIGHT ) ) - 0.5;

	// move to center
	*rx = center_x + (x * width);
	*ry = center_y + (y * height);
}

void new_neuron(double rx, double ry, int state)
{

	struct neuron_t *n = malloc(sizeof(struct neuron_t));
	if(!n)
	{
		perror("malloc");
		exit(1);
	}
	n->p.x = NEURON_SIZE*3*round(rx/(NEURON_SIZE*3));
	n->p.y = NEURON_SIZE*3*round(ry/(NEURON_SIZE*3));
	n->state = state;
	n->next_state = 0;
	list_add(&neurons, n);
	printf("NEW NEURON ON (%f,%f)\n", n->p.x, n->p.y);
}

int get_index_neuron(struct neuron_t *n)
{
	struct list_node_t *node;
	struct neuron_t *neuron;
	int i;
	for(i=0,node = neurons.start; node != NULL; i++, node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		if(neuron == n) return i;
	}
	return -1;
}

void new_axon(int sp, int dp)
{
	struct neuron_t *s = NULL, *d = NULL;
	struct neuron_t *neuron;
	struct list_node_t *node;
	int i;
	//printf("AXON (%d->%d)\n", sp, dp);
	for(i = 0, node = neurons.start; node != NULL ; i++, node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		if(i == sp)
		{
			s = neuron;
			//printf("s = %p\n", s);
		}
		if(i == dp)
		{
			d = neuron;
			//printf("d = %p\n", d);
		}
	}
	if((s == NULL) && (d == NULL))
	{
		printf("ERROR axon incorrecto.\n");
		exit(1);
	}
	struct axon_t *a = malloc(sizeof(struct axon_t));
	if(!a)
	{
		perror("malloc");
		exit(1);
	}

	a->s = s;
	a->d = d;
	
	list_add(&axons, a);
}

void mouse(int button, int state, int mx, int my)
{
	double rx, ry;
	get_mouse_absolute(mx, my, &rx, &ry);
	
	//printf("MOUSE: %4d, %4d, %f, %f\n", button, state, rx, ry);
	
	if((button == 0) && (state == 0))
	{
		mouse0_x = rx;
		mouse0_y = ry;
	}
	else if((button == 0) && (state == 1))
	{
		if(((mouse0_x-NEURON_SIZE > rx) || (mouse0_x+NEURON_SIZE < rx)) ||
			((mouse0_y-NEURON_SIZE > ry) || (mouse0_y+NEURON_SIZE < ry)))
		{
			connect_neuron(mouse0_x, mouse0_y, rx, ry);
		}
		else
		{
			struct neuron_t *neuron;
			if((neuron = find_neuron(rx, ry, NEURON_SIZE*1.5)) == NULL)
			{
				new_neuron(rx, ry, 0);
			}
			else
			{
				neuron->state = 1;
			}
		}
	}
	else if((button == 2) && (state == 0))
	{
		mouse1_x = rx;
		mouse1_y = ry;
	}
	else if((button == 2) && (state == 1))
	{
		if(((mouse1_x-NEURON_SIZE > rx) || (mouse1_x+NEURON_SIZE < rx)) ||
			((mouse1_y-NEURON_SIZE > ry) || (mouse1_y+NEURON_SIZE < ry)))
		{
			rx = NEURON_SIZE*3*round(rx/(NEURON_SIZE*3));
			ry = NEURON_SIZE*3*round(ry/(NEURON_SIZE*3));
			move_neuron(mouse1_x, mouse1_y, rx, ry);
		}
		else
		{
			disconnect_neuron(rx, ry);
		}

	}
	else if(((button == 3) || (button == 4)) && (state == 0))
	{
		// flip mouse y axis so up is +y
		my = glutGet( GLUT_WINDOW_HEIGHT ) - my;

		// convert mouse coords to (-1/2,-1/2)-(1/2, 1/2) box
		double x = ( mx / (double)glutGet( GLUT_WINDOW_WIDTH ) ) - 0.5;
		double y = ( my / (double)glutGet( GLUT_WINDOW_HEIGHT ) ) - 0.5;

		double preX = ( x * width );
		double preY = ( y * height );

		int modif = glutGetModifiers();
		double zoomFactor = 1.2;
		if( button == 3 )
		{
			// zoom in
			if(!(modif & GLUT_ACTIVE_CTRL))
			{
				width /= zoomFactor;
				height /= zoomFactor;
			}
			game_speed /= zoomFactor;
		}
		if( button == 4 )
		{
			// zoom out
			if(!(modif & GLUT_ACTIVE_CTRL))
			{
				width *= zoomFactor;
				height *= zoomFactor;
			}
			game_speed *= zoomFactor;
		}

		double postX = ( x * width );
		double postY = ( y * height );

		// recenter
		center_x += ( preX - postX );
		center_y += ( preY - postY );
	}
}

void mouse_move(int x, int y)
{
	//printf("MOUSE: %4d, %4d\n", x, y);
	//mouse(0,0,x,y);
}


void init()
{
	glClearColor(0,0,0,0);
}

char *level_name = NULL;

void load_level()
{
	if(level_name == NULL)
	{
		printf("NO LEVEL NAME\n");
		return;
	}
	FILE *level;
	level = fopen(level_name, "r+");
	if(level == NULL)
	{
		perror("fopen");
		return;
	}
	char txt[1024];
	if(fgets(txt, 1024, level) == NULL)
	{
		perror("fgets");
		exit(1);
	}
	if(strcmp(txt, "neurons:\n") == 0)
	{
		printf("neurons: GOT!\n");
	}
	else
	{
		printf("GOT: %s\n", txt);
	}
	if(fgets(txt, 1024, level) == NULL)
	{
		perror("fgets");
		exit(1);
	}
	while(strcmp(txt, "axons:\n") != 0)
	{
		double x, y;
		int n, s;
		sscanf(txt, "%d, %lf, %lf, %d", &n, &x, &y, &s);
		new_neuron(x, y, s);
		printf("NEW NEURON: %d, %lf, %lf, %d\n", n, x, y, s);
		if(fgets(txt, 1024, level) == NULL)
		{
			perror("fgets");
			exit(1);
		}
	}
	while(fgets(txt, 1024, level) != NULL)
	{
		int s, d;
		sscanf(txt, "%d, %d", &s, &d);
		new_axon(s, d);
		printf("NEW AXON: %d -> %d\n", s, d);
	}

	fclose(level);
}
void clear_level()
{
	list_empty(&neurons);
	list_empty(&axons);
}

void save_level()
{
	if(level_name == NULL)
	{
		printf("NO LEVEL NAME\n");
		return;
	}
	FILE *level;
	level = fopen(level_name, "w+");
	if(level == NULL)
	{
		perror("fopen");
		return;
	}
	struct list_node_t *node;
	struct neuron_t *neuron;
	struct axon_t *axon;
	int i, s, d;

	fprintf(level, "neurons:\n");
	for(i=0,node = neurons.start; node != NULL; i++, node = node->next)
	{
		neuron = (struct neuron_t *) node->ptr;
		fprintf(level, "%d, %lf, %lf, %d\n",
				i,
				neuron->p.x,
				neuron->p.y,
				neuron->state);
	}
	fprintf(level, "axons:\n");
	for(node = axons.start; node != NULL; node = node->next)
	{
		axon = (struct axon_t *) node->ptr;
		s = get_index_neuron(axon->s);
		d = get_index_neuron(axon->d);
		fprintf(level, "%d, %d\n",
				s,
				d);
	}
	fclose(level);
	printf("LEVEL SAVED!\n");
}

void key_pressed(unsigned char key, int x, int y)
{
	if(key == 'q')
	{
		exit(0);
	}
	else if(key == 's')
	{
		save_level();
	}
	else if(key == 'c')
	{
		clear_level();
	}
}

int main(int argc, char **argv)
{
	printf("argc = %d\n", argc);
	if(argc == 2)
	{
		level_name = argv[1];
	}
	list_empty(&neurons);
	list_empty(&axons);

	if(level_name) load_level();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Hello OpenGL");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_move);
	glutKeyboardFunc(key_pressed);
	glutMainLoop();
	return 0;
}

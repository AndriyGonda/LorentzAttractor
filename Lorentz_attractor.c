#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>


#define MaxX  ((100))
#define MaxZ  ((100))
#define MaxY  ((100))
#define NPOINTS 20000u 
#define DIM(ARRAY_NAME) (ARRAY_NAME[NPOINTS])
static GLfloat DIM(x), DIM(y),DIM(z);
static int np=1;
// Rotation amounts and zoom 
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f; 
static GLfloat Scale = 1.0f;

GLint  iMultiSample = 0;
GLint  iNumSamples = 0;

/** The main caclulation function for f(x,y,t) = sin(x+t)*cos(y+t) */
void calculate(void);

/** Change viewing volume and viewport.  Called when window is resized*/
void ChangeSize(GLsizei w, GLsizei h);

/** Setup render scene*/
void SetupRC(); 

/**Set the functional keys
 * Up,Down  - y-rotation
 * Left,Right - x-rotation
 * PageUp,PageDown  - z-rotation
 * F5 - allow zoom +
 * F6 - allow zoom - */
void SpecialKeys(int key, int x, int y);

/** Called to draw scene*/
void RenderScene(void);
/** callback that call everytime */
inline void idle_plot(void);

/** setup plotting*/
void series_plot(void);

int main(int argc, char *argv[])
    {
		
		for (int i=0; i<NPOINTS; i++)
		 {
			x[i] = 1;// 3.051522;
			y[i] =0;// 1.582542;
			z[i] = 0;///15.62388;
		 }
		calculate();
		/**using GLUT for display*/
		glutInit(&argc, argv);
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
         
		glutInitWindowSize(800, 600);
		glutCreateWindow("Lorentz atractor 3D");
		
		glutReshapeFunc(ChangeSize);
		glutSpecialFunc(SpecialKeys);
		glutDisplayFunc(RenderScene);
		glutIdleFunc(idle_plot);
		SetupRC();
		glutMainLoop();
		
	 return 0;
    }

void ChangeSize(GLsizei w, GLsizei h)
    {
		GLfloat fAspect;
	 
		if(h == 0) h = 1;

		glViewport(0, 0, w, h);
	 
		fAspect = (GLfloat) w / (GLfloat)h;
	 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		gluPerspective(60.0f, fAspect, 1.0, 1e10f);
		gluLookAt(MaxX, MaxY, MaxZ, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
    }

void SetupRC()
    {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
		glEnable(GL_DEPTH_TEST);   
		glFrontFace(GL_CW);  
		glEnable(GL_CULL_FACE);
		
    }
    

void SpecialKeys(int key, int x, int y)
    {
		if(key == GLUT_KEY_UP)
			xRot-= 5.0f;
	 
		if(key == GLUT_KEY_DOWN)
			xRot += 5.0f;
	 
		if(key == GLUT_KEY_LEFT)
			yRot -= 5.0f;
	 
		if(key == GLUT_KEY_RIGHT)
			yRot += 5.0f;
		
		 if(key == GLUT_KEY_PAGE_DOWN)
			zRot -= 5.0f;
		 
		  if(key == GLUT_KEY_PAGE_UP)
			zRot += 5.0f;
			
		 if (key ==GLUT_KEY_F5) 
				Scale+=0.1f;   
			
		  if (key == GLUT_KEY_F6) 
				Scale-=0.1f;     
				   
			xRot = (GLfloat)((const int)xRot % 360);
			yRot = (GLfloat)((const int)yRot % 360);
			zRot = (GLfloat)((const int)zRot % 360);
		glutPostRedisplay();
    }
 

void RenderScene(void)
    {
    // Clear the window with current clearing color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    
		// Save the matrix state and do the rotations
	glPushMatrix();
	  //  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
       // glLightfv(GL_LIGHT0, GL_POSITION, light_position);
       // glEnable(GL_LIGHT0);
       // glEnable(GL_LIGHTING);   
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		if (Scale<0.5f) Scale = 0.5f; 
		glScalef(Scale, Scale, Scale);
	 
		//draw the ortho lines
		 glLineWidth(4u);
		 glBegin(GL_LINES);
			 glColor3f(1.0f, 0.0f, 0.0f);
			 glVertex3f(-MaxX*0.1f, 0.0f, 0.0f);
			 glVertex3f(MaxX*0.1f, 0.0f, 0.0f);
		 glEnd();
		 
		 glBegin(GL_LINES);
			 glColor3f(0.0, 1.0, 0.0);
			 glVertex3f(0, -MaxY*0.1, 0);
			 glVertex3f(0, MaxY*0.1, 0);
		 glEnd();
		 

		 glBegin(GL_LINES);
			 glColor3f(0.0f, 0.0f, 1.0f);
			 glVertex3f(0.0f, 0.0f, -MaxZ*0.1f);
			 glVertex3f(0.0f, 0.0f,MaxZ*0.1f);
		 glEnd();   
		
		 series_plot();	
	   
	glPopMatrix();
	 
		// Buffer swap
		glutSwapBuffers();
}

void idle_plot(void)
 {
	
	 if (np < NPOINTS) np+=1;
	calculate();
	glutPostRedisplay();
 }

inline void series_plot(void)
 {
   glEnable(GL_POINT_SMOOTH);
   glPointSize(3u); 
   glColor3f(0.0f,0.5f,0.0f); 
   
     glBegin(GL_POINTS);
	   for (unsigned int i=0u; i<np; i++)
			   glVertex3f(x[i],y[i],z[i]);
	 glEnd(); 
	 
    
 }

void calculate(void)
 {
	double DT = 1e-2;
	double sigma  =  10;
	double  r  = 20;
	double b  =   8.f/3.f;
	double xn,yn,zn;
	 for (unsigned i = 0 ; i< np; i++)
	  {
		xn=  x[i] + sigma*(y[i]-x[i])*DT;
		yn = y[i] + (x[i]*(r- z[i]) - y[i])*DT;
		zn  =z[i]  + (x[i]*y[i] - b*z[i])*DT;
		x[i] = xn;
		z[i] = zn;
		y[i] = yn;  
	  }
	
 }

#include "Board.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include "Graphics.h"
#include "Image.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <assert.h>
#include <fstream>

pthread_mutex_t UpdateLock;
Board FinalBoard;
bool PauseBoard;

GLuint _textureId;
 
double rotate_y=0; 
double rotate_x=0;


struct Graph {
    int x1;
    char **s1;
};

//TODO: (still not gonna use TODOIST :P)
 //     glutMotionFunc( ) for rotating
//      glScalef() for zooming in and out
//      give texture to the walls
//      give thickness to the walls
namespace {
    //Converts a four-character array to an integer, using little-endian form
    int toInt(const char* bytes) {
        return (int)(((unsigned char)bytes[3] << 24) |
                     ((unsigned char)bytes[2] << 16) |
                     ((unsigned char)bytes[1] << 8) |
                     (unsigned char)bytes[0]);
    }
    
    //Converts a two-character array to a short, using little-endian form
    short toShort(const char* bytes) {
        return (short)(((unsigned char)bytes[1] << 8) |
                       (unsigned char)bytes[0]);
    }
    
    //Reads the next four bytes as an integer, using little-endian form
    int readInt(ifstream &input) {
        char buffer[4];
        input.read(buffer, 4);
        return toInt(buffer);
    }
    
    //Reads the next two bytes as a short, using little-endian form
    short readShort(ifstream &input) {
        char buffer[2];
        input.read(buffer, 2);
        return toShort(buffer);
    }
    
    //Just like auto_ptr, but for arrays
    template<class T>
    class auto_array {
        private:
            T* array;
            mutable bool isReleased;
        public:
            explicit auto_array(T* array_ = NULL) :
                array(array_), isReleased(false) {
            }
            
            auto_array(const auto_array<T> &aarray) {
                array = aarray.array;
                isReleased = aarray.isReleased;
                aarray.isReleased = true;
            }
            
            ~auto_array() {
                if (!isReleased && array != NULL) {
                    delete[] array;
                }
            }
            
            T* get() const {
                return array;
            }
            
            T &operator*() const {
                return *array;
            }
            
            void operator=(const auto_array<T> &aarray) {
                if (!isReleased && array != NULL) {
                    delete[] array;
                }
                array = aarray.array;
                isReleased = aarray.isReleased;
                aarray.isReleased = true;
            }
            
            T* operator->() const {
                return array;
            }
            
            T* release() {
                isReleased = true;
                return array;
            }
            
            // void reset(T* array_ = NULL) {
            //     if (!isReleased && array != NULL) {
            //         delete[] array;
            //     }
            //     array = array_;
            // }
            
            T* operator+(int i) {
                return array + i;
            }
            
            T &operator[](int i) {
                return array[i];
            }
    };
}

Image* loadBMP(const char* filename) {
    ifstream input;
    input.open(filename, ifstream::binary);
    assert(!input.fail() || !"Could not find file");
    char buffer[2];
    input.read(buffer, 2);
    assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
    input.ignore(8);
    int dataOffset = readInt(input);
    
    //Read the header
    int headerSize = readInt(input);
    int width;
    int height;
    width = readInt(input);
    height = readInt(input);
    
    //Read the data
    int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
    int size = bytesPerRow * height;
    auto_array<char> pixels(new char[size]);
    input.seekg(dataOffset, ios_base::beg);
    input.read(pixels.get(), size);
    
    //Get the data into the right format
    auto_array<char> pixels2(new char[width * height * 3]);
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            for(int c = 0; c < 3; c++) {
                pixels2[3 * (width * y + x) + c] =
                    pixels[bytesPerRow * y + 3 * x + (2 - c)];
            }
        }
    }
    
    input.close();
    return new Image(pixels2.release(), width, height);
}

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                 //Always GL_TEXTURE_2D
                 0,                             //0 for now
                 GL_RGB,                        //Format OpenGL uses for image
                 image->width, image->height,   //Width and height
                 0,                             //The border of the image
                 GL_RGB,                        //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE,              //GL_UNSIGNED_BYTE, because pixels are stored
                                                //as unsigned numbers
                 image->pixels);                //The actual pixel data
    return textureId;                           //Returns the id of the texture
}


void initRendering() 
{
   	Image* image = loadBMP("BrickWall.bmp");
  	_textureId = loadTexture(image);
    delete image;
}

void mouseclick(int button,int state,int x,int y )
{
     // glutMotionFunc( )
    if(state== GLUT_UP )
    {
        int const window_width  = glutGet(GLUT_WINDOW_WIDTH);
        int const window_height = glutGet(GLUT_WINDOW_HEIGHT);
        float const window_aspect = (float)window_width / (float)window_height;
        // cout<<x<<'\t'<<y<<endl;
        float f1=window_width/1000.0;
        float f2=window_height/500.0;
        // cout<<(x>920*f1)<<"     "<<(x<998*f2)<<endl;
        if(x>920*f1 && x<998*f1 && y>435*f2 && y<498*f2)
        {
            PauseBoard=false;
            cout<<"Play Button"<<endl;
        }
        if(x>840*f1 && x<920*f1 && y>435*f2 && y<498*f2)
        {
            PauseBoard=true;
            cout<<"Pause Button"<<endl;
        }
        if(x>615*f1 && x<835*f1 && y>435*f2 && y<498*f2)
        {
            PauseBoard=true;
            Ball newBalltoAdd= Ball(FinalBoard.GetDimensionX(),FinalBoard.GetDimensionY(),1);
            while (!CheckCorrect(FinalBoard.GetVectorBalls(), newBalltoAdd))
            {
                newBalltoAdd=Ball(FinalBoard.GetDimensionX(), FinalBoard.GetDimensionY(),1);
            }
            FinalBoard.AddBallToBoard(newBalltoAdd);
            PauseBoard=false;
            cout<<"Add Button"<<endl;
        }
        if(x>80*f1 && x<155*f1 && y>435*f2 && y<498*f2)
        {
            cout<<"SpeedUp Button"<<endl;
        }
        if(x>0*f1 && x<80*f1 && y>435*f2 && y<498*f2)
        {
            cout<<"SlowDown Button"<<endl;
        }
    
    }
    glutPostRedisplay();
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 27: //Escape key
            exit(0);
    }
}
void specialKeys( int key, int x, int y ) 
{
 
  if (key == GLUT_KEY_RIGHT)
    rotate_y += 2;
 
  else if (key == GLUT_KEY_LEFT)
    rotate_y -= 2;
 
  else if (key == GLUT_KEY_UP)
    rotate_x += 2;
 
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 2;
 
  glutPostRedisplay();
 
}


void display(void)
{
	// cout<<"In display\n";
    int const window_width  = glutGet(GLUT_WINDOW_WIDTH);
    int const window_height = glutGet(GLUT_WINDOW_HEIGHT);
    float const window_aspect = (float)window_width / (float)window_height;

    glClearColor(0.5, 0.5, 1.0, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0-window_width, window_width,0- window_height,window_height,-2000,2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat const light_pos[4]     = {0.0f, 0, 0    , 1.0  };
    GLfloat const light_color[4]   = { 1,  1,  1, 1.};
    GLfloat const light_ambient[4] = { 0.10,  0.10,  0.30, 1.};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos),
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

    GLfloat const light_pos2[4]     = {float(0-FinalBoard.GetDimensionX()), float(0-FinalBoard.GetDimensionY()),  0  , 1.0  };
    GLfloat const light_color2[4]   = { 1,  0,  0, 1.};
    GLfloat const light_ambient2[4] = { 0.10,  0.10,  0.30, 1.};
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos2),
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color2);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_color2);

    GLfloat const light_pos3[4]     = {float(FinalBoard.GetDimensionX()),float(FinalBoard.GetDimensionY()),  0  , 1.0  };
    GLfloat const light_color3[4]   = { 0,  1,  0, 1.};
    GLfloat const light_ambient3[4] = { 0.10,  0.10,  0.30, 1.};
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos3),
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color3);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient3);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_color3);

    GLfloat const light_pos4[4]     = {float(0-FinalBoard.GetDimensionX()), float(FinalBoard.GetDimensionY()),  0  , 1.0  };
    GLfloat const light_color4[4]   = { 1,  1,  0, 1.};
    GLfloat const light_ambient4[4] = { 0.10,  0.10,  0.30, 1.};
    glLightfv(GL_LIGHT3, GL_POSITION, light_pos4),
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_color4);
    glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient4);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_color4);

    // GLfloat const light_pos5[4]     = {0,0, 300  , 1.0  };
    // GLfloat const light_color5[4]   = { 1,  1,  1, 1.};
    // GLfloat const light_ambient5[4] = { 0.10,  0.10,  0.30, 1.};
    // glLightfv(GL_LIGHT4, GL_POSITION, light_pos5),
    // glLightfv(GL_LIGHT4, GL_DIFFUSE, light_color5);
    // glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient5);
    // glLightfv(GL_LIGHT4, GL_SPECULAR, light_color5);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glEnable(GL_LIGHT1);
    // glEnable(GL_LIGHT2);
    // glEnable(GL_LIGHT3);
    // glEnable(GL_LIGHT4);

   

    glRotatef( rotate_x, 200, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 200, 0.0 );
    // glScalef( 1.0+rotate_x/100.0,1.0+rotate_x/100.0,1.0f ); 
    float f=(4.0/6.0)*min(window_width,window_height);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0f, 0.0f); 
    glTexCoord2f(1.0f, 0.0f);  
    glVertex3f( f , -f, -f);
    glTexCoord2f(1.0f, 1.0f); 
    glVertex3f(  f,  f, -f ); 
    glTexCoord2f(0.0f, 1.0f);   
    glVertex3f( -f,  f, -f ); 
    glTexCoord2f(0.0f, 0.0f);  
    glVertex3f( -f, -f, -f);      
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(  0.0,    1.0,  0 );
    glVertex3f( f, -f, -f );
    glVertex3f( f,  f, -f );
    glVertex3f( f,  f,  f );
    glVertex3f( f, -f,  f );
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(   0,  0,  1 );
    glVertex3f(  f, -f, -f );
    glVertex3f(  f, -f,  f );
    glVertex3f( -f, -f,  f );
    glVertex3f( -f, -f, -f );
    glEnd();

    for( int i=0;i<FinalBoard.GetNumberBalls();i++ ) 
    {
        glPushMatrix();
        // cout<<FinalBoard.GetVectorBalls()[i].GetX()<<"  "<<FinalBoard.GetVectorBalls()[i].GetY()<<endl;
        glColor3f((FinalBoard.GetBallFromId(i).GetColor().GetR())/255.0,(FinalBoard.GetBallFromId(i).GetColor().GetG())/255.0,(FinalBoard.GetBallFromId(i).GetColor().GetB())/255.0);
        glTranslatef(FinalBoard.GetBallFromId(i).GetX(), FinalBoard.GetBallFromId(i).GetY(),FinalBoard.GetBallFromId(i).GetZ());
        // cout <<FinalBoard.GetBallFromId(i).GetColor().GetR()/255.0<<"\n";
        GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.0f};
		// GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
		GLfloat shininess[] = {50};
		// glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        glutSolidSphere(FinalBoard.GetBallFromId(i).GetRadius(), 31, 10);
        glPopMatrix();
    }
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int x, int y)
{
	int w=glutGet(GLUT_WINDOW_WIDTH);
    int h=glutGet(GLUT_WINDOW_HEIGHT);
    double dim=(2.0*min(w,h))/3.0;

    FinalBoard.SetDimensionX(dim);
    FinalBoard.SetDimensionY(dim);
    FinalBoard.SetDimensionZ(dim);


    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0-w,w,0-h,h,-200,200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

int graphics(int argc,char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280,720);
    glutCreateWindow("Team BabeMagnets");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseclick);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(handleKeypress);
    initRendering();

    glutMainLoop();

    return 0;
}



// int counter=0

void *UpdateBoardThread(void* id)
{
	while (true)
	{
        if (!PauseBoard)
        {
    		pthread_mutex_lock (&UpdateLock);
    		long ballid = (long) id;
    		// cout <<"Starting for ball " <<ballid<<"\n";
    		
    		Ball BallConsidered = FinalBoard.GetBallFromId(ballid);
    
    		double BallConsidered_Coordx = BallConsidered.GetX(); 
    		double BallConsidered_Coordy = BallConsidered.GetY();
    		double BallConsidered_Coordz = BallConsidered.GetZ();
            double BallConsidered_Radius = BallConsidered.GetRadius();
    		double BallConsidered_VelocityX=BallConsidered.GetVelocityX();
    		double BallConsidered_VelocityY=BallConsidered.GetVelocityY();
    		double BallConsidered_VelocityZ=BallConsidered.GetVelocityZ();
            // BallConsidered.SetX(((BallConsidered_Coordx+BallConsidered_VelocityX)%(2*FinalBoard.GetDimensionX())) -FinalBoard.GetDimensionX());
    		if (BallConsidered_Coordx + BallConsidered_VelocityX + BallConsidered_Radius> FinalBoard.GetDimensionX())
    		{
    			BallConsidered.SetX(FinalBoard.GetDimensionX() -BallConsidered_Radius);
    			// BallConsidered.SetY(BallConsidered_Coordy+BallConsidered_VelocityY);
    			// BallConsidered.SetZ(BallConsidered)
                BallConsidered.SetVelocityX(0-BallConsidered.GetVelocityX());
    		}
    		else if (BallConsidered_Coordx+BallConsidered_VelocityX + FinalBoard.GetDimensionX() -BallConsidered_Radius<0)
    		{
    			BallConsidered.SetX(0-FinalBoard.GetDimensionX()+BallConsidered_Radius);
    			// BallConsidered.SetY(BallConsidered_Coordy+BallConsidered_VelocityY);
    			BallConsidered.SetVelocityX(0-BallConsidered.GetVelocityX());	
    		}
    		else
    		{
    			BallConsidered.SetX(BallConsidered_Coordx+BallConsidered_VelocityX);
    		}
    
            if (BallConsidered_Coordy+BallConsidered_VelocityY +BallConsidered_Radius> FinalBoard.GetDimensionY())
            {
                BallConsidered.SetY(FinalBoard.GetDimensionY()-BallConsidered_Radius);
                // BallConsidered.SetX(BallConsidered_Coordx+BallConsidered_VelocityX);
                BallConsidered.SetVelocityY(0-BallConsidered.GetVelocityY());
            }
            else if (BallConsidered_Coordy+BallConsidered_VelocityY + FinalBoard.GetDimensionY() -BallConsidered_Radius <0)
            {
                BallConsidered.SetY(0-FinalBoard.GetDimensionY() + BallConsidered_Radius);
                // BallConsidered.SetX(BallConsidered_Coordx+BallConsidered_VelocityX);    
                BallConsidered.SetVelocityY(0-BallConsidered.GetVelocityY());   
            }
            else
            {
                BallConsidered.SetY(BallConsidered_Coordy+BallConsidered_VelocityY);
            }
            
    
            if (BallConsidered_Coordz+BallConsidered_VelocityZ +BallConsidered_Radius> FinalBoard.GetDimensionZ())
            {
                BallConsidered.SetZ(FinalBoard.GetDimensionZ()-BallConsidered_Radius);
                // BallConsidered.SetZ(BallConsidered_Coordz+BallConsidered_VelocityZ);
                BallConsidered.SetVelocityZ(0-BallConsidered.GetVelocityZ());
            }
            else if (BallConsidered_Coordz+BallConsidered_VelocityZ + FinalBoard.GetDimensionZ() -BallConsidered_Radius <0)
            {
                BallConsidered.SetZ(0-FinalBoard.GetDimensionZ() + BallConsidered_Radius);
                // BallConsidered.SetZ(BallConsidered_Coordz+BallConsidered_VelocityZ);    
                BallConsidered.SetVelocityZ(0-BallConsidered.GetVelocityZ());   
            }
            else
            {
                BallConsidered.SetZ(BallConsidered_Coordz+BallConsidered_VelocityZ);
            }
            
    
    		vector<Ball> Vector_Of_Balls = FinalBoard.GetVectorBalls();
    		double mass1 = BallConsidered.GetRadius()*BallConsidered.GetRadius()*BallConsidered.GetRadius();
    		double ux1,uy1,uz1; 

    		for(int i=0;i<FinalBoard.GetNumberBalls();i++)
    		{
    			if (i != ballid)
    			{
    				ux1 =  BallConsidered.GetVelocityX();
                    uy1 =  BallConsidered.GetVelocityY();
                    uz1 =  BallConsidered.GetVelocityZ();
    		
    				double dx = Vector_Of_Balls[i].GetX()-BallConsidered.GetX();
    				double dy = Vector_Of_Balls[i].GetY()-BallConsidered.GetY();
                    double dz = Vector_Of_Balls[i].GetZ()-BallConsidered.GetZ();

                    double distance = sqrt(dx*dx + dy*dy + dz*dz);

    				if(BallConsidered.GetRadius()+Vector_Of_Balls[i].GetRadius()>=distance)
    				{
    					cout <<BallConsidered.GetRadius() <<"\t" <<Vector_Of_Balls[i].GetRadius()<<"\t"<<distance<<"\n";
    					
                        double l=(BallConsidered.GetRadius()+Vector_Of_Balls[i].GetRadius() -distance)/2;
    
    					double mass2 = Vector_Of_Balls[i].GetRadius()*Vector_Of_Balls[i].GetRadius()*Vector_Of_Balls[i].GetRadius(); 
    					
                        double ux2 = Vector_Of_Balls[i].GetVelocityX();
    					double uy2 = Vector_Of_Balls[i].GetVelocityY();
                        double uz2 = Vector_Of_Balls[i].GetVelocityZ();
    
    					// Along radial
    					double u1=(ux1*dx + uy1*dy + uz1*dz)/distance;
    					double u2=(ux2*dx + uy2*dy + uz2*dz)/distance;
                        
                        // //Along XYZ unchanged (tangential)
                        // double vx1=ux1 - (u1*dx)/distance;
                        // double vy1=uy1 - (u1*dy)/distance;
                        // double vz1=uz1 - (u1*dz)/distance;
                        // double vx2=ux2 - (u2*dx)/distance;
                        // double vy2=uy2 - (u2*dy)/distance;
                        // double vz2=uz2 - (u2*dz)/distance;

    					//After collision radial
    					double v1=(mass1*u1 + 2*mass2*u2 - mass2*u1)/(mass1+mass2);
    					double v2=(mass2*u2 + 2*mass1*u1 - mass1*u2)/(mass1+mass2);
                        
                        //Along XYZ unchanged (tangential)
                        double vx1=ux1 + ((v1-u1) *dx)/distance;
                        double vy1=uy1 + ((v1-u1) *dy)/distance;
                        double vz1=uz1 + ((v1-u1) *dz)/distance;
                        double vx2=ux2 + ((v2-u2) *dx)/distance;
                        double vy2=uy2 + ((v2-u2) *dy)/distance;
                        double vz2=uz2 + ((v2-u2) *dz)/distance;


    					BallConsidered.SetX(BallConsidered.GetX() - (l*dx)/distance);
    					BallConsidered.SetY(BallConsidered.GetY() - (l*dy)/distance);
                        BallConsidered.SetZ(BallConsidered.GetZ() - (l*dz)/distance);
                        
    
    					Vector_Of_Balls[i].SetX(Vector_Of_Balls[i].GetX() + (l*dx)/distance);
                        Vector_Of_Balls[i].SetY(Vector_Of_Balls[i].GetY() + (l*dy)/distance);
                        Vector_Of_Balls[i].SetZ(Vector_Of_Balls[i].GetZ() + (l*dz)/distance);
    

    					BallConsidered.SetVelocity(vx1,vy1,vz1);
    					// BallConsidered.SetX(BallConsidered.GetX()+vx2*vx1/(vx2+vx1));
    					// BallConsidered.SetY(BallConsidered.GetY()+;
    					Vector_Of_Balls[i].SetVelocity(vx2,vy2,vz2);
    					// Vector_Of_Balls[i].SetX(Vector_Of_Balls[i].GetX()+vx2);
    					// Vector_Of_Balls[i].SetY(Vector_Of_Balls[i].GetY()+vy2);
    				}	
    			}
    		}
    		FinalBoard.SetVectorBalls(Vector_Of_Balls);
    
    		FinalBoard.SetBallFromId(ballid,BallConsidered);	

    		pthread_mutex_unlock (&UpdateLock);	
    		usleep(50000);
    	}
    }
}	

void *DisplayUpdate(void* id)
{
	Graph  *pa= (Graph *)id;
	// cout <<"Updating diaply\n";
	graphics(pa->x1,pa->s1);
	// usleep(10000);
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	const int NumberOfBalls = atoi(argv[1]);
	pthread_mutex_init(&UpdateLock,NULL);
	FinalBoard=Board(800.0,600.0,600.0,NumberOfBalls);
	
    pthread_t BallThreads [NumberOfBalls];
	pthread_t DisplayThread;
	
    cout <<"Starting\n";
    PauseBoard=false;
	cout<<FinalBoard.GetBoardInformation()<<"\n";

	Graph graphics1;
	graphics1.x1=argc;
	graphics1.s1=argv;

	std::cout <<"Ball threads" <<NumberOfBalls <<"\n";
	for (long i=0; i<NumberOfBalls ;i++)
	{
		cout <<"Creating thread : " <<i <<"\n";
		pthread_create(&BallThreads[i],NULL,UpdateBoardThread,(void *)i);
	
	}

	pthread_create(&DisplayThread,NULL,DisplayUpdate,&graphics1);
	
	for (int i=0; i<NumberOfBalls ;i++)
	{
		pthread_join(BallThreads[i],NULL);
	}

    pthread_join(DisplayThread,NULL);
	// create a display thread 
	// run the threads 
	pthread_exit(NULL);
	return 0;
}

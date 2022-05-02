/*********
   CTIS164 - Template Source Program
----------
STUDENT : Sajad Abdolvandi
SECTION : 1
HOMEWORK: 3
----------
PROBLEMS: NO PROBLEMS
----------
ADDITIONAL FEATURES:
 * Changing gradient background
 * Win and GameOver modes
 * Fading out text for game discription
 * You chan change fireRate of the canon shooter
 * If you get caught by the UFO the ball will be confiscated by FBI and moved slowly through inside of the UFO (ALL OBJECTS WILL BE DISAPPEARED AND THE BACKGROUND WILL CHNGE AND GAME OVER TEXT WILL APPEAR)
 * Every time you run the game there will be random stars in the background
 * Use of quadratic and sinusodial for some movements and speed
 * Random changing colors for UFO lights
 * DrawEllipse Funciton, ShootVert Function, CloseAnim Funciton
 And lots of other stuff that you will notice inside the game...
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 700
#define SIZE 500
#define STAR 1000
#define TIMER_PERIOD  1 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
typedef struct{
    double x,y;
} vert;
vert vertex[STAR];
typedef struct{
    double x,y;
    double angle;
} point_t;
int angle = 90;
int TrX= 0;
#define ORIGINX 0
#define ORIGINY -330
#define FIRERATE 4
int generalTimer = 0;
int UFOmove = 0;


typedef struct{
    int x,y;
    int angle;
    bool shot;
    int Tr;
    int translate;
} ShootObj_t;
int movingX,movingY;
ShootObj_t Shooter[SIZE];
int ShootCount = 0;
typedef struct{
    int r,g,b;
} myRGB;
myRGB RGB;
float A = 70,freq = 0.8,C = 0,B = -30,freq2 = -1,A2 = 70;
float f(float x) {
   return A2 * sin((freq * x + C) * D2R) + B;
}
float f2(float x) {
   return A * sin((freq2 * x + C) * D2R) + B;
}
typedef struct{
    int x, y;
    bool hit;
} circles;
circles targets[20];
float quadratic(float x){
    return A*powf((x+2), 2) + 10;
}
int fire_rate = 0;
float fadeOpac = 1;
int fireTimer = 0;
double rectUpY =330;
double rectDownY = -330;
int whichCanon;
bool ufoBool = false;
float circleMove = 0;
bool alienBool = false;
bool alienBool2 = false;
bool F1Bool  =false;
bool End = false;
bool WinBool = false;
bool exitBool = false;
bool ExitAnim = false;
/////////////////////////





void circle( int x, int y, float r )
{
#define PI 3.1415
   float angle ;
   glBegin( GL_POLYGON ) ;
   for ( int i = 0 ; i < 100 ; i++ )
   {
      angle = 2*PI*i/100;
      glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
   }
   glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
   float angle;

   glBegin(GL_LINE_LOOP);
   for (int i = 0; i < 100; i++)
   {
      angle = 2 * PI*i / 100;
      glVertex2f(x + r*cos(angle), y + r*sin(angle));
   }
   glEnd();
}

void print(int x, int y, const char *string, void *font )
{
   int len, i ;

   glRasterPos2f( x, y );
   len = (int) strlen( string );
   for ( i =0; i<len; i++ )
   {
      glutBitmapCharacter( font, string[i]);
   }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string , ...)
{
   va_list ap;
   va_start ( ap, string );
   char str[1024] ;
   vsprintf( str, string, ap ) ;
   va_end(ap) ;
   
   int len, i ;
   glRasterPos2f( x, y );
   len = (int) strlen( str );
   for ( i =0; i<len; i++ )
   {
      glutBitmapCharacter( font, str[i]);
   }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
   va_list ap;
   va_start(ap, string);
   char str[1024];
   vsprintf(str, string, ap);
   va_end(ap);
   glPushMatrix();
      glTranslatef(x, y, 0);
      glScalef(size, size, 1);
      
      int len, i;
      len = (int)strlen(str);
      for (i = 0; i<len; i++)
      {
         glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
      }
   glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
float RGBconvert(float color){
    return color/255;
}

void DrawStars()

{
    glColor4f(1, 1, 1,0.6);
    for (int i = 0; i<STAR; i++) {
        circle(vertex[i].x, vertex[i].y, 0.8);
    }
}

/*void VertFunc(int x, int y) {
  
    double xp = cos(angle*D2R) * (x-orX) - sin(angle*D2R);
   double yp= sin(angle*D2R) * (x-orY) + cos(angle*D2R);


   glVertex2d(xp+TrX, yp);
}
 */
void VertFunc(float x ,float y)
{
    float s,c,xnew,ynew;
    s= sin((angle-90)*D2R);
    c = cos((angle-90)*D2R);
    x -= ORIGINX;
    y -= ORIGINY;
    xnew = x * c - y * s;
    ynew = x * s + y * c;
    x = xnew + ORIGINX;
    y = ynew + ORIGINY;
    glVertex2d(x+TrX, y);
}

void CanonShooter(void){
    
    // Stand
    glColor3f(RGBconvert(139), RGBconvert(120), RGBconvert(109));
    glBegin(GL_POLYGON);
    glVertex2f(-40+TrX  ,-winHeight/2  );
    glVertex2f(0+TrX ,-winHeight/2+30 );
    glVertex2f(40+TrX ,-winHeight/2  );
    glEnd();
    // Circle
    glColor3f(RGBconvert(250), RGBconvert(240), RGBconvert(240));
    circle(0+TrX, -winHeight/2+20, 15);
    glColor3f(RGBconvert(0),RGBconvert(0), RGBconvert(6));
    circle(0+TrX, -winHeight/2+20, 11);
    // big outside quad
    glColor3f(RGBconvert(250), RGBconvert(240), RGBconvert(240));
    glBegin(GL_QUADS);
    VertFunc(-15,-winHeight/2+20);
    VertFunc(-8,-winHeight/2+20+30);
    VertFunc(8, -winHeight/2+20+30);
    VertFunc(15, -winHeight/2+20);
    glEnd();
     // inside quad
    glColor3f(RGBconvert(0),RGBconvert(0), RGBconvert(0));
    glBegin(GL_QUADS);
    VertFunc(-11 , -winHeight/2+20);
    VertFunc(-5, -winHeight/2+20+30);
    VertFunc(5, -winHeight/2+20+30);
    VertFunc(11, -winHeight/2+20);
    glEnd();
    // lines
    
    glColor3f(RGBconvert(250), RGBconvert(240), RGBconvert(240));

    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    VertFunc(0 , -winHeight/2+20);
    VertFunc(0, -winHeight/2+20+5);
    glEnd();
    glColor3f(RGBconvert(250), RGBconvert(240), RGBconvert(240));
 //
    glBegin(GL_LINE_STRIP);
    VertFunc(0, -winHeight/2+20+10);
    VertFunc(0, -winHeight/2+20+15);
    glEnd();
    glColor3f(RGBconvert(250), RGBconvert(240), RGBconvert(240));
//
    glBegin(GL_LINE_STRIP);
    VertFunc(0, -winHeight/2+20+20);
    VertFunc(0, -winHeight/2+20+25);
    glEnd();
    glBegin(GL_QUADS);
    VertFunc(-7, -winHeight/2+20+30);
    VertFunc(-7, -winHeight/2+20+30+3);
    VertFunc(7, -winHeight/2+20+30+3);
    VertFunc(7, -winHeight/2+20+30);

    glEnd();

    
}
void upBar(void){
    glColor3f(0, 0,0 );
    glRectd(-winWidth/2, winHeight/2-30, winWidth/2, winHeight/2);
    glColor3f(1,1, 1);
    vprint(-580,330, GLUT_BITMAP_HELVETICA_10, "LEFT & RIGHT: Chage the Starting point !");
    vprint(-340,330, GLUT_BITMAP_HELVETICA_10, "UP & DOWN: Chage the angle of \"Cannon Shooter\" !");
    vprint(-40,330, GLUT_BITMAP_HELVETICA_10, "SPACEBAR: Shoot from \"Cannon Shooter\" !");
    vprint(220,330, GLUT_BITMAP_HELVETICA_10, "F1: Pause the animation !");
    vprint(370,330, GLUT_BITMAP_HELVETICA_10, "F2: Reset game !");
    vprint(480,330, GLUT_BITMAP_HELVETICA_10, "ESC : End game !");
}
void Gradient(void){
  
    glBegin(GL_QUADS);
    glColor3f(0,0,0);

    glVertex2f(-winWidth/2, winHeight/2);
    glVertex2f(winWidth/2,winHeight/2);
    glColor3f(RGBconvert(80),RGBconvert(80),RGBconvert(80));

    glVertex2f(winWidth/2,-winHeight/2-200);
    glVertex2f(-winWidth/2,-winHeight/2-200);
    glEnd();
 
}
void WinGradient(){
  
    glBegin(GL_QUADS);
    glColor3f(0,0.7,0);

    glVertex2f(-winWidth/2, winHeight/2);
    glVertex2f(winWidth/2,winHeight/2);
    glColor3f(0.8,0.8,0.8);

    glVertex2f(winWidth/2,-winHeight/2-200);
    glVertex2f(-winWidth/2,-winHeight/2-200);
    glEnd();
 
}
void RedGradient(){
  
    glBegin(GL_QUADS);
    glColor3f(1,0,0);

    glVertex2f(-winWidth/2, winHeight/2);
    glVertex2f(winWidth/2,winHeight/2);
    glColor3f(0,0,0);

    glVertex2f(winWidth/2,-winHeight/2-200);
    glVertex2f(-winWidth/2,-winHeight/2-200);
    glEnd();
 
}

float ShootVertX(float x ,float y,int ang)
{
    float s,c,xnew,ynew;
    s= sin(((ang)*D2R));
    c = cos(((ang)*D2R));
    x -= ORIGINX;
    y -= ORIGINY;
    xnew = x * c - y * s;
    ynew = x * s + y * c;
    x = xnew + ORIGINX;
    y = ynew + ORIGINY;
    return x;
    
}
float ShootVertY(float x ,float y,int ang)
{
    float s,c,xnew,ynew;
    s= sin(((ang)*D2R));
    c = cos(((ang)*D2R));
    x -= ORIGINX;
    y -= ORIGINY;
    xnew = x * c - y * s;
    ynew = x * s + y * c;
    x = xnew + ORIGINX;
    y = ynew + ORIGINY;
    return y;
}

void TobeShotObj(){
    if (WinBool != true) {
        if (ufoBool != true) {
            glColor3f(1, 1, 1);
            glPointSize(15);
            for (int i = 1; i<=ShootCount; i++) {
                if (Shooter[i].shot == true) {
                    glBegin(GL_POINTS);
                    glVertex2f(Shooter[i].x, Shooter[i].y);
                    glEnd();

                }
               
            }
            glPointSize(1);

        }else{
            glColor3f(1, 1, 1);
            glPointSize(15);
            glBegin(GL_POINTS);
            glVertex2f(Shooter[whichCanon].x, Shooter[whichCanon].y);
            glEnd();
        }
    }
 

}



void DrawEllipse(float cx, float cy, float rx, float ry, int number)
{
    float ang = 2 * 3.1415926 / (number);
    float c = cosf(ang);
    float s = sinf(ang);
    float t;

    float x = 1;
    float y = 0;

    glBegin(GL_POLYGON);
    for(int i = 0; i < number; i++)
    {
        glVertex2f(x * rx + cx, y * ry + cy);
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    
    }
    glEnd();
}

void UFO(){
    //lines
      glColor4f(1, 1, 1,0.7);
      glBegin(GL_LINES);
      glVertex2f(-5+UFOmove, 6 +f(UFOmove));
      glVertex2f(-12+UFOmove, 13+f(UFOmove));
      glVertex2f(5+UFOmove, 6+f(UFOmove));
      glVertex2f(12+UFOmove, 13+f(UFOmove));
      glEnd();
    float c = 0.5 * sin(0.5 * generalTimer * D2R) + 0.5;
    
    // lights
    //glColor3f(0.3, 0.4, 0.8);
    glColor3f(0.4, 0.7, 0.63);
    circle(0+UFOmove, f(UFOmove), 10);
    //glColor3f(0.9, 0.9, 0.9);
    glColor3f(c, c*c, 1-c);
    circle(-15 +UFOmove, -6+f(UFOmove), 6.5);
    circle(15+UFOmove, -6+f(UFOmove), 6.5);
    circle(0+UFOmove, -8+f(0+UFOmove), 6.5);
    // tip
   
    // body
    //glColor3f(0.4, 0.7, 0.63);
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_POLYGON);
    glVertex2f(-15+UFOmove, 3+f(UFOmove));
    glVertex2f(15+UFOmove, 3+f(UFOmove));
    glVertex2f(25+UFOmove, -8+f(UFOmove));
    glVertex2f(-25+UFOmove, -8+f(UFOmove));
    glEnd();
    glColor4f(1, 1, 1,0.2);
    glBegin(GL_LINES);
    glVertex2f(-10+UFOmove, -8+f(UFOmove));
    glVertex2f(-25+UFOmove, -60+f(UFOmove));

    glVertex2f(10+UFOmove, -8+f(UFOmove));
    glVertex2f(25+UFOmove, -60+f(UFOmove));
    glEnd();
    glColor4f(1, 1, 1,0.1);

    glBegin(GL_POLYGON);
    glVertex2f(-10+UFOmove, -8+f(UFOmove));
    glVertex2f(10+UFOmove, -8+f(UFOmove));
    glVertex2f(25+UFOmove, -60+f(UFOmove));
    glVertex2f(-25+UFOmove, -60+f(UFOmove));
    glEnd();
    glColor4f(1, 1, 1,0.8);
    DrawEllipse(0+UFOmove, -60+f(0+UFOmove), 25, 10, 1000);
    
    glColor3f(0, 0, 0);
    
    vprint2(-12+UFOmove, -6+f(UFOmove), 0.06, "F B I");


    

}
void CloseAnim(){
    if (ExitAnim == true) {
        glColor3f(0, 0, 0);
        glRectd(-winWidth/2, winHeight/2, winWidth/2, rectUpY);
        glRectd(-winWidth/2, -winHeight/2, winWidth/2, rectDownY);
    }
}
void myCircle_filled(float x, float y, float r) {
    glBegin(GL_POLYGON);
    for (float angle = 0; angle < 360; angle += 10) {
        glVertex2f(r * cos(angle*D2R) + x, r * sin(angle*D2R) + y);
    }
    glEnd();
}

void pie_filled(float x, float y, float r, float start, float end) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r * cos(angle*D2R) + x, r * sin(angle*D2R) + y);
    }
    glVertex2f(r * cos(end*D2R) + x, r * sin(end*D2R) + y);
    glEnd();
}

void dispCircleTargets(){
    
    for (int i = 0; i<20; i++) {
        if (targets[i].hit == false) {
            glColor3f(1,1,0);
            pie_filled(targets[i].x,targets[i].y, 10, 30, 330);
            glColor3f(0, 0, 0);
            myCircle_filled(targets[i].x,targets[i].y+5, 2);
            }
        }
    }
void Alien(){
    int xp =A*cos(generalTimer*D2R*2);
    int xy  = A*sin(generalTimer*D2R*2);
    // ghost
    glColor3f(RGBconvert(139), RGBconvert(120), RGBconvert(109));
    DrawEllipse(-430+xp, -240+xy, 27,35,1000);

    glColor3f(RGBconvert(210), RGBconvert(210), RGBconvert(210));
    
    DrawEllipse(-430+xp, -240+xy, 22,30,1000);
   

    glColor3f(.1, .1, .1);
    DrawEllipse(-440+xp, -237+xy, 5, 9, 1000);
    DrawEllipse(-420+xp,  -237+xy, 5, 9, 1000);
  
}
void Alien2(){
    int xp =-A*cos(generalTimer*D2R*2);
    int xy  = A*sin(generalTimer*D2R*2);
    // ghost
    glColor3f(RGBconvert(139), RGBconvert(120), RGBconvert(109));
    DrawEllipse(430+xp, -240+xy, 27,35,1000);

    glColor3f(RGBconvert(210), RGBconvert(210), RGBconvert(210));
    
    DrawEllipse(430+xp, -240+xy, 22,30,1000);
   

    glColor3f(.1, .1, .1);
    DrawEllipse(440+xp, -237+xy, 5, 9, 1000);
    DrawEllipse(420+xp,  -237+xy, 5, 9, 1000);
  
}

float isHit(float x1,float y1,float x2,float y2){
    float xp = powf(x2-x1, 2);
    float yp = powf(y2-y1, 2);
    float res = sqrtf(xp+yp);
    return res;
}
void display() {
if(rectDownY >= 20)
    exit(0);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (ufoBool == false) {
        Gradient();

    }else
        RedGradient();
    
    if (WinBool == true) {
        WinGradient();
        vprint2(-240, 220, 0.7, "YOU WON!");
    }
    glPointSize(1.1);
    DrawStars();
 
    TobeShotObj();
    if (WinBool != true) {
        UFO();

    }

   
    if (WinBool == false && ufoBool == false) {
        CanonShooter();
        glColor4f(1, 1, 1, fadeOpac);
        vprint(-370, 270, GLUT_BITMAP_HELVETICA_18, "Kill all Aliens and Pacmans, if you kill them all and dont get caught by FBI UFO, you will win!");

    }
    if (ufoBool== true)
        vprint2(-270, 220, 0.7, "GAME OVER!");
    glPointSize(1);
    glColor3f(1, 1, 1);
    dispCircleTargets();
    if (alienBool == false) {
        Alien();

    }
    if (alienBool2 == false) {
        Alien2();

    }

    upBar();
    CloseAnim();
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
      exit(0);
    if (key == 32 && F1Bool == false && fire_rate == 0 ) {
        ShootCount++;
        Shooter[ShootCount].angle = angle;
        Shooter[ShootCount].translate = TrX;
        Shooter[ShootCount].shot = true;
        fire_rate  =FIRERATE;
    }
   // to refresh the window it calls display() function
}

void onKeyUp(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
      exit(0);
    
   // to refresh the window it calls display() function
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void ResetEveryThing(void){
    UFOmove = -700;
    generalTimer = 200;
    angle = 90;
    ShootCount = 0;
    for (int i = 0; i<20; i++) {
        targets[i].hit = false;
    }
    for (int i = 0; i<SIZE; i++) {
        Shooter[i].shot = false;
        Shooter[i].Tr = 0;
        Shooter[i].angle = 90;
    }
    alienBool = false;
    alienBool2 = false;
    WinBool = false;
    ufoBool = false;
    fadeOpac = 1;
    ExitAnim = 0;
    exitBool= 0;
}
void onSpecialKeyDown( int key, int x, int y )
{
   // Write your codes here.
   switch (key) {
   case GLUT_KEY_UP:
           up = true;
           if (angle>=2 && F1Bool == false) {
               angle-=8;

           }
           break;
   case GLUT_KEY_DOWN:
           down = true;
           if (angle<=178 && F1Bool == false) {
               angle+=8;

           }
           break;
   case GLUT_KEY_LEFT: left = true;
           if (TrX >= -winWidth/2 +50&& F1Bool == false) {
               TrX-= 10;
           }
           break;
   case GLUT_KEY_RIGHT: right = true;
           if (TrX <= winWidth/2 -50&& F1Bool == false) {
               TrX+= 10;

           }break;
   }
    if (key == GLUT_KEY_F2) {
        ResetEveryThing();
    }
    if (key == GLUT_KEY_F1) {
        F1Bool = !F1Bool;
    }

   // to refresh the window it calls display() function
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp( int key, int x, int y )
{
   // Write your codes here.
   switch (key) {
   case GLUT_KEY_UP: up = false; break;
   case GLUT_KEY_DOWN: down = false; break;
   case GLUT_KEY_LEFT: left = false; break;
   case GLUT_KEY_RIGHT: right = false; break;
   }

   // to refresh the window it calls display() function
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick( int button, int stat, int x, int y )
{
   // Write your codes here.


   
   // to refresh the window it calls display() function
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize( int w, int h )
{
   winWidth = w;
   winHeight = h;
   glViewport( 0, 0, w, h ) ;
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
   glMatrixMode( GL_MODELVIEW);
   glLoadIdentity();
   display(); // refresh window.
}

void onMoveDown( int x, int y ) {
   // Write your codes here.


   
   // to refresh the window it calls display() function
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove( int x, int y ) {
   // Write your codes here.


    movingX = x - winWidth / 2;
    movingY = winHeight / 2 - y;
   // to refresh the window it calls display() function
}
#if TIMER_ON == 1
void onTimer( int v ) {
    
   glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
   // Write your codes here.
    fadeOpac-= 0.003;
    fireTimer +=10;
    if (F1Bool == false) {
        generalTimer++;
        if (ufoBool == false) {
            UFOmove +=7;
            if (UFOmove>=650) {
                UFOmove = -650;
            }
             circleMove =  -500 * sin(generalTimer * D2R+180) + 100;


            for (int i = 0; i<SIZE; i++) {
                if (Shooter[i].shot == true) {
                    Shooter[i].Tr += 10;
                }
            }

        }
       
         

    }
    /*
    if (UFOmove < 560 && ufoBool == false){
        UFOmove +=10;
        
    }
    if (UFOmove>=560) {
        ufoBool = true;
    }
    if (ufoBool == true) {
        UFOmove -=10;

    }
    if (UFOmove<= -560) {
        ufoBool = false;
    }
     */
    for (int j = 0; j<20; j++) {
        if (targets[j].hit == false) {
            targets[j].x = j*30 + (-300)+circleMove;
            targets[j].y =200+f2(circleMove-300+(j*20));
        }
        for (int i = 0; i<=ShootCount; i++) {
            if (isHit(targets[j].x, targets[j].y, Shooter[i].x, Shooter[i].y) <= 13) {
                targets[j].hit = true;
                
            }
            if (isHit(A*cos(generalTimer*D2R*2)-430, A*sin(generalTimer*D2R*2)-240, Shooter[i].x, Shooter[i].y) <=30) {
                alienBool = true;
            }
            if (isHit(-A*cos(generalTimer*D2R*2)+430, A*sin(generalTimer*D2R*2)-240, Shooter[i].x, Shooter[i].y) <=30) {
                alienBool2 = true;
            }
            if (isHit(UFOmove,f(0+UFOmove)-60,  Shooter[i].x, Shooter[i].y) <= 30) {
                ufoBool = true;
                whichCanon = i;
                alienBool = true;
                alienBool2 = true;

                for (int i = 0; i<20; i++) {
                    targets[i].hit = true;
                    
                }
                
            }
        }
       
      
    }
  
    // move inside ufo
    exitBool = false;
        if (ufoBool == true  && Shooter[whichCanon].y <= f(UFOmove)-3) {
            if (generalTimer%5==0) {
                
            
                if ( Shooter[whichCanon].x > UFOmove) {
                    Shooter[whichCanon].x--;
                }else if(Shooter[whichCanon].x < UFOmove){
                    Shooter[whichCanon].x++;
                }
                Shooter[whichCanon].y ++;

            }
            exitBool = true;
        }
    
    if (exitBool == false && ufoBool == true) {
        ExitAnim = true;
        
    }
    if (ExitAnim == true && Shooter[whichCanon].y >=f(UFOmove)-3 && Shooter[whichCanon].x <= UFOmove) {
        rectUpY -= 10;
        rectDownY+=10;
    }
    
    // when hits inside the ufo other balls disappear
    if (ufoBool != true && WinBool != true) {
        for (int i = 0; i<=ShootCount; i++) {
            Shooter[i].x = ShootVertX(Shooter[i].Tr, ORIGINY, Shooter[i].angle) + Shooter[i].translate;
            Shooter[i].y = ShootVertY(Shooter[i].Tr, ORIGINY, Shooter[i].angle);
        }
    }
    // move stars up when finished
    if (ufoBool == true) {
        for (int i = 0; i<STAR; i++) {
            vertex[i].y++;
        }
    }
    
    // chek if all bals are hit and alien is hit
        End = false;
        for (int i = 0; i<20; i++) {
            if (targets[i].hit == false) {
                End = true;
            }
        }
  
    if (alienBool == true && End == false && ufoBool != true && alienBool2 == true) {
        WinBool = true;
        // move stars down when won
        if (WinBool == true) {
            for (int i = 0; i<STAR; i++) {
                vertex[i].y--;
            }
        }
        
    }
    if (fire_rate > 0) fire_rate--;

   // to refresh the window it calls display() function
   glutPostRedisplay() ; // display()

}
#endif

void Init() {
   
   // Smoothing shapes

    glEnable( GL_POINT_SMOOTH );
       glEnable( GL_BLEND );
       glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   
   
}

int main( int argc, char *argv[] ) {
    srand(time(NULL));

    for (int i = 0; i<STAR; i++) {
        vertex[i].x =(rand()% (600-(-600)+1)+(-600));
        vertex[i].y =(rand()% (400-(-10000)+1)+(-10000));
    }
    for (int i = 0; i<SIZE; i++) {
        Shooter[i].shot = false;
        Shooter[i].Tr = 0;
        Shooter[i].angle = 90;
        Shooter[i].x = -999999999;
    }
   glutInit(&argc, argv );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   //glutInitWindowPosition(100, 100);
   glutCreateWindow("Sajad Abdolvandi HomeWork 3 - SpaceWar");

   glutDisplayFunc(display);
   glutReshapeFunc(onResize);

   //
   // keyboard registration
   //
   glutKeyboardFunc(onKeyDown);
   glutSpecialFunc(onSpecialKeyDown);

   glutKeyboardUpFunc(onKeyUp);
   glutSpecialUpFunc(onSpecialKeyUp);

   //
   // mouse registration
   //
   glutMouseFunc(onClick);
   glutMotionFunc(onMoveDown);
   glutPassiveMotionFunc(onMove);

  
   #if  TIMER_ON == 1
   // timer event
   glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
   #endif

   Init();
   
   glutMainLoop();
}

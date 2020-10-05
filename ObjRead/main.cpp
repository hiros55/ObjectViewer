#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include "ObjView.h"

GLdouble vertexPlane[][3] = {
    {1.0,1.0,0.0},
    {-1.0,1.0,0.0},
    {-1.0,-1.0,0.0},
    {1.0,-1.0,0.0}
};


int facePlane[][4] = {
    {3,2,1,0},
};
int MouseRotate_x, MouseRotate_y, MouseRotate_z;
int MousePosition_x, MousePosition_y, MousePosition_z;
int Rotate_x, Rotate_y, Rotate_z;
int Position_x, Position_y, Position_z;
int LastMouse_x, LastMouse_y, LastMouse_z;
bool mouse_drag_l;
bool mouse_drag_r;
Obj obj;
struct Obj::ObjDate* objdate = NULL;


/*void idle(void) {
    glutPostRedisplay();
}*/

void updateview() {
   gluLookAt(0, 100 , 0, 0, 0, 0, 1.0, 0.0, 0.0);
}


void display(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,-Position_z);
    glRotatef(-Rotate_y,1,0,0);
    glRotatef(-Rotate_x,0,1,0);

    glColor3d(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    for (int j = 0; j < 4; j++) {
        glVertex3dv(vertexPlane[facePlane[0][j]]);
    }
    glEnd();

    objdate = obj.Load(FileName);
    obj.RenderObj(objdate);

    glutSwapBuffers();
    glFlush();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double)w / (double)h, 1.0, 500);
}



void mouse(int button, int state, int x, int y)
{
    mouse_drag_l = false;
    mouse_drag_r = false;
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (GLUT_UP) {
            mouse_drag_l = true;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (GLUT_UP) {
            mouse_drag_r = true;
        }
        break;
    default:
        break;
    }
    LastMouse_x = x;
    LastMouse_y = y;
}

void mousedrag(int x, int y) {
    if (mouse_drag_l) {
        Rotate_x -= x - LastMouse_x;
        if (Rotate_x < 0.0)
            Rotate_x += 360;
        else if (Rotate_x > 360)
            Rotate_x -= 360;

        Rotate_y -= y - LastMouse_y;
        if (Rotate_y < -90.0)
            Rotate_y = -90.0;
        else if (Rotate_y > 30)
            Rotate_y = 30;
    }

    if (mouse_drag_r) {
        Position_z -= (y - LastMouse_y) * 0.2;
        if (Position_z < 10) {
            Position_z = 10;
        }
    }
    LastMouse_x = x;
    LastMouse_y = y;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

    if (key == 'l') {
        char filename[256]  = "";

        OPENFILENAME	open_file;
        memset(&open_file, 0, sizeof(OPENFILENAME));
        open_file.lStructSize = sizeof(OPENFILENAME);
        open_file.lpstrFile = filename;
        open_file.nMaxFile = 256;
        open_file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

        // ファイルダイアログを表示
        BOOL  ret = GetOpenFileName(&open_file);

        if (ret) {
            objdate = obj.Load(filename);
        }
    }
}

void Init(void)
{
    glEnable(GL_COLOR_MATERIAL);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearColor(1, 1, 1, 0);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL obj viewer");
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutMotionFunc(mousedrag);
    glutKeyboardFunc(keyboard);
    Init();
    glutMainLoop();
    return 0;
}
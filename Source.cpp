#include<iostream>
#include<string>
#include <GL/glut.h>

using namespace std;

float home_colours[4][3] = {
    {0, 0, 1},  //blue
    {1, 1, 0},  //yellow
    {0, 1, 0},  //green
    {1, 0, 0}   //red
};

int home_path_vertices[4][6][2] = {
    { {420, 120}, {420, 360}, {480, 360}, {480, 60}, {360, 60}, {360, 120} },    //blue path
    { {840, 480}, {840, 360}, {780, 360}, {780, 420}, {540, 420},  {540, 480} },  //yellow path
    { {480, 780}, {480, 540}, {420, 540}, {420, 840},  {540, 840}, {540, 780} },  //green path
    { {60, 420}, {60, 540}, {120, 540}, {120, 480}, {360, 480}, {360, 420} }     //red path
};


bool roll = false;
string upper_string = "CLICK TO", lower_string = "ROLL DICE!";

void init() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 900, 0, 900);
    glMatrixMode(GL_MODELVIEW);
}

void draw_home_paths() {
    for (int i = 0; i < 4; ++i) {
        glColor3fv(home_colours[i]);
        glBegin(GL_POLYGON);
        for (int j = 0; j < 6; ++j) {
            glVertex2iv(home_path_vertices[i][j]);
        }
        glEnd();
    }
}

void draw_home_square(bool is_inner) {
    int val = (is_inner) ? 60 : 0;
    //cout << val << endl;
    glRectf(0 + val, 0 + val, 360 - val, 360 - val);

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0 + val, 0 + val);
    glVertex2i(360 - val, 0 + val);
    glVertex2i(360 - val, 360 - val);
    glVertex2i(0 + val, 360 - val);
    glEnd();
}

void draw_board() {
    //1: DRAW COLOURED PATHS
    draw_home_paths();

    //2: DRAW GRID LINES
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    for (int i = 1; i < 15; ++i) {
        //vertical lines
        glVertex2i(60 * i, 0);
        glVertex2i(60 * i, 900);
        //horizontal lines
        glVertex2i(0, 60 * i);
        glVertex2i(900, 60 * i);
    }
    glEnd();

    //3: DRAW HOME BOXES
    glColor3fv(home_colours[0]);
    draw_home_square(false);
    glPushMatrix();
    glTranslatef(540, 0, 0);
    glColor3fv(home_colours[1]);
    draw_home_square(false);
    glTranslatef(0, 540, 0);
    glColor3fv(home_colours[2]);
    draw_home_square(false);
    glTranslatef(-540, 0, 0);
    glColor3fv(home_colours[3]);
    draw_home_square(false);
    glPopMatrix();

    //4: DRAW INNER BOXES
    glColor3f(1, 1, 1);
    draw_home_square(true);
    glPushMatrix();
    glTranslatef(540, 0, 0);
    glColor3f(1, 1, 1);
    draw_home_square(true);
    glTranslatef(0, 540, 0);
    glColor3f(1, 1, 1);
    draw_home_square(true);
    glTranslatef(-540, 0, 0);
    glColor3f(1, 1, 1);
    draw_home_square(true);
    glPopMatrix();
}

void display_characters(const char* str, int x, int y) {
    glColor3f(1, 1, 1);
    int w = glutBitmapLength(GLUT_BITMAP_8_BY_13, (unsigned char*)str);

    glRasterPos2f(x - (float)w / 2, y);

    int len = strlen(str);
    for (int i = 0; i < len; ++i)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
}

void draw_player(int r, int x, int y, float colour[]) {
    glColor3fv(colour);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (float i = 0.0f; i <= 360; ++i) {
        glVertex2f(r * cos(3.14159 * i / 180.0) + x, r * sin(3.14159 * i / 180.0) + y);
    }
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_POINTS);
    for (float i = 0.0f; i <= 360; ++i) {
        glVertex2f(r * cos(3.14159 * i / 180.0) + x, r * sin(3.14159 * i / 180.0) + y);
    }
    glEnd();
}

class Player {

public:

    int colour, position, home_position, home_path_position;
    int x, y, home_x, home_y, init_x, init_y;

    Player(int ch, int x1, int y1, int home_pos) {
        colour = ch;
        x = init_x = x1;
        y = init_y = y1;
        position = -1;
        home_path_position = home_pos;
        home_position = (home_path_position + 1) % 52;

        if (colour == 0) {  //blue
            home_x = x + 120; home_y = y;
        }
        else if (colour == 1) {
            home_x = x; home_y = y + 120;
        }
        else if (colour == 2) { //green
            home_x = x - 120; home_y = y;
        }
        else if (colour == 3) {
            home_x = x; home_y = y - 120;
        }
    }
};


Player players[] = { Player(0, 270, 90, 51), Player(1, 810, 270, 38), Player(2, 630, 810, 25), Player(3, 90, 630, 12) };

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    draw_board();

    for (int i = 0; i < 4; ++i) {
            draw_player(20, players[i].x, players[i].y, home_colours[i]);
    }

    glColor3f(0, 0, 0);
    glRectf(360, 360, 540, 540);
    display_characters(upper_string.c_str(), 450, 460);
    display_characters(lower_string.c_str(), 450, 430);
 
    glutPostRedisplay();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x >= 360 && x <= 540 && y >= 360 && y <= 540) {
        roll = true;
        display();
    }

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 0);
    glutInitWindowSize(900, 900);
    glutCreateWindow("LUDO");
    srand(time(0));
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
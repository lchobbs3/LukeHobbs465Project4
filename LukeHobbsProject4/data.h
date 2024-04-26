
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#define PHONG 0.0001


struct Color {
    GLfloat channel[3];

    Color() {

    }

    Color(float x, float y, float z) {
        this->channel[0] = x;
        this->channel[1] = y;
        this->channel[2] = z;
    }

    const Color operator + (const Color& right) const
    {
        Color result;
        result.channel[0] = channel[0] + right.channel[0];
        result.channel[1] = channel[1] + right.channel[1];
        result.channel[2] = channel[2] + right.channel[2];
        return result;
    }

    void print() {
        cout << this->channel[0] << " " << this->channel[1] << " " << this->channel[2] << endl;
    }

};

struct Vertex {
    float x;
    float y;
    float z;

    Color c;

    float uni_x; //store the normalized value of this vector (for cos_theta and cross product use
    float uni_y;
    float uni_z;

    Vertex() {

    }

    /*Normalize the vertex's vector to make the length equal to 1 and store it in uni_x,y,z*/
    void normalize()
    {
        float length = sqrt(x * x + y * y + z * z);
        this->uni_x = x / length;
        this->uni_y = y / length;
        this->uni_z = z / length;
    }

    /*Reload dot product */
    // You have to implement it here
    const float operator * (const Vertex& right) const
    {
        float result = uni_x * right.uni_x + uni_y * right.uni_y + uni_z + right.uni_z;
        return result;
    }


    /*Reload - operator*/
    const Vertex operator - (const Vertex& right) const
    {
        Vertex result;
        result.x = x - right.x;
        result.y = y - right.y;
        result.z = z - right.z;
        return result;
    }


    /*Reload + operator*/
    const Vertex operator + (const Vertex& right) const
    {
        Vertex result;
        result.x = x + right.x;
        result.y = y + right.y;
        result.z = z + right.z;
        return result;
    }

    /*Reload * operator to times a number*/
    const Vertex operator * (const float right) const
    {
        Vertex result;
        result.x = x * right;
        result.y = y * right;
        result.z = z * right;

        return result;
    }

    /*For normalized vector dot product use*/
    float normDot(const Vertex& right) const
    {
        float result = uni_x * right.uni_x + uni_y * right.uni_y + uni_z * right.uni_z;
        return result;
    }

    /*The cross-product */
    Vertex cross(const Vertex& right)
    {
        Vertex result;
        // Calculate determinant
        result.x = y * right.z - z * right.y;
        result.y = -(x * right.z - z * right.x);
        result.z = x * right.y - y * right.x;

        //cout << result.x << " " << result.y << " " << result.z << endl;

        return result;
    }

};

struct Triangle {
    int v_idx[3];
    Color c[3];
};

/* Here you need to implement some operations for 4-by-1 vector*/
struct Vec {
    float elem[4];


};

/* Here you need to implement a 4-by-4 matrix */
struct Mat {
    float elem[4][4];


    /*Normalize the matrix*/
    void eye()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == j)
                    elem[i][j] = 1.0f;
                else
                    elem[i][j] = 0.0f;
            }
        }
    }

    /*This function is to reload the multipliation operator for matrix products*/
    const Mat operator * (const Mat& right) const
    {
        Mat result;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.elem[i][j] = 0;
                for (int k = 0; k < 4; k++)
                {
                    result.elem[i][j] += elem[i][k] * right.elem[k][j];
                }
            }
        }

        return result;
    }

    /*This function is to reload the multipliation operator for matrix times a 4-by-1 vector*/
    const Vec operator * (const Vec& vec) const
    {
        Vec result;

        for (int i = 0; i < 4; i++)
        {

            result.elem[i] = 0;
            for (int k = 0; k < 4; k++)
            {
                result.elem[i] += elem[i][k] * vec.elem[k];
            }

        }

        return result;
    }

    void printMat()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%.5f, ", elem[i][j]);
            }
            printf("\n");
        }
    }
};


vector<Vertex> vertice_list;
vector<Triangle> triangles_list;

/*Define the light and colors*/
Vertex light; // Cl
Color ambient_color; // Ca
Color bunny_color; // Cr
Color phong_color; // Cp (or Cs)

/*Define the viewpoint*/
Vertex view_pos;

/*Parameter control the rotation*/
float theta_x = .0f;
float theta_y = .0f;
float theta_z = .0f;

/*Parameter control the translation*/
float translate_x = .0f;
float translate_y = .0f;
float translate_z = .0f;


int mesh_only = 0; //control whether a mesh triangle or plane is drawn
int shading_type = 0; //0 - no shading; 1 - ambient + diffuse shading; 2 - ambient + diffuse + phong; 3 - static shading


/*For the mouse control use*/
/* Define data */
int mouse_down = 0; //Represent the left mouse key is clicked down
int change_mode = 0; //0 means rotation; 1 means translation
int current_x = 0, current_y = 0;


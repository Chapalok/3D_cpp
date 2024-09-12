#include "graphics.h"
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

class tck_of_view 
{
	public:
		double x, y, z;	
		tck_of_view()
		{
			x = 0; y = 0; z = 0;
		}
		tck_of_view(double a, double b, double c)
		{
			x = a; y = b; z = c;
		}
};

class vertex 
{
	public:
		tck_of_view world; 
		tck_of_view vidov; 
		vertex(){};
		void clvertex(tck_of_view p)
		{
			world = p;
		}
		~vertex(){};
		void View_Coord(tck_of_view mircoord) 
		{
			double roangle = mircoord.x, tetangle=mircoord.y, fiangle=mircoord.z;
			vidov.x = -world.x * sin(tetangle) + world.y * cos(tetangle); 
			vidov.y = -world.x * cos(fiangle) * cos(tetangle) - world.y * cos(fiangle) * sin(tetangle) + world.z * sin(fiangle); 
			vidov.z = -world.x * sin(fiangle) * cos(tetangle) - world.y * sin(fiangle) * sin(tetangle) - world.z * cos(fiangle) + roangle;
		}
}; 

class edge
{
	private:
		vertex tmpvertex, fervertex; 
	public:
		edge(){}; 
		void edgech (vertex sv, vertex fv)
		{
			tmpvertex = sv;
			fervertex = fv;
		}
		~edge(){};
		void drawedge(double d) 
		{
			d = d/2; 
			double x1 = (int)((d*tmpvertex.vidov.x)/tmpvertex.vidov.z); 
			double y1 = (int)((d*tmpvertex.vidov.y)/tmpvertex.vidov.z);
			double x2 = (int)((d*fervertex.vidov.x)/fervertex.vidov.z);
			double y2 = (int)((d*fervertex.vidov.y)/fervertex.vidov.z);
			line(x1+(getmaxx()/2), y1+(getmaxy()/2), x2+(getmaxx()/2), y2+(getmaxy()/2));
  		}
};

struct strk{
	public: 
		int a, b;
};

class drawingmtd
{
	private:
		vertex *verts; 
		edge *edges; 
		strk *strk1;
		tck_of_view viewpoint;
		bool proff = 0;
	public:
		int n, m;
		friend class edge;
		friend class vertex;
		void draw() 
		{
			for (int i=0; i<m; i++)
				edges[i].drawedge(viewpoint.x);
		}
		void drawing()
		{
			int c, a, b;
			if(!proff)
			{
				FILE *f; 
				f = fopen("kub.txt", "r");
  				if (f != NULL) 
				{
					fscanf(f, "%d", &n);  
    				verts = new vertex[n];
    				for (int i=0; i<n; i++)
					{
    					fscanf(f, "%d", &a);
    					fscanf(f, "%d", &b);
    					fscanf(f, "%d", &c);
    					tck_of_view p = tck_of_view(a*100, b*100, c*100);
						verts[i].clvertex(p);
						verts[i].View_Coord(viewpoint);
					}
					fscanf(f, "%d", &m);
					edges = new edge[m];
					strk1 = new strk[m];
					for (int i=0; i<m; i++)
					{
						fscanf(f, "%d", &a);
    					fscanf(f, "%d", &b);
    					strk1[i].a = a;
    					strk1[i].b = b;
    					edges[i].edgech (verts[strk1[i].a], verts[strk1[i].b]);
					}
					proff = 1;
				}
			}
			for (int i=0; i<n; i++)
				verts[i].View_Coord(viewpoint);
			for (int i=0; i<m; i++)
    			edges[i].edgech (verts[strk1[i].a], verts[strk1[i].b]);
		} 
		void setviewpoint(double roangle, double tetangle, double fiangle)
		{
			viewpoint.x = roangle;
			viewpoint.y = tetangle;
			viewpoint.z = fiangle;
		}
};

int main()
{
	double rott=200, tetangle=90, fiangle=120;
	int d;
	int gddriver = DETECT, gmode, errorcode;
	initgraph(&gddriver, &gmode, "");
	drawingmtd cube;
	cube.setviewpoint(rott, tetangle, fiangle);
	cube.drawing();	
	cube.draw();
	while (!kbhit())
	{
		cleardevice();
		setbkcolor(0);
		setcolor(13); 
		tetangle += ((3.14/27));
		fiangle += ((3.14/27));
		cube.setviewpoint(rott, tetangle, fiangle);
		cube.drawing();	
		cube.draw();
		delay(250);
	}
	getch();
	closegraph();
	return 0;
}



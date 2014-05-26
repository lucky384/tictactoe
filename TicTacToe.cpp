#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <gl/glut.h>
using namespace std;

GLfloat mat2_specular[] = { 0.0, 0.0, 0.5, 1.0 };
GLfloat mat2_shininess[] = { 5.0 };
GLfloat mat3_specular[] = { 0.5, 0.0, 0.0, 1.0 };
GLfloat mat3_shininess[] = { 5.0 };

//All the possible lines
int lines[8][3][2] = {
	{ { 0,0 }, { 0,1 }, { 0,2 } },
	{ { 1,0 }, { 1,1 }, { 1,2 } },
	{ { 2,0 }, { 2,1 }, { 2,2 } },
	{ { 0,0 }, { 1,0 }, { 2,0 } },
	{ { 0,1 }, { 1,1 }, { 2,1 } },
	{ { 0,2 }, { 1,2 }, { 2,2 } },
	{ { 0,0 }, { 1,1 }, { 2,2 } },
	{ { 2,0 }, { 1,1 }, { 0,2 } },
};


//Enumerated Variable
enum
{
	USER=0,COMP=1,START=2,OVER=3
}turn=USER;

//Enumerated data type to store the data
//of each of the cell of the mark
enum Mark
{
	UNMARKED=0,
	X=1,
	O=2
};

//Class Representing the mark
class Maze
{
private:
	Mark mark[3][3];
	int angle[3][3];

	float anim_z,anim_ang;


public:
	char * message;

	Maze()
	{
		for(int i = 0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				mark[i][j]=UNMARKED;
				angle[i][j]=0;
			}
		}

		anim_ang=90;
		anim_z=-15;

		turn=START;

		message="";
	}
	void render()
	{
		GLfloat mat_specular[] = { 0.0, 0.5, 0.0, 1.0 };
		GLfloat mat_shininess[] = { 50.0 };

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		glPushMatrix();

		glTranslatef(0,0,anim_z);
		glRotatef(anim_ang,1,1,0);

		glPushMatrix();
			glTranslatef(-0.5,-1.5,0.0);
			glRotatef(-90,1.0,0.0,0.0);
			glutSolidCone(0.1f,3.0f,5,5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.5,-1.5,0.0);
			glRotatef(-90,1.0,0.0,0.0);
			glutSolidCone(0.1f,3.0f,5,5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5,0.5,0.0);
			glRotatef(-90,0.0,1.0,0.0);
			glutSolidCone(0.1f,3.0f,5,5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5,-0.5,0.0);
			glRotatef(-90,0.0,1.0,0.0);
			glutSolidCone(0.1f,3.0f,5,5);
		glPopMatrix();
		for(int i =0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				if(angle[i][j]>0)
					angle[i][j]-=5;

				switch(mark[i][j])
				{
				case X:
					glPushMatrix();
					glTranslatef(-1+i,1-j,0.0);
					glMaterialfv(GL_FRONT, GL_SPECULAR, mat2_specular);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat2_specular);
					glMaterialfv(GL_FRONT, GL_SHININESS, mat2_shininess);

					glPushMatrix();
					glRotatef(angle[i][j],1,1,1);
					glPushMatrix();
                    glTranslatef(-0.35,0.35,0.0);

					glRotatef(45,0,0,1);
					glRotatef(90,1,0,0);
					glutSolidCone(0.1f,1.0f,5,5);
					glPopMatrix();
					glPushMatrix();

					glTranslatef(0.35,0.35,0.0);
					glRotatef(-45,0,0,1);
					glRotatef(90,1,0,0);
					glutSolidCone(0.1f,1.0f,5,5);
					glPopMatrix();

					glPopMatrix();
					glPopMatrix();
					break;
				case O:
					glPushMatrix();
					glTranslatef(-1+i,1-j,0.0);
					glMaterialfv(GL_FRONT, GL_SPECULAR, mat3_specular);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat3_specular);
					glMaterialfv(GL_FRONT, GL_SHININESS, mat3_shininess);

					glPushMatrix();
					glRotatef(angle[i][j],1,0,1);
					glutSolidTorus(0.1,0.3,30,30);
					glPopMatrix();
					glPopMatrix();
				}
			}
		}
		glPopMatrix();


		glPushMatrix();
		glRasterPos3f(-0.1,0,3.0);
		glColor3f(1.0f,1.0f,1.0f);
		for (char * c=message; *c != '\0'; c++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}
		glPopMatrix();
	}

	int putMark(Mark mrk,int x,int y)
	{
		if(x<0||x>2||y<0||y>2)
			return 0;
		if(mark[x][y]!=UNMARKED)
			return 0;
		mark[x][y]=mrk;
		angle[x][y]=90;
		return 1;
	}

	//Think And Put
	int putMark(Mark mrk)
	{
		int x,y;
		int i,j;
		int count;

		//If Single Mark -> Victory then Mark
		for(i =0; i < 8;i++)
		{
			int count = 0, flag = -1;
			for( j=0; j<3; j++)
			{
				if (mark[lines[i][j][0]][lines[i][j][1]]==mrk)
					count++;
				if (mark[lines[i][j][0]][lines[i][j][1]]==UNMARKED)
					flag=j;
			}
			if ( count==2 && flag !=-1)
			{
				putMark(mrk,lines[i][flag][0],lines[i][flag][1]);
				return 1;
			}
		}

		//If Single Mark prevents loss then mark
		for(i=0;i<8;i++)
		{
			int c1=0,c2=0,flag=0;
			for(j=0;j<3;j++)
			{
				if(mark[lines[i][j][0]][lines[i][j][1]]==mrk)
					c1++;
				if(mark[lines[i][j][0]][lines[i][j][1]]==3-mrk)
					c2++;
				if(mark[lines[i][j][0]][lines[i][j][1]]==UNMARKED)
					flag=j;
			}
			if ( c2==2 && c1==0 && flag !=-1)
			{
				putMark(mrk,lines[i][flag][0],lines[i][flag][1]);
				return 1;
			}
		}

		//Defence Strategy - 1 *****
		//For each unmarked cell
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(mark[i][j]!=UNMARKED)
					continue;

				//Check if Marking will result in check-mate
				int c2=0;
				mark[i][j]=mrk;
				for(int k=0;k<8;k++)
				{
					int c1=0,c3=0;
					for(int l=0;l<3;l++)
					{
						if(mark[lines[k][l][0]][lines[k][l][1]]==mrk)
							c1++;
						if(mark[lines[k][l][0]][lines[k][l][1]]==UNMARKED)
							c3++;
					}
					if(c1==2&&c3==1)
						c2++;
				}
				mark[i][j]=UNMARKED;
				if(c2>1)
				{
					putMark(mrk,i,j);
					return 1;
				}

				//Check if Opponent's marking at this point
				//wil lead to check-mate
				mark[i][j]=(Mark)(3-mrk);
				for(int k=0;k<8;k++)
				{
					int c1=0,c3=0;
					for(int l=0;l<3;l++)
					{
						if(mark[lines[k][l][0]][lines[k][l][1]]==3-mrk)
							c1++;
						if(mark[lines[k][l][0]][lines[k][l][1]]==UNMARKED)
							c3++;
					}
					if(c1==2&&c3==1)
						c2++;
				}
				mark[i][j]=UNMARKED;
				if(c2>1)
				{
					putMark(mrk,i,j);
					return 1;
				}
			}
		}

		count=0;
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(mark[i][j]==UNMARKED)
					count++;
			}
		}

		//Defence Strategy - 2
		//If opponent marked in corner, mark in center
		if(count==8)
		{
			if(mark[0][0]==3-mrk||mark[0][2]==3-mrk||mark[2][2]==3-mrk||mark[2][0]==3-mrk)
			{
				putMark(mrk,1,1);
				return 1;
			}
		}

		//If in a line, 1 opposite mark is present,other 2
		//cells are empty, then block opponent
		for(i=0;i<8;i++)
		{
			int count = 0, empty =0,flag = -1;
			for(j=0;j<3;j++)
			{
				if(mark[lines[i][j][0]][lines[i][j][1]]==3-mrk)
					count++;
				if(mark[lines[i][j][0]][lines[i][j][1]]==UNMARKED)
				{
					empty++;
					flag=j;
				}
			}
			if(count==1&&empty==2)
			{
				putMark(mrk,lines[i][flag][0],lines[i][flag][1]);
				return 1;
			}
		}

		//NO ATTACK NO DEFENCE
		//PUT A RANDOM MARK
		do
		{
			x = rand()%3;
			y = rand()%3;
		}while(putMark(mrk,x,y)==0);

		return 1;
	}
	int checkDraw()
	{
		int i,j;
		for(i =0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(mark[i][j]==UNMARKED)
					goto out;
			}
		}
out:
		if(i==3&&j==3)
		{
			turn=OVER;
			return 1;
		}
		return 0;
	}

	int checkWin(Mark mrk)
	{
		for(int i = 0;i<8;i++)
		{
			int j;
			for(j=0;j<3;j++)
			{
				if(mark[lines[i][j][0]][lines[i][j][1]]!=mrk)
				{
					break;
				}
			}
			if(j==3)
			{
				for(j=0;j<3;j++)
				{
					angle[lines[i][j][0]][lines[i][j][1]]=360;
				}
				turn=OVER;
				return 1;
			}
		}
		return 0;
	}
	int InitAnim()
	{
		if(anim_z<0)
		{
			anim_ang-=1.5;
			anim_z+=0.25;
			return 1;
		}
		return 0;
	}
	void ExitAnim()
	{
		int count=0;
		for(int i =0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				if(angle[i][j]==0)
					count++;
			}
		}
		if(count==9)
		{
			anim_ang-=3;
			anim_z+=0.1;
		}
	}
}maze;

void initializeGraphics(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Tic Tac Toe");

	glClearColor(0.9,1.0,0.9,0);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	float position[4]= {5,5,5,0};
	glLightfv(GL_LIGHT0,GL_POSITION,position);

}

void render(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(turn==START)
  {
	  if(!maze.InitAnim())
		  turn=USER;
  }
  if(turn==COMP)
  {
	  maze.putMark(O);
	  turn=USER;
	  if(maze.checkWin(O))
		  maze.message="YOU LOSE";
	  else
	  {
		  if(maze.checkDraw())
			maze.message="DRAW !";
	  }

  }
  if(turn==OVER)
  {
	  maze.ExitAnim();
  }

  maze.render();
  glutSwapBuffers();
  glFlush();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,4.0,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
}

void mouse(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&turn==USER)
	{
		int a=-1,b=-1;
		if(x>25&&x<173)
			a=0;
		if(x>173&&x<325)
			a=1;
		if(x>325&&x<482)
			a=2;
		if(y>25&&y<173)
			b=0;
		if(y>173&&y<325)
			b=1;
		if(y>325&&y<482)
			b=2;
		if(a!=-1&&b!=-2)
		{
			maze.putMark(X,a,b);
			turn=COMP;
			if(maze.checkWin(X))
				maze.message="YOU WIN";
			else
			{
				if(maze.checkDraw())
					maze.message="DRAW !";
			}

		}
	}
}


void callback(void)
{
	glutReshapeFunc(changeSize);
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutMouseFunc(mouse);
}

int main(int argc,char **argv)
{
	srand(unsigned(time(NULL)));

	initializeGraphics(argc,argv);

	callback();

	glutMainLoop();
}

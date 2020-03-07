/*
 *	Pour compiler sous Linux:
 *
 *		g++ -o intGL Affiche.cc -lglut -lGLU -lGL
 *
 *	Pour compiler sous MacOSX
 *
 *		g++ -o intGL Affiche.cc -Wno-deprecated-declarations -framework GLUT -framework OpenGL
 */

#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// pour current_time
#include <sys/time.h>

// les deux pour 'mkdir'.
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*
 *	Classe utilitaire: Vector
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vector
{
	public:
		inline Vector() : x(0), y(0), z(0), w(1) {};  
		inline Vector(const float nx, const float ny, const float nz) : x(nx), y(ny), z(nz), w(1) {};
		inline Vector(const Vector &v) : x(v.x), y(v.y), z(v.z), w(1) {};  

		inline Vector operator+(const Vector &v) const
		{     
			return Vector(x+v.x, y+v.y, z+v.z);
		}

		inline Vector operator-(const Vector &v) const
		{
			return Vector(x-v.x, y-v.y, z-v.z);
		}

		//dot product
		inline float operator*(const Vector &v) const
		{
			return x*v.x + y*v.y + z*v.z;
		}

		//cross product   (this cross v)
		inline Vector operator^(const Vector &v) const
		{
			return Vector( (y*v.z-z*v.y), (z*v.x-x*v.z), (x*v.y-y*v.x) );
		}

		inline Vector operator+( const float f ) const
		{
			return Vector(x+f, y+f, z+f);
		}

		inline Vector operator-( const float f ) const
		{
			return Vector(x-f, y-f, z-f);
		}

		inline Vector operator-() const
		{
			return Vector(-x, -y, -z);
		}

		inline Vector operator*(const float f) const
		{
			return Vector(x*f, y*f, z*f);
		}

		inline Vector operator/(const float f) const
		{
			float inv = 1.0f/f;
			return Vector(x*inv, y*inv, z*inv);
		}

		inline Vector& operator=(const Vector &v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		inline Vector& operator+=(const Vector &v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		inline Vector& operator-=(const Vector &v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		inline bool operator!=(const Vector &v)
		{
			return !(x == v.x && y == v.y && z == v.z);
		}

		inline bool operator==(const Vector& v)
		{
			return x == v.x && y == v.y && z == v.z;
		}

		inline Vector& operator*=(const float f)
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		inline Vector& operator/=(const float f)
		{
			float inv = 1.0f / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}


		inline float length() const
		{
			return sqrtf(*this * *this);
		}

		inline Vector normalized() const
		{
			float	len = length ();
			return len == 0. ? *this : *this / len;
		}

		float x;
		float y;
		float z;
		float w;
};

inline void normalize(Vector& v)
{
	v /= v.length();
};

#endif


// dimensions de la fenêtre.
static int	window_width = 1024;
static int	window_height = 768;

#define RAYON_DEF	50

// le rayon de la vésicule.

static int		rayon = RAYON_DEF;

// fonte d'affichage.
static int		fontheight = 20;	// hauteur.
static int		fontascent = 20;	// et ligne de base de la fonte d'affichage.

#define BASE	53


// les display lists.
static GLuint	axeList = 0;
static GLuint	helpList = 0;

// état initial de l'observateur.
#define	DEF_PX		0.
#define DEF_PY		0.
#define DEF_PZ		3600.	// 80.
#define DEF_SITE	0
#define DEF_AZIMUTH	0
#define DEF_ORBITAL	0
#define DEF_APERTURE	3 // 90

// position et direction de l'observateur.

static int	lastx = 0;
static int	lasty = 0;
static int	aperture = DEF_APERTURE;
static int	site = DEF_SITE;
static int	orbital = DEF_ORBITAL;
static int	azimuth = DEF_AZIMUTH;

static int*		currenty = 0;
static int*		currentx = 0;

static float	bg_r = 0;
static float	bg_g = 0;
static float	bg_b = 0;

#define TNG_WIDTH	130.

static float	tng_width = TNG_WIDTH;

// callbacks.
static void 	spec_keyboard_up (int key, int x, int y);
static void 	spec_keyboard (int key, int x, int y);
static void 	reshape (int l, int h);
static void 	keyboard (unsigned char key, int x, int y);
static void 	mouse (int button, int state, int x, int y);
static void 	mouse_motion (int x, int y);
static void 	timer (int);
static void		display ();

static bool		showAxes = true;
static bool		showBox = false;
static bool		showMembrane = true;

// fonctions utilitaires.
static void 	make_sphere_tex ();
static void		make_scenery ();

// quelques variables.

static GLuint	Sphere_tex;
static GLuint	Disc_tex;
static char		fps_string [64];

static bool		diffusion_only = false;

// centre de rotation.
static Vector	center, icenter;
// l'observateur.
static Vector	obs (DEF_PX, DEF_PY, DEF_PZ), iobs;
// molécule actuellement sélectionnée.
static Vector	selection (0., 0., 0.);

/*
 *	Espèce moléculaire.
 */

class EspeceMol {
public:
	int	_r, _g, _b;		// couleurs.
	int	_diameter;		// diamètre.
	int	_num_type;		// type.
};

/*
 *	Une molécule
 */

class Molecule {
public:	
	Molecule*	_next;		// pointeur vers la molécule suicante.
	EspeceMol*	_type;		// le type de molécule.
	float		_x, _y, _z;	// coordonnées réelles de la molécule.
};

static EspeceMol
	Esp_1 = {
		200, 200, 100,
		4,
		1
	},
	Esp_2 = {
		30, 200, 80,
		7,
		2
	};

static Molecule
	m1 = {
		0,
		&Esp_1,
		10., 10., 10.
	},
	m2 = {
		&m1,
		&Esp_1,
		12., 0., 20.
	},
	m3 = {
		&m2,
		&Esp_2,
		-12., -10., -20.
	};

static Molecule*	les_molecules = &m3;	// tête de liste des molécules.

/*
 *	Établit le point de vue initial de l'observateur.
 */

static void init_observer (void) {
	lastx = 0;
	lasty = 0;
	aperture = DEF_APERTURE;
	site = DEF_SITE;
	orbital = DEF_ORBITAL;
	azimuth = DEF_AZIMUTH;
	currenty = 0;
	currentx = 0;
	center.x = 0; center.y = 0; center.z = 0;
	selection.x = 0; selection.y = 0; selection.z = 0;
	// calcule la distance de l'observateur en fonction de la taille du compartiment principal.
	obs.x = DEF_PX; obs.y = DEF_PY;
	obs.z = (28. * rayon) + 1000.;
}

/*
 *	Calcule un nombre de secondes depuis ~2001
 */

inline static int get_current_time (void)
{
	struct timeval	tv;

	(void) gettimeofday (&tv, 0);
	// retirer 10^9 secondes, soit ~31 ans...
	return ((tv.tv_sec-1000000000) * 1000000) + tv.tv_usec;
}

/*
 *	Représenter les molécules par de belles sphères éclairées
 *	sous forme de texture plaqués sur un carré (qui sera toujours
 *	affiché de face).
 */

static void make_sphere_tex ()
{
	int	i, j;
#define DIAMETER 128
#define	RADIUS (DIAMETER / 2)
	GLubyte	sphereImage [DIAMETER][DIAMETER][2];

	for (i = 0; i < DIAMETER; i++) {
		for (j = 0; j < DIAMETER; j++) {
			int	dist = (RADIUS-i)*(RADIUS-i) + (RADIUS-j)*(RADIUS-j);
			int	light = (int) (4. * sqrt ((float)(RADIUS*RADIUS - dist))) - 1;

			sphereImage[i][j][0] = (GLubyte) light;
			sphereImage[i][j][1] = (GLubyte) (dist < (RADIUS-2)*(RADIUS-2)) ? 255 : 0;
		}
	}
	glAlphaFunc (GL_GREATER, 0);	// pour masquer les bords des carrés.
	glEnable (GL_ALPHA_TEST);		// et n'afficher que le cercle au centre.

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glGenTextures (1, &Sphere_tex);
	glBindTexture (GL_TEXTURE_2D, Sphere_tex);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gluBuild2DMipmaps (GL_TEXTURE_2D, GL_LUMINANCE_ALPHA,
					DIAMETER, DIAMETER,
					GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, sphereImage);

	// presque la même, mais juste pour avoir un disque.
	for (i = 0; i < DIAMETER; i++) {
		for (j = 0; j < DIAMETER; j++) {
			int	dist = (RADIUS-i)*(RADIUS-i) + (RADIUS-j)*(RADIUS-j);

			sphereImage[i][j][0] = (GLubyte) 255;
			sphereImage[i][j][1] = (GLubyte) (dist < (RADIUS-2)*(RADIUS-2)) ? 255 : 0;
		}
	}
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glGenTextures (1, &Disc_tex);
	glBindTexture (GL_TEXTURE_2D, Disc_tex);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gluBuild2DMipmaps (GL_TEXTURE_2D, GL_LUMINANCE_ALPHA,
					DIAMETER, DIAMETER,
					GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, sphereImage);
}

static void quad_draw (Molecule* m)
{
	EspeceMol*	v = m -> _type;
	Vector	s (m -> _x, m -> _y, m -> _z);
	float	size = v -> _diameter > .3 ? v -> _diameter / 2. : v -> _diameter;

	// afficher la molécule.
	glColor3ub (v -> _r, v -> _g, v -> _b);
	glPushMatrix ();

	glTranslatef (s.x, s.y, s.z);
	glRotatef (-azimuth, 0, 0, 1);
	glRotatef (-orbital, 1, 0, 0);
	glRotatef (-site, 0, 1, 0);
	glBegin (GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f (0-size, 0-size, 0);
		glTexCoord2f(1.0, 0.0); glVertex3f (0+size, 0-size, 0);
		glTexCoord2f(1.0, 1.0); glVertex3f (0+size, 0+size, 0);
		glTexCoord2f(0.0, 1.0); glVertex3f (0-size, 0+size, 0);
	glEnd ();
	glPopMatrix ();
}

/*
 *	Affiche les molécules
 */

static void display_molecules ()
{
	Molecule*	p;

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, Sphere_tex);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	for (p = les_molecules; p != 0; p = p -> _next)
		quad_draw (p);
	glDisable(GL_TEXTURE_2D);
}

/*
 *	Affiche une membrane.
 */

static void draw_membrane (float ln, float th, float x, float y, float z, float red)
{
	GLdouble	clip [] = { -1., 0., 0., 0. };
	GLUquadricObj*	quad = gluNewQuadric ();

	glEnable (GL_CULL_FACE);
	glCullFace (GL_FRONT);		// n'afficher que la face interne de la membrane.

#define TAU 0.25

	glColor4f (red, .45, .6, 0.15);
	glPushMatrix ();
	// afficher le pôle -X
	glTranslatef (x-(ln+TAU), y, z);
	glEnable (GL_CLIP_PLANE0);
	glClipPlane (GL_CLIP_PLANE0, clip);
	glRotatef (90., 0., 1., 0.);
	gluSphere (quad, (th+2*TAU), 32, 32);
	glRotatef (-90., 0., 1., 0.);
	// afficher le pôle +X
	glTranslatef (2*(ln+TAU), 0., 0.);
	clip [0] = 1.;
	glClipPlane (GL_CLIP_PLANE0, clip);
	glRotatef (90., 0., 1., 0.);
	gluSphere (quad, (th+2*TAU), 32, 32);
	glDisable (GL_CLIP_PLANE0);
	// afficher le cylindre médian.
	glTranslatef (0., 0., -2*(ln+TAU));
	gluCylinder (quad, th+2*TAU, th+2*TAU, 2*(ln+TAU), 64, 32);
	gluDeleteQuadric (quad);
	glDisable (GL_CULL_FACE);
	glPopMatrix ();
}


/*
 *	Crée une GlList avec le décor.
 */

static void make_scenery ()
{
	glNewList (axeList, GL_COMPILE);
	// dessiner les axes.
	if (showAxes) {
		glLineWidth (1.);

		glEnable (GL_LINE_SMOOTH);
		glDepthMask (GL_FALSE);
		glBegin (GL_LINES);
		glColor3ub (180, 180, 0);
		glVertex3f (-rayon-10., 0., 0.);
		glVertex3f (rayon+10., 0., 0.);

		glVertex3f (0, -rayon-4, 0);
		glVertex3f (0, rayon+4, 0);

		glVertex3f (0, 0, -rayon-4);
		glVertex3f (0, 0, rayon+4);
		glEnd ();
		glDisable (GL_LINE_SMOOTH);
		glDepthMask (GL_TRUE);
	}
	// dessiner la membrane.
	if (showMembrane)
		draw_membrane (0, rayon, 0, 0, 0, 0.4);
	glEndList ();
}

/*
 *	Fait 'vivre' l'automate (glutIdleFunc).
 */

static int	time_started;

static bool stopped = true;

static void diff_react (void)
{
#if 0
	// ne prendre ni trop grand (pour laisser du temps aux interactions)
	// ni trop petit (pour ne pas ramer)
	int	gen_incr = 50;

	// appeler la fonction de calcul de gen_incr pas de temps.
	compute (gen_incr);
#endif
	// réafficher.
	if (diffusion_only)
		glutPostRedisplay ();
}

static void menu (int m)
{
	switch (m) {
		case 0: showAxes = false; break;
		case 1: showAxes = true; break;
		case 2: showMembrane = false; break;
		case 3: showMembrane = true; break;
		case 4: showBox = false; break;
		case 5: showBox = true; break;
		default:
			return;
	}
	make_scenery ();
	glutPostRedisplay ();
}

/*
 *	Traite les clicks des boutons de la souris.
 */

static int	obsx, obsz;

static void mouse (int button, int state, int x, int y)
{
	Vector	s;

	if (state == GLUT_DOWN) {
		int mod = glutGetModifiers ();
		lastx = x;
		lasty = y;
		switch (button) {
			case GLUT_LEFT_BUTTON:
				// sélection ou rotation.
				switch (mod) {
					// déplacement à la souris.
					case GLUT_ACTIVE_CTRL:
						currentx = &obsx;
						currenty = &obsz;
						return;

					// sélectionner le pivot de rotation.
					case 0:
						currentx = &site;
						currenty = &azimuth;
						return;
				}

			case GLUT_MIDDLE_BUTTON:
				currentx = 0;
				currenty = 0;
				return;

			case GLUT_RIGHT_BUTTON:
				currentx = 0;
				currenty = &aperture;
				return;

			case 3:
				orbital = (orbital+360-6) % 360;
				glutPostRedisplay ();
				return;

			case 4:
				orbital = (orbital+6) % 360;
				glutPostRedisplay ();
				return;
		}
	}
}

void mouse_motion (int x, int y)
{
	int deltax = x-lastx;
	int deltay = y-lasty;

	lastx = x;
	lasty = y;

	if (currentx != 0)	 (*currentx) += deltax;
	if (currenty != 0)	 (*currenty) += deltay;

	if (currenty == &aperture) {
		if (aperture < 1)
			aperture = 1;
		else if (aperture > 175)
			aperture = 175;
	}
	else if (currenty == &azimuth)
		azimuth = (azimuth + 360) % 360;

	if (currentx == &site)
		site = (site + 360) % 360;

	if (currentx == &obsx) {
		obs.x -= (float) deltax;
		obs.z += (float) deltay * 10.;
	}

	if (currentx != 0 || currenty != 0)
		glutPostRedisplay ();
}

const int	HELP_Y = 40;
const int	BORDER = 220;
const int	HWIDTH = 600;
const int	HHEIGHT = 620;

/*
 *	Construit les points intersectant un quart d'ellipse.
 */

static void ellipse (float x1, float y1, float x2, float y2, bool clockwise)
{
	float	a = fabs (x2-x1);
	float	b = fabs (y1-y2);
	int		n = 20;
	float	x;
	float	xincr = a/n;

	if (clockwise) {
		for (x = -a; --n >= 0; x += xincr)
			glVertex2f (x + x2, y1 - b * sqrt (1 - (x*x) / (a*a)));
		if (x != 0.)
			glVertex2f (x2, y1 - b);
	} else {
		for (x = 0.; --n >= 0; x -= xincr)
			glVertex2f (x + x2, y1 - b * sqrt (1 - (x*x) / (a*a)));
		if (x != -a)
			glVertex2f (x1, y1);
	}
}

/*
 *	Dessine un coin supérieur gauche style Star Trek TNG.
 */

static void draw_ul_corner (float x1, float y1, float width,
							float x2, float y2, float height)
{
	glBegin (GL_POLYGON);
	ellipse (x1, y1, x2, y2, true);
	ellipse (x1+width, y1, x2, y2+height, false);
	glEnd ();
	// le bord antialiasé.
	glBegin (GL_LINE_STRIP);
	ellipse (x1, y1, x2, y2, true);
	ellipse (x1+width, y1, x2, y2+height, false);
	glVertex2f (x1, y1);
	glEnd ();
}

/*
 *	Dessine un rectangle.
 */

static void draw_rectangle (float x, float y, float width, float height)
{
	glBegin (GL_POLYGON);
	glVertex3f (x, y, 0.);
	glVertex3f (x+width, y, 0.);
	glVertex3f (x+width, y+height, 0.);
	glVertex3f (x, y+height, 0.);
	glEnd ();
	// le bord antialiasé.
	glBegin (GL_LINE_STRIP);
	glVertex3f (x, y, 0.);
	glVertex3f (x+width, y, 0.);
	glVertex3f (x+width, y+height, 0.);
	glVertex3f (x, y+height, 0.);
	glVertex3f (x, y, 0.);
	glEnd ();
}

#define num_x	(tng_width-40.)
#define tng_x	(tng_width+30.)

static const int	fps_width = 100;
static const int	time_width = 100;
static const int	mode_width = 100;

static void draw_scenery ()
{
	int	rh = fontheight+2;

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0, window_width, window_height, 0);
	glViewport (0, 0, window_width, window_height);

	glEnable (GL_LINE_SMOOTH);
	glColor3ub (80, 60, 100);
	draw_ul_corner (3., 50., tng_width, tng_x, 3., rh);
	// les rectangles du haut.
	draw_rectangle (tng_x+3., 3., mode_width, rh);
	draw_rectangle (tng_x+3.+mode_width+3., 3., window_width - (tng_x+3.+mode_width+3.) - time_width - fps_width, rh);
	draw_rectangle (window_width - time_width - fps_width + 3., 3., time_width-6., rh);
	draw_rectangle (window_width - fps_width, 3., fps_width-3, rh);
	// les rectangles de gauche.
	int	voffset = 2*fontheight+3;

	draw_rectangle (3., BASE, tng_width, voffset);
	glDisable (GL_LINE_SMOOTH);
}

/*
 *	Place l'observateur et le monde.
 */

static inline void set_observer (float x =0.)
{
	float	a = (float) site;

	if (x < 0)
		a -= atan (- x / obs.z);
	else if (x > 0)
		a += atan (x / obs.z);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	// placer l'observateur.
	gluLookAt (obs.x+x, obs.y, obs.z,	obs.x, obs.y, 0,	0, 1, 0);
	// tourner autour du pivot.
	glRotated (a, 0, 1, 0);
	glRotated (orbital, 1, 0, 0);
	glRotatef (azimuth, 0, 0, 1);
	// se centrer autour de la base pivot.
	glTranslatef (-center.x, -center.y, -center.z);
}

/*
 *	Établit la projection en 2D.
 */

static inline void set_projection (void)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (aperture, (double)window_width / (double)window_height, 10., 100000.);
	glViewport (0, 0, window_width, window_height);
}

// fonction de réaffichage.

static void display ()
{
	static int	tt = 0;
	static int	nb_frames = 0;
	static int	fps = 0;

	if (tt == 0) {
		tt = get_current_time ();
		nb_frames = 0;
	}
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	set_projection ();
	// placer l'observateur.
	set_observer ();
	// afficher les molécules.
	display_molecules ();
	glCallList (axeList);
	draw_scenery ();
	glutSwapBuffers ();
	// une image de plus.
	++nb_frames;
	if (get_current_time () - tt >= 1000000) {
		tt = 0;
		fps = nb_frames;
		nb_frames = 0;
	}
	// afficher le nombre d'images par secondes.
	fprintf (stderr, "\r%d fps", fps);
}

static bool	move_up = false;
static bool	move_down = false;
static bool	move_forward = false;
static bool	move_backward = false;
static bool	step_right = false;
static bool	step_left = false;

static void reshape(int width, int height)
{
	window_width = width;
	window_height = height;
}

static int	key_count = 0;
static void spec_keyboard_up (int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_PAGE_UP:		move_forward = false; break;
		case GLUT_KEY_PAGE_DOWN:	move_backward = false; break;
		case GLUT_KEY_UP:
			if (glutGetModifiers () == GLUT_ACTIVE_SHIFT)
				move_forward = false;
			else
				move_up = false;
			break;
		case GLUT_KEY_DOWN:
			if (glutGetModifiers () == GLUT_ACTIVE_SHIFT)
				move_backward = false;
			else
				move_down = false;
			break;
		case GLUT_KEY_LEFT:			step_left = false; break;
		case GLUT_KEY_RIGHT:		step_right = false; break;
	}
	if (--key_count == 0)
		glutSetKeyRepeat (GLUT_KEY_REPEAT_ON);
}

static void spec_keyboard (int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_PAGE_UP:		move_forward = true; break;
		case GLUT_KEY_PAGE_DOWN:	move_backward = true; break;
		case GLUT_KEY_UP:
			if (glutGetModifiers () == GLUT_ACTIVE_SHIFT)
				move_forward = true;
			else
				move_up = true;
			break;
		case GLUT_KEY_DOWN:
			if (glutGetModifiers () == GLUT_ACTIVE_SHIFT)
				move_backward = true;
			else
				move_down = true;
			break;
		case GLUT_KEY_LEFT:			step_left = true; break;
		case GLUT_KEY_RIGHT:		step_right = true; break;
	}
	if (key_count++ == 0)
		glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
}

static void timer (int)
{
	static float	accel_std = 0.0;
	static float	accel_fast = 0.0;
	static float	accel;
	// les états de l'automate.
	static enum {
		MOVE_LEFT = -1, STANDARD, MOVE_RIGHT,
		FAST_LEFT, FAST_RIGHT,
		FORWARD, BACKWARD, FAST_FORWARD, FAST_BACKWARD,
		UP, DOWN, FAST_UP, FAST_DOWN
	} state = STANDARD;
	// fixer le facteur d'accélération en fonction du frame-rate.
	if (accel_std == 0.) {
		accel_std = 1.0;
		accel_fast = 4. * accel_std;
		accel = accel_std;
	}
#define TIMER_INTERVAL 25
	glutTimerFunc (TIMER_INTERVAL, timer, 0);
	// faire bouger l'observateur.
	switch (state) {
		case STANDARD:
			accel = accel_std;
			if (step_right) {
				state = MOVE_RIGHT;
				return;
			}
			if (step_left) {
				state = MOVE_LEFT;
				return;
			}
			if (move_forward) {
				state = FORWARD;
				return;
			}
			if (move_backward) {
				state = BACKWARD;
				return;
			}
			if (move_up) {
				state = UP;
				return;
			}
			if (move_down) {
				state = DOWN;
				return;
			}
			return;

		case FAST_RIGHT:
			if (!step_left) {
				state = MOVE_RIGHT;
				accel = accel_std;
			} else
				accel = accel_fast;
			// fall through.
		case MOVE_RIGHT:		// décalé à droite.
			if (!step_right) {
				state = STANDARD;
				return;
			}
			if (step_left)
				state = FAST_RIGHT;

			obs.x += accel;
			glutPostRedisplay ();
			return;

		case FAST_LEFT:
			if (!step_right) {
				state = MOVE_LEFT;
				accel = accel_std;
			} else
				accel = accel_fast;
			// fall through.
		case MOVE_LEFT:	// décalé à gauche.
			if (!step_left) {
				state = STANDARD;
				return;
			}
			if (step_right)
				state = FAST_LEFT;

			obs.x -= accel;
			glutPostRedisplay ();
			return;

		case FAST_FORWARD:
			if (!move_backward) {
				state = FORWARD;
				accel = accel_std;
			} else
				accel = accel_fast;
			// fall through.
		case FORWARD:
			if (!move_forward) {
				state = STANDARD;
				return;
			}
			if (move_backward)
				state = FAST_FORWARD;

			if (obs.z > 8*accel)
				obs.z -= 8*accel;
			glutPostRedisplay ();
			return;

		case FAST_BACKWARD:
			if (!move_forward) {
				state = BACKWARD;
				accel = accel_std;
			} else
				accel = accel_fast;
			// fall through.
		case BACKWARD:
			if (!move_backward) {
				state = STANDARD;
				return;
			}
			if (move_forward)
				state = FAST_BACKWARD;

			obs.z += 8*accel;
			glutPostRedisplay ();
			return;

		case FAST_UP:
			if (!move_down) {
				state = UP;
				accel = accel_std;
			} else
				accel = accel_fast;
			// fall through.
		case UP:
			if (!move_up) {
				state = STANDARD;
				return;
			}
			if (move_down)
				state = FAST_UP;

			obs.y += accel;
			glutPostRedisplay ();
			return;

		case FAST_DOWN:
			if (!move_up) {
				state = DOWN;
				accel = accel_std;
			} else
				accel = accel_fast;
			// fall through.
		case DOWN:
			if (!move_down) {
				state = STANDARD;
				return;
			}
			if (move_up)
				state = FAST_DOWN;

			obs.y -= accel;
			glutPostRedisplay ();
			return;
		}
}

/*
 *	Traite les événements du clavier.
 */

static void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case '\t':
			stopped = !stopped;
			if (stopped) {
				glutIdleFunc (0);
			} else {
				time_started = get_current_time ();
				glutIdleFunc (diff_react);
			}
			break;

		case 'q':
		case 033:
			::exit (0);
			break;

		case 'd':
			diffusion_only = ! diffusion_only;
			glutPostRedisplay ();
			break;

		case '\b':		// rétablit les conditions initiales.
			init_observer ();
			glutPostRedisplay ();
			break;
	}
}

int main (int argc, char** argv)
{
	int				mask = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;

	glutInit (&argc, argv);
	glutInitDisplayMode (mask);
	glutInitWindowSize (window_width, window_height);
	(void) glutCreateWindow ("Mol-Simulator");
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	glutSpecialUpFunc (spec_keyboard_up);
	glutSpecialFunc (spec_keyboard);
	glutMouseFunc (mouse);
	glutMotionFunc (mouse_motion);	

	glClearColor (bg_r, bg_g, bg_b, 1.);
	glEnable (GL_DEPTH_TEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glPointSize (6.);
	// les display lists.
	axeList = glGenLists (2);
	helpList = glGenLists (3);
	// faire les display lists.
	make_scenery ();
	// menu popup attaché au bouton 2.
	glutCreateMenu (menu);
	glutAddMenuEntry ("Hide axes", 0);
	glutAddMenuEntry ("Show axes", 1);
	glutAddMenuEntry ("Hide membrane", 2);
	glutAddMenuEntry ("Show membrane", 3);
	glutAttachMenu (GLUT_MIDDLE_BUTTON);

	glutTimerFunc (25, timer, 0);
	// la texture de sphère.
	make_sphere_tex ();
	init_observer ();
	glutMainLoop ();
	return 0;
}

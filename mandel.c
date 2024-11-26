/// 
// Amos(Mohan) Li
// CPE 2600-121
// 11/25/2024
// Lab 10-multi-proccessing
// File: mandel.c
// Modified by: Amos(Mohan) Li
// Compile Command: make
// Main change: add semaphore and multi-proccessing function to generate multiple
// images at the same time to the directory images
// 
// Based on example code found here:
// https://users.cs.fiu.edu/~cpoellab/teaching/cop4610_fall22/project3.html
//
// Converted to use jpg instead of BMP and other minor changes
//  
///
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "jpegrw.h"

// local routfile_templateines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max );
static void show_help();


int main( int argc, char *argv[] ) {
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.
	const char *outfile_template = "images/mandel%d.jpg";
	double xcenter = 0;
	double ycenter = 0;
	double xscale = 4;
	int    image_width = 1000;
	int    image_height = 1000;
	int    max = 1000;
	int num_process = 1;
	int total_frame = 50;

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"x:y:s:W:H:m:o:h:n:"))!=-1) {
		switch(c) 
		{
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				xscale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 'o':
				outfile_template = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
			case 'n':
				num_process = atoi(optarg);
				break;
		}
	}

	// Add semaphore initialization at the top of your `main` function
	sem_t *sem = sem_open("/mandel_sem", O_CREAT, 0644, num_process);
	if (sem == SEM_FAILED) {
    	perror("sem_open failed");
    	exit(EXIT_FAILURE);
	}


	for (int frame = 0; frame < total_frame; frame++) {
		sem_wait(sem); // Decrease the semaphore count
		pid_t pid = fork();
		if (pid == 0) {
			char outfile[256];
			snprintf(outfile, sizeof(outfile), outfile_template, frame);
			//change the scale
			double frame_xscale = xscale * (1 - 0.015 * frame);
			double frame_xcenter = xcenter + 0.02 * frame;
			double frame_ycenter = ycenter - 0.02 * frame; 
			// Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
			double frame_yscale = frame_xscale / image_width * image_height;

			// Display the configuration of the new image.
			printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d outfile_template=%s\n",frame_xcenter,frame_ycenter,frame_xscale,frame_yscale,max,outfile_template);

			// Create a new raw image of the appropriate size.
			imgRawImage* img = initRawImage(image_width,image_height);

			// Fill it with a black
			setImageCOLOR(img,0);

			// Compute the Mandelbrot image
			compute_image(img,frame_xcenter-frame_xscale/2,frame_xcenter+frame_xscale/2,frame_ycenter-frame_yscale/2,frame_ycenter+frame_yscale/2,max);

			// Save the image in the stated file.
			storeJpegImageFile(img,outfile);

			// free the mallocs
			freeRawImage(img);
			sem_post(sem); // Release the semaphore before exiting
			exit(0);
		} else if (pid < 0) {
			perror("fork failed");
			sem_post(sem); // Release the semaphore before exiting
			exit(1);
		}	
	}

	// Parent frame: wait for all child framees to complete
	for (int frame = 0; frame < total_frame; frame++) {
		wait(NULL);
	}
	sem_close(sem);
	sem_unlink("/mandel_sem");
	return 0;
}




/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max ) {
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max ) {
	int i,j;

	int width = img->width;
	int height = img->height;

	// For every pixel in the image...

	for(j=0;j<height;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			setPixelCOLOR(img,i,j,iteration_to_color(iters,max));
		}
	}
}


/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max ) {
	int color = 0xFFFFFF*iters/(double)max;
	return color;
}


// Show help message
void show_help() {
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-n <number of framees>	Number of frame to use. (default = 1)\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the image in pixels. (default=1000)\n");
	printf("-o <file>   Set outfile_templateput file. (default=mandel.bmp)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}

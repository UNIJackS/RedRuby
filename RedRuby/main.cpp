/*
 * The code below is meant to detect whether the Red Ruby
 * is present. The starategy implemented by the code is not very effective.
 * Study the code so that you understand what the strategy is and how
 * it is implemented. Then design and implement a better startegy.
 *
 * */




#include <iostream>
#include "E101.h"

using namespace std;

//0.7... is the lowest i saw but 0.6 gives wiggle room
const double threshhold_value = 0.5;

//number of pixels bounds are alowed to fluctuate from inital
const int tollarance = 10;

const int num_rows = 240;
const int num_col = 320;

const int total_num_pixels = num_rows * num_col;

struct boundInfo {
	int top;
	int bottom;
	int left;
	int right;
	int redPixelNum;
}


bool isRed(int y, int x) {
	int red = (int)get_pixel(x, y, 0);
	int intesity = (int)get_pixel(x, y, 3);;

	double redness = (double)red / (3.0 * (double)intesity);

	if (redness > threshhold_value) {
		return(true);
	}
	else {
		return(false);
	}

	cout << " error in isRed function" << endl;
	return(false);
}


bool betterIsRed(int y, int x) {

}

boundInfo findBounds() {
	boundInfo output;

	output.redPixelNum = 0;
	bool firstRedRow = true;

	//This loops through all the pixels in the image
	for (int row = 0; row < num_rows; row += 1) {
		for (int col = 0; col < num_col; col += 1) {
			if (isRed(col, row)) {

				//This checks for the first row with a red pixel
				if (firstRedRow) {
					output.top = row;
					firstRedRow = false;
				}

				//This reccords the last row for the bottom of the bound
				output.bottom = row;

				//This reccords the left most collum 
				if (col < output.left) {
					output.left = col;
				}

				//This reccords the right most collum
				if (col > output.right) {
					output.right = col;
				}

				output.redPixelNum += 1;
			}
		}
	}

	//sets the values to be the outline of the screen 
	//if there are no red pixesl found
	if (output.redPixelNum = 0) {
		output.left = 0;
		output.right = num_col;
		output.top = 0;
		output.bottom = num_rows;
	}

	return output;
}


bool notStolenCheck(boundInfo intial, boundInfo current) {
	if (current.top > inital.top + tollarance) {
		cout << "top bound too far up" << endl;
		return false;
	}
	else if (current.bottom < inital.bottom - tollarance) {
		cout << "bottom bound too far down" << endl;
		return false;
	}
	else if (current.left < intial.left - tollarance) {
		cout << "Left bound too far left" << endl;
		return false;
	}
	else if (current.right > inital.right + tollarance) {
		cout << "Right bound too far right" << endl;
		return false;
	}
	else if (current.redPixelNum > inital.redPixelNum + 300) {
		cout << "number of red pixels went above tollarance" << endl;
		return false;
	}
	else if (current.redPixelNum < inital.redPixelNum - 300) {
		cout << "number of red pixels went bellow tollarance" << endl;
		return false;
	}
	else if (current.redPixelNum == 0) {
		cout << "no red pixels detected" << endl;
		return false;
	}
	else {
		return true;
	}

}

void drawBounds(boundInfo inital, boundInfo detected) {


	//This code draws a green outline for the bounds of the detected red ruby
	int detectedRows = detected.bottom - detected.top;
	int detectedCols = detected.right - detected.left;
	//This draws the left and right lines
	for (int row = detected.top; row < detectedRows; row += 1) {
		char green = 255;
		char red = 0;
		char blue = 0;
		set_pixel(row, detected.left, red, green, blue);
		set_pixel(row, detected.right, red, green, blue);
	}
	//This draws the top and bottom lines
	for (int col = detected.left; col < detectedCols; col += 1) {
		char green = 255;
		char red = 0;
		char blue = 0;
		set_pixel(detected.top, col, red, green, blue);
		set_pixel(detected.bottom, col, red, green, blue);

	}

	//This code draws a red outline for the tollerance zone
	int initalRows = inital.bottom - inital.top + 2 * tollarance;
	int initalCols = inital.right - inital.left + 2 * tollarance;
	//This draws the left and right lines
	for (int row = inital.top - tollarance; row < initalRows; row += 1) {
		char green = 255;
		char red = 0;
		char blue = 0;
		set_pixel(row, detected.left - tollarance, red, green, blue);
		set_pixel(row, detected.right + tollarance, red, green, blue);
	}
	//This draws the top and bottom lines
	for (int col = inital.left - tollarance; col < detectedCols; col += 1) {
		char green = 255;
		char red = 0;
		char blue = 0;
		set_pixel(inital.top - tollarance, col, red, green, blue);
		set_pixel(inital.bottom + tollarance, col, red, green, blue);

	}




}


int main() {
	int err = init(0);
	cout << "Error: " << err << endl;
	open_screen_stream();

	take_picture();
	update_screen();

	boundInfo initalImage = findBounds();

	notStolen = true;
	runNum = 0;

	while (notStolen) {
		cout << "ruby checked";

		take_picture();
		update_screen();


		boundInfo currentImage = findBounds();


		//checks against the intial iamge to seee if red ruby still there
		notStolen = notStolenCheck(initalImage, currentImage);

		//draws the bounding boxs to the screen
		drawBounds(initalImage, currentImage)

			sleep1(100); // slow down a bit to make display easier
	}

	cout << "RED RUBY STOLEN" << endl;

	close_screen_stream();
	return 0;
}

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


//number of pixels bounds are alowed to fluctuate from inital
const int padding = 25;
const float redTolarance = 1.7;-
const int sizeTolarance = 500;
const int minRedPixels = 2000;

const int num_rows = 240;
const int num_col = 320;

const int total_num_pixels = num_rows * num_col;

struct boundInfo {
	int top;
	int bottom;
	int left;
	int right;
	int redPixelNum;
};

bool isRed(int y, int x) {
	int red = (int)get_pixel(x, y, 0);
	int blue = (int)get_pixel(x, y, 1);
	int green = (int)get_pixel(x, y, 2);


	if (red > redTolarance * blue && red > redTolarance * green) {
		set_pixel(x, y, 0, 255, 0);
		return(true);
	}
	return false;
}


boundInfo findBounds() {
	boundInfo output;

	output.redPixelNum = 0;
	output.left = num_col;
	output.right = 0;
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
	if (0 == output.redPixelNum) {
		output.left = 0;
		output.right = num_col;
		output.top = 0;
		output.bottom = num_rows;
	}

	return output;
}


bool notStolenCheck(boundInfo inital, boundInfo current) {
	if (current.top > inital.top + padding) {
		cout << "top bound too far up" << endl;
		return false;
	}
	else if (current.bottom < inital.bottom - padding) {
		cout << "bottom bound too far down" << endl;
		return false;
	}
	else if (current.left < inital.left - padding) {
		cout << "Left bound too far left" << endl;
		return false;
	}
	else if (current.right > inital.right + padding) {
		cout << "Right bound too far right" << endl;
		return false;
	}
	else if (current.redPixelNum > inital.redPixelNum + sizeTolarance) {
		cout << "number of red pixels went above tollarance" << endl;
		return false;
	}
	else if (current.redPixelNum < inital.redPixelNum - sizeTolarance) {
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

boundInfo initalCheck() {
	bool invalidImage = true;
	while (invalidImage) {
		take_picture();
		boundInfo initalImage = findBounds();

		bool test = false;
		if (initalImage.redPixelNum > minRedPixels) {
			cout << "ruby present";
			test = true;

			if (initalImage.left - padding < 0) {
				cout << "but too close to left edge" << endl;
				test = false;
			}
			if (initalImage.right + padding > num_col) {
				cout << "but too close to right edge" << endl;
				test = false;
			}
			if (initalImage.top - padding < 0) {
				cout << "but too close to top edge" << endl;
				test = false;
			}
			if (initalImage.bottom + padding > num_row) {
				cout << "but too close to bottom edge" << endl;
				test = false;
			}
		}
		else {
			cout << "ruby not present"
		}

		invalidImage = test;

		string test = "";
		cout << "Please move ruby away from edge or into view and then press enter"
			cin >> test;
	}

}

void drawBounds(boundInfo input, int padding) {
	//This draws the green left and right lines
	for (int currentRow = 0; currentRow < (input.bottom - input.top) + 2 * padding; currentRow += 1) {
		set_pixel(input.top + currentRow - padding, input.left - padding, 0, 255, 0);
		set_pixel(input.top + currentRow - padding, input.right + padding, 0, 255, 0);
	}
	//This draws the blue top and bottom lines
	for (int currentCol = 0; currentCol < (input.right - input.left) + 2 * padding; currentCol += 1) {
		set_pixel(input.top - padding, input.left + currentCol - padding, 0, 0, 255);
		set_pixel(input.bottom + padding, input.left + currentCol - padding, 0, 0, 255);

	}


}


int main() {
	int err = init(0);
	cout << "Error: " << err << endl;
	open_screen_stream();

	boundInfo initalImage = initalCheck();

	bool notStolen = true;
	while (notStolen) {
		take_picture();

		boundInfo currentImage = findBounds();

		//draws the bounding boxs to the screen
		drawBounds(currentImage, 0);
		drawBounds(initalImage, padding);
		update_screen();


		//checks against the intial iamge to seee if red ruby still there
		notStolen = notStolenCheck(initalImage, currentImage);

		sleep1(100); // slow down a bit to make display easier
	}

	cout << "RED RUBY STOLEN" << endl;

	close_screen_stream();
	return 0;
}

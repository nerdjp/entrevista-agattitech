#include <iostream>
#include <cmath>
#include <vector>
#include <SDL2/SDL.h>

//Struct with coordenates to return from the search
struct Point {
	float x;
	float y;
};

//Helper function to print stuff;
void print(float a, std::string b = "") {
	std::cout << b << a << std::endl;
}

// Fibonacci...
long fibonacci(int x) {
	if(x == 0 || x == 1) {
		return 1;
	}
	return fibonacci(x - 1) + fibonacci(x - 2);
}

//Function to be minimized
float f(float x) {
	float x2 = std::pow(x, 2);
	return std::sqrt(((std::pow(2.71828, 4 * x2)) + std::pow(std::sin(x), 2)) / ((3 * x2) + std::pow(x, 4)));
}

//I renamed some variables in the original assinement
//to help me understand the algorythm
//I -> lenght / segmentLenght
//x -> left
//y -> right
//u -> segment2 (the point on the right side of the line)
//v -> segment1 (the point on the left side of the line)
//a -> test1
Point fibonnachiLineSearch(float x, float y, float n) {

	//Calculating the initial ratio:
	long Fn = fibonacci(n);
	long Fn1 = fibonacci(n - 1);

	//Calculating the the initial lenght to search
	//And the size of the initial search segments
	float initialLenght = y - x;
	float segmentLenght = ((float)Fn1 / Fn) * initialLenght;

	//Variables that mark the beggining and end of the current total search segment;
	float left = x;
	float right = y;
	
	//Variables that mark the current points to be tested on the X axis
	float segment2 = left + segmentLenght;
	float segment1 = right - segmentLenght;

	//Variables that mark the result of the point tests, on the Y axis
	float test1 = f(segment2);
	float test2 = f(segment1);

	//Printing the variables as required
	std::cout << "========" << std::endl;
	print(0, "Iteration n: ");
	std::cout << std::endl;
	print(Fn, "Fn: ");
	print(Fn1, "Fn1: ");
	std::cout << std::endl;
	print(segmentLenght, "Segment Lenght: ");
	print(left, "Left: ");
	print(segment1, "Segment 1: ");
	print(test1, "Test 1: ");
	print(segment2, "Segment 2: ");
	print(test2, "Test 2: ");
	print(right, "Right: ");
	std::cout << "========" << std::endl;
	std::cout << std::endl;


	//Loop through and test the points, until the tolerance is hit
	for (int k = 1; k != n - 2; k++) {

		//Insted of recalculating the fibonnaci through recursion,
		//the smallest is subtrcted from the largest, finding the next value down
		long temp = Fn;
		Fn = Fn1;
		Fn1 = temp - Fn1;

		//Finding the new segment size
		segmentLenght = ((float)Fn1 / Fn) * segmentLenght;

		//Verifing which test is closer to the minimium
		//Moving the outermost points inwards
		//Then, recalculating the segment point of
		//the closest segment point
		if(test1 <= test2) {
			left = segment1;
			segment1 = segment2;
			segment2 = left + segmentLenght;
			test2 = test1;
			test1 = f(segment2);
		} else {
			right = segment2;
			segment2 = segment1;
			segment1 = right - segmentLenght;
			test1 = test2;
			test2 = f(segment1);
		}

		//Printing the variables as required
		std::cout << "========" << std::endl;
		print(k, "Iteration n: ");
		std::cout << std::endl;
		print(Fn, "Fn: ");
		print(Fn1, "Fn1: ");
		std::cout << std::endl;
		print(segmentLenght, "Segment Lenght: ");
		print(left, "Left: ");
		print(segment1, "Segment 1: ");
		print(test1, "Test 1: ");
		print(segment2, "Segment 2: ");
		print(test2, "Test 2: ");
		print(right, "Right: ");
		std::cout << "========" << std::endl;
		std::cout << std::endl;
		
		if(segment2 < segment1) {
			break;
		}
	}

	//The result is the median between the two closest points found within tolerance
	float result = ((left + right) / 2);
	
	//Return a struct with both X and Y for the minimum point
	return {result, f(result)};
}

std::vector<Point> calculateGrapth(float start, float end) {
	std::vector<Point> points;
	for(float i = start; i <= end; i += 0.01) {
		float x = i;
		float y = f(x);
		Point p;
		points.push_back({x, y});
	}
	return points;
}

int main() {
	//Initializing SDL structs
	SDL_Window* window = SDL_CreateWindow("Fibonnaci Line Search", 0, 0, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	//Scale and offset for SDL2 rendering
	int scale = 400;
	int vOffset = 1000;
	int pOffsetX = 0;
	int pOffsetY = 0;

	//Parameters for the Fibonnaci Search
	float searchRangeStart = 0.3;
	float searchRangeEnd = 0.8;
	float tolerance = 8;
	
	// Fill a array with some points for drawing the graph
	std::vector<Point> points = calculateGrapth(searchRangeStart, searchRangeEnd);

	//Store actual points and visual points apart so that recalculation
	//is not needed upon movement / scale rendering
	std::vector<SDL_Point> sdlPoints;

	// Finding the minimum point on the graph (the actual algorythim)
	Point point = fibonnachiLineSearch(searchRangeStart, searchRangeEnd, tolerance);

	// Printing the point coordinates on screen
	print(point.x, "Point X: ");
	print(point.y, "Point Y: ");

	// Variables for window lifetime, scaling, movement and events
	bool done = false;
	bool isDragging = false;
	int mouseX = 0;
	int mouseY = 0;
	SDL_Event event;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;


	//Main window loop, to keep the window open and handle scale / movement
	while(!done) {
		frameStart = SDL_GetTicks();

		sdlPoints.clear();
		for(auto p : points) {
			int x = pOffsetX + p.x * scale;
			int y = pOffsetY + (p.y * -scale) + vOffset;
			sdlPoints.push_back({x, y});
		}

		// Rendering the graph
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLines(renderer, &sdlPoints[0], sdlPoints.size());

		// Creating a square to mark the minimum point
		SDL_Rect rect;
		rect.h = 5;
		rect.w = 5;
		rect.x = pOffsetX + (int)((point.x * scale) - 2.5);
		rect.y = pOffsetY + (int)((point.y * -scale) - 2.5) + vOffset;

		// Drawing the square for the minimum point
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);

		// Pooling events to handle movement and zoom
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_MOUSEWHEEL:
					if(event.wheel.y > 0) {
						scale += 50;
					} else {
						scale -= 50;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					isDragging = true;
					break;
				case SDL_MOUSEBUTTONUP:
					isDragging = false;
					break;
				case SDL_MOUSEMOTION:
					int newMouseX, newMouseY;
					SDL_GetMouseState(&newMouseX, &newMouseY);
					if(isDragging) {
						pOffsetX -= (mouseX - newMouseX);
						pOffsetY -= (mouseY - newMouseY);
					}
					mouseX = newMouseX;
					mouseY = newMouseY;
					break;
				case SDL_QUIT:
					done = true;
					break;
			}
		}
		
		//Limiting the number of updates per second to 60, so that
		//The rendering doesn't take all of the CPU
		frameTime = SDL_GetTicks() - frameStart;
		if(frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	
	return 0;
}

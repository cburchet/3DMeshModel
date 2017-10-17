#include "algebra3.h"
#include <random>
#include <vector>

// Simple class for hold face data
class mesh{
	private:
	public:
		vec3 **square;
		mesh()
		{
			//create formula to set each vertice based on width of mesh. and height of mesh?
			square = new vec3*[4];
			for (int i = 0; i < 4; i++)
			{
				square[i] = new vec3[4];
			}
		}

		mesh(int rows, int cols, float width, float height)
		{
			square = new vec3*[rows];
			for (int k = 0; k < rows; k++)
			{
				square[k] = new vec3[cols];
			}
			float distW = width / (cols-1);
			float distH = height / (rows-1);
			float halfWide = cols / 2;
			float halfHeight = rows / 2;
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					double random = (double)rand() / (double)RAND_MAX;
					random *= 2;
					square[i][j] = vec3((halfWide * -1) + (distW * j), random, halfHeight - (distH * i));
				}
			}

		}
};
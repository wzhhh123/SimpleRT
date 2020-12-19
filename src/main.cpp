

#include "base/datatoimage.h"


int main() {

	//i*1.0f * 255 / 512, j*1.0f * 255 / 512, 0.0f)

	glm::vec3 *data = new glm::vec3[SIZE*SIZE];


	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			data[SIZE*i + j].x = i * 255.0f / SIZE;
			data[SIZE*i + j].y = 0;
			data[SIZE*i + j].z = j * 255.0f / SIZE;
		}
	}

	DataToImage(data);

	delete[] data;
}
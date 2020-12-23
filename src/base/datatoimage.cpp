

#include "datatoimage.h"
#include "glm.hpp"

void DataToImage(const dVec3* data) {

	FILE *fp;
	fp = fopen("../assets/image/test.ppm", "w");

	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", SIZE, SIZE);
	fprintf(fp, "255\n");
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			fprintf(fp, "%.0f %.0f %.0f\n", data[SIZE*i + j].x, data[SIZE*i + j].y, data[SIZE*i + j].z);
		}
	}

	fclose(fp);
}
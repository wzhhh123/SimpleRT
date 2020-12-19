

#include "datatoimage.h"


void DataToImage() {

	FILE *fp;
	fp = fopen("../image/test.ppm", "w");

	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", SIZE, SIZE);
	fprintf(fp, "255\n");

	for (int i = 0; i < 512; ++i) {
		for (int j = 0; j < 512; ++j) {
			fprintf(fp, "%.0f %.0f %.0f\n", i*1.0f * 255 / 512, j*1.0f * 255 / 512, 0.0f);
		}
	}

	fclose(fp);
}
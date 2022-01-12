

#include "datatoimage.h"
#include "glm.hpp"


//to ppm��  -255,0,0������ʾ�˸���ɫ����������ֵ�����������
void DataToImage(const dVec3* data) {

	FILE *fp;
	fp = fopen("../assets/image/test.ppm", "w");

	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", IMG_SIZE, IMG_SIZE);
	fprintf(fp, "255\n");
	for (int i = 0; i < IMG_SIZE; ++i) {
		for (int j = 0; j < IMG_SIZE; ++j) {
			fprintf(fp, "%.0f %.0f %.0f\n", data[IMG_SIZE*i + j].x, data[IMG_SIZE*i + j].y, data[IMG_SIZE*i + j].z);
		}
	}

	fclose(fp);
}

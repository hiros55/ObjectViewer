#include <vector>
#include <GL/glut.h>
#include "ObjView.h"
#define MAX_VERTICES_SIZE 4096

struct  Obj::Vector3f
{
	float x, y, z;
};


struct Obj::ObjDate {

	Vector3f* vertices;
	int vertices_count;

	Vector3f* normals;
	int normal_count;

	Vector3f* texcoords;
	int texcoord_count;

	int* tri_v_no;
	int* tri_vn_no;
	int* tri_vt_no;
	int tri_count;

	bool* material;
};

Obj::ObjDate* Obj::Load(const char* FileName) {

	FILE* file;

	file = fopen(FileName, "r");
	if (file == NULL)
		return NULL;


	Obj::ObjDate* Obj = new Obj::ObjDate();
	Obj->vertices = new Obj::Vector3f[MAX_VERTICES_SIZE];
	Obj->vertices_count = 0;
	Obj->normals = new Obj::Vector3f[MAX_VERTICES_SIZE];
	Obj->normal_count = 0;
	Obj->texcoords = new Obj::Vector3f[MAX_VERTICES_SIZE];
	Obj->texcoord_count = 0;
	Obj->tri_v_no = new int[MAX_VERTICES_SIZE * 3];
	Obj->tri_vn_no = new int[MAX_VERTICES_SIZE * 3];
	Obj->tri_vt_no = new int[MAX_VERTICES_SIZE * 3];
	Obj->tri_count = 0;
	Obj->material = new bool[MAX_VERTICES_SIZE * 3];

	char  s[256];//fgetsで読み込んだ文字列

	//一時代入用変数
	Obj::Vector3f vec;//頂点・法線・テクスチャ
	int v_no[4], vt_no[4], vn_no[4];
	//カウント
	int f_count;
	int i, j;

	while (!feof(file))
	{
		fgets(s, 256, file);

		if (s[0] == 'v') {

			if (s[1] == 'n') {
				sscanf(s, "vn %f %f %f", &vec.x, &vec.y, &vec.z);

				if (Obj->normal_count < MAX_VERTICES_SIZE) {
					Obj->normals[Obj->normal_count] = vec;
					Obj->normal_count++;
				}

			}
			else if (s[1] == 't') {
				sscanf(s, "vt %f %f %f", &vec.x, &vec.y, &vec.z);

				if (Obj->texcoord_count < MAX_VERTICES_SIZE) {
					Obj->texcoords[Obj->texcoord_count] = vec;
					Obj->texcoord_count++;
				}
			}
			else {
				sscanf(s, "v %f %f %f", &vec.x, &vec.y, &vec.z);

				if (Obj->vertices_count < MAX_VERTICES_SIZE) {
					Obj->vertices[Obj->vertices_count] = vec;
					Obj->vertices_count++;
				}
			}

		}

		if (s[0] == 'f') {
			f_count = sscanf(s, "f %d//%d %d//%d %d//%d", &v_no[0], &vn_no[0], &v_no[1], &vn_no[1], &v_no[2], &vn_no[2]);

			if (f_count == 6) {
				i = Obj->tri_count * 3;
				for (j = 0; j < 3; j++) {
					Obj->tri_v_no[i + j] = v_no[j] - 1;
					Obj->tri_vn_no[i + j] = vn_no[j] - 1;
					Obj->tri_vt_no[i + j] = vt_no[j] - 1;
				}
				Obj->tri_count++;

			}

			if (Obj->tri_count >= MAX_VERTICES_SIZE)
				break;
		}
	};
	fclose(file);
	return Obj;
}


 void Obj::RenderObj(Obj::ObjDate* objectDate) {
	int i = 0, j = 0, no;
	float a = 0, b = 0, c = 0;
	double v1_x = 0, v1_y = 0, v1_z = 0, v2_x = 0, v2_y = 0, v2_z = 0;
	float S = 0, s = 0;
	int no_1, no_2, no_3;

	for (i = 0; i < objectDate->tri_count; i++) {
		for (j = 0; j < 3; j++) {
			no = objectDate->tri_v_no[i * 3 + j];
			Vector3f& v = objectDate->vertices[no];

			if (j == 0) {
				v1_x = v.x; v1_y = v.y; v1_z = v.z;
				no_1 = no;
			}
			else if (j == 1) {
				v2_x = v.x; v2_y = v.y; v2_z = v.z;
				no_2 = no;
			}
			else {
				a = sqrt(fabs(v2_x - v1_x) + fabs(v2_y - v1_y) + fabs(v2_z - v1_z));
				b = sqrt(fabs(v2_x - v.x) + fabs(v2_y - v.y) + fabs(v2_z - v.z));
				c = sqrt(fabs(v1_x - v.x) + fabs(v1_y - v.y) + fabs(v1_z - v.z));
				no_3 = no;
				s = (a + b + c) / 2;
				S = sqrt(s * (s - a) * (s - b) * (s - c));


				if (S < 0.1) {
					objectDate->material[no_1] = true;
					objectDate->material[no_2] = true;
					objectDate->material[no_3] = true;
				}
				else {
					objectDate->material[no_1] = false;
					objectDate->material[no_2] = false;
					objectDate->material[no_3] = false;
				}
			}
		}

	}

	glBegin(GL_TRIANGLES);
	for (i = 0; i < objectDate->tri_count; i++) {


		for (j = 0; j < 3; j++) {

			no = objectDate->tri_vn_no[i * 3 + j];

			Vector3f& vn = objectDate->normals[no];
			glNormal3f(vn.x, vn.y, vn.z);

			no = objectDate->tri_v_no[i * 3 + j];
			Vector3f& v = objectDate->vertices[no];

			glVertex3f(v.x, v.y, v.z);


		}
	}
	glEnd();

	free(objectDate->vertices);
	free(objectDate->normals);
	free(objectDate->texcoords);
	free(objectDate->tri_vn_no);
	free(objectDate->tri_vt_no);
	free(objectDate->tri_v_no);
	free(objectDate->material);
}


 //未実装

 
/*void LoadMaterial(FILE * file) {

}*/

 /*float Truncation(float x) {
	 int num;
	 float ans, ans_at;
	 num = (int)x;

	 if (x >= 0) {
		 ans = x - num;
		 for (int l = 0; l < 2; l++) {
			 ans = ans * 10;
			 num = (int)ans;
			 ans = ans - num;
		 }
		 roundf(ans);
	 }
	 else {
		 ans = x + num;//-0.145
		 ans = 1 + ans;
	 }

	 return roundf(ans);
 }*/
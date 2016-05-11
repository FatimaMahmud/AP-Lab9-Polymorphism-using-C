#include <stdio.h>
#include <stdlib.h>

typedef (*vmethod)(struct data *d);

vmethod *tb_m;
vmethod *tb_v;

typedef struct Data {
	int rows;
	int columns;
	int *matrixs;
	vmethod *vtable_m;
	void(*init)(struct data *d);
	void(*multiply)(struct data *d, struct data *d1, struct data *d2);
	void(*add)(struct data *d, struct data *d1, struct data *d2);
} data;

typedef struct vector {
	data vectordata;
	vmethod *vtable_v;
	void(*Vinit)(struct vector *mat, int col, int row);
} v;

void printing();

void Mmultiply(data *d, data *d1, data *store){
	printf("\nMultiplying\n");
	int i, j, k, values = 0, in = 0;

	for (i = 0; i < d->rows; i++) {
		for (j = 0; j < d1->columns; j++) {
			values = 0;
			for (k = 0; k < d1->rows; k++){
				(values += (d->matrixs[(i*d->columns) + k] * d1->matrixs[(k*d1->columns) + j]));
				//printf("%d \n", values);
			}
			//printf("%d  %d %d \n", d->rows, d1->rows, d1->columns);
			store->matrixs[in] = values;

			//printf("%d %d\n", values ,in);
			in++;

		}
	}
}

void Vmultiply(v *v1, v *v2, v *store){
	printf("\nMultiplying\n");
	int i, j, k, values = 0, in = 0;

	for (i = 0; i < v1->vectordata.rows; i++) {
		for (j = 0; j < v2->vectordata.columns; j++) {
			values = 0;
			for (k = 0; k < v2->vectordata.rows; k++){
				(values += (v1->vectordata.matrixs[(i*v1->vectordata.columns) + k] * v2->vectordata.matrixs[(k*v2->vectordata.columns) + j]));
				//printf("%d \n", values);
			}
			//printf("%d  %d %d \n", d->rows, d1->rows, d1->columns);
			store->vectordata.matrixs[in] = values;

			//printf("%d %d\n", values ,in);
			in++;

		}
	}
}



void Madds(data *d, data *d1, data *store){
	printf("Adding Matrix\n");
	int i, j, values = 0, in = 0;
	for (i = 0; i < d->rows; i++) {
		for (j = 0; j < d1->columns; j++) {
			(values += (d->matrixs[(i*d->columns) + j] * d1->matrixs[(i*d1->columns) + j]));

		}
		store->matrixs[in] = values;
		in++;
		values = 0;
	}
}

void Vadds(v *d, v *d1, v *store){
	printf("Adding Vector\n");
	int i, j, values = 0, in = 0;
	for (i = 0; i < d->vectordata.rows; i++) {
		for (j = 0; j < d1->vectordata.columns; j++) {
			(values += (d->vectordata.matrixs[(i*d->vectordata.columns) + j] * d1->vectordata.matrixs[(i*d1->vectordata.columns) + j]));

		}
		store->vectordata.matrixs[in] = values;
		in++;
		values = 0;
	}
}


void *Minit(data *d, int row, int col) {

	int **arr;
	d->add = Madds;
	d->multiply = Mmultiply;
	d->columns = col;
	d->rows = row;
	d->matrixs = (int *)malloc(sizeof(int)*(row*col));
	int i, j;


	for (i = 0; i < (col*row); i++){
		d->matrixs[i] = i + 1;
	}

}

void Vinit(v *vectorptr, int row, int col) {

	int **arr;
	vectorptr->vectordata.add = Vadds;
	vectorptr->vectordata.multiply = Vmultiply;
	vectorptr->vectordata.columns = col;
	vectorptr->vectordata.rows = row;
	vectorptr->vectordata.matrixs = (int *)malloc(sizeof(int)*(row*col));
	int i, j;


	for (i = 0; i < (col*row); i++){
		vectorptr->vectordata.matrixs[i] = i + 1;

	}
}



void printing(data *d){

	int i, j, k = 0;
	printf("\n");
	printf("Matrix AB Multiplication\n");
	for (k = 0; k < d->rows; k++){
		for (j = 0; j < d->columns; j++){
			printf("\t %d", d->matrixs[(k * d->columns) + j]);
			//printf("\n");	
		}
	}
	printf("\n");
}


int L1_norm(data *m1){
	int i = 0;
	int sum = 0;
	int j = 0;
	int final = 0;
	for (j = 0; j < m1->columns; j++){
		for (i = 0; i<(m1->rows); i++){
			sum = sum + m1->matrixs[j*m1->columns + i];
		}
		if (sum>final){
			final = sum;
		}
	}

	return final;

}

int L1_norm_v(data *m1){
	int i = 0;
	int sum = 0;
	int j = 0;
	int final = 0;
	for (i = 0; i < (m1->rows); i++){
		sum = sum + m1->matrixs[i];

	}

	return sum;

}

int main() {

	int value;

	printf("Press 0 for Vector functions, Press 1 for Matrix functions");
	scanf_s("%d", &value);

	data matrix1;
	data matrix2;
	data matrixAddResult;
	data matrixMultResult;

	v vector1;
	v vector2;
	v vectorStore;

	int i, j, k;

	int Mrows = 2, Mcolumns = 3;
	int M1rows = 2, M1columns = 3;

	int Vrows = 2, Vcolumns = 1;
	int V1rows = 2, V1columns = 1;

	if (value == 1){
		matrix1.init = Minit;
		matrix1.init(&matrix1, 2, 3);
		matrix2.init = Minit;
		matrix2.init(&matrix2, 3, 2);

		if (Mcolumns == V1columns && Mrows == M1rows){
			matrix2.add(&matrix1, &matrix2, &matrixAddResult);
			printing(&matrixAddResult);
		}
		else
			printf("Addition not possible!\n");

		if (Mcolumns == M1rows){
			matrix2.multiply(&matrix1, &matrix2, &matrixMultResult);
			printing(&matrixMultResult);
		}
		else
			printf("Multiplication not possible!\n");


		tb_m = (vmethod *)malloc((10)* sizeof(vmethod));
		tb_m[0] = L1_norm;
		matrix1.vtable_m = tb_m;
		matrix2.vtable_m = tb_m;
		printf("%d is L1 norm of first matrix \n", matrix1.vtable_m[0](&matrix1));
		printf("%d is L1 norm of second matrix \n", matrix2.vtable_m[0](&matrix2));

	}
	else if (value == 0){

		tb_v = (vmethod *)malloc((10)* sizeof(vmethod));
		vector2.Vinit = Vinit;
		vector2.Vinit(&vector2, 1, V1rows);
		vector1.Vinit = Vinit;
		vector1.Vinit(&vector1, 1, V1columns);
		if (V1rows == V1columns){
			vector1.vectordata.add(&vector1, &vector2, &vectorStore);
			printf("Addition of vector:\n");
			for (i = 0; i < vector2.vectordata.rows; i++){

				printf("%d  ", vectorStore.vectordata.matrixs[i]);

				printf("\n");
			}
		}
		else
			printf("Addition not possible!\n");

		tb_v[0] = L1_norm_v;
		vector1.vtable_v = tb_v;
		vector2.vtable_v = tb_v;
		printf("%d is L1 norm of first vector \n", vector1.vtable_v[0](&vector1.vectordata));
		printf("%d is L1 norm of second vector \n", vector2.vtable_v[0](&vector2.vectordata));

	}


	//int i;
	//data d[] = { { 2, 3, malloc(sizeof(int)* 22500) }, { 3, 2, malloc(sizeof(int)* 22500) }, { 2, 2, malloc(sizeof(int)* 22500) } };
	//data d2;

	//v vect2;
	////v vect1;
	////v vect3;
	////v vect4;
	//v vect = { 2, 3, malloc(sizeof(int)* 22500) };

	////vect2.vtable_ptr = vtable_vector_class;
	////vect2.vtable_ptr(&vect1, &vect2, &vect3);

	//vect2.vectordata.init(&vect2.vectordata);

	//d[0].init = init;
	//d[1].init = init;

	//d[0].init(&d[0]);
	//d[1].init(&d[1]);

	//d[0].multiply(&d[0], &d[1], &d[2]);
	//printing(&d[2]);

	//L1Norm(&d[0], &d[1], &d[2]);

	//d[0].add(&d[0], &d[1], &d[2]);
	//printing(&d[2]);
}



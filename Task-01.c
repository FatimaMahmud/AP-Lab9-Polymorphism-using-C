#include <stdio.h>
#include <stdlib.h>

typedef struct Data {
	int rows;
	int columns;
	int *matrixs;
	void(*init)(struct data *d);
	void(*multiply)(struct data *d, struct data *d1, struct data *d2);
	void(*add)(struct data *d, struct data *d1, struct data *d2);
} data;

typedef struct vector {
	data vectordata;
} vector;

void printing();

unsigned int c[15][15];

int Arows;
int Acolumns;
int Bcolumns;
int Brows;
int * mA;
int * mB;

void multi(data *d, data *d1, data *store){
	printf("\nMultiplying");
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

			printf("%d\n", values);
			in++;

		}
	}
}



void adds(data *d, data *d1, data *store){
	printf("Adding");
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

void *init(data *d) {

	int MatrixArows = (d)->rows;
	int MatrixAcolumns = (d)->columns;

	Arows = MatrixArows;
	Acolumns = MatrixAcolumns;

	printf("\nMatrixArows: %d\n", MatrixArows);
	printf("MatrixAcolumns: %d\n", MatrixAcolumns);

	d->multiply = multi;
	d->add = adds;


	int i, j, k, l = 0, in = 1;

	unsigned int values = 0;

	printf("\nMatrixA");
	for (i = 0; i<MatrixArows; i++){
		for (j = 0; j<MatrixAcolumns; j++){
			d->matrixs[(i * MatrixAcolumns) + j] = in++;
			printf("\n\t %d", d->matrixs[(i * MatrixAcolumns) + j]);
		}
		//printf("\n");
	}

}


void printing(data *d){

	int i, j, k = 0;
	printf("\n");
	printf("Matrix AB Multiplication\n");
	for (k = 0; k < d->rows; k++){
		for (j = 0; j < d->columns; j++){
			printf("\t %d", d->matrixs[(k * d->columns) + j]);
			printf("\n");
		}
	}
	printf("\n");
}

int main() {


	int i;
	data d[] = { { 2, 3, malloc(sizeof(int)* 22500) }, { 3, 2, malloc(sizeof(int)* 22500) }, { 2, 2, malloc(sizeof(int)* 22500) } };
	data d2;

	d[0].init = init;
	d[1].init = init;

	d[0].init(&d[0]);
	d[1].init(&d[1]);

	d[0].multiply(&d[0], &d[1], &d[2]);
	printing(&d[2]);

	d[0].add(&d[0], &d[1], &d[2]);
	printing(&d[2]);
}



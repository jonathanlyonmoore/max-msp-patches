// K-Nearest-Neigbor Stuff for comparison to template...#include "tap.bink.h"#define malloc(x) getbytes(x)#define assert(x) (x)#define FLT_MAX 1E100int majority(NN *knn, int k, FEATURE *database);NN *k_nn_new(int k);float k_nn(NN *nn, int k, int candidate, float dist);float calc_dist(FEATURE *unknown, FEATURE *known,float *weights, float min_dist);void makeFeatureVector(FEATURE *database, float *array, int size, int id, int class_id){	int i;	  database[id].id = id;	database[id].symbol_id = class_id;  for (i = 0; i < size; i++)		database[id].feature[i] = array[i];}/*** index - if unknown is in the database, otherwise set to -1** size - size of the database*/int find_nn(FEATURE *unknown, int index, FEATURE *database, int size, 			float *weights, int k){	int i, min = 1;	float dist, min_dist = FLT_MAX;	NN *knn;		knn = k_nn_new(k); // initialize an array to store the nn's				for (i = 0; i < size; i++)	{    if (i != index) // allows leave-one-out method		{			dist = calc_dist(unknown, &database[i], weights, min_dist);			if (dist < min_dist)			{			  min_dist = dist;			  min = i;			  min_dist = k_nn(knn, k, min, min_dist);			}		}	}	min = majority(knn, k, database); 	freebytes(knn, k);	return (min);}int majority(NN *knn, int k, FEATURE *database){/* returns class number. NB: no tie breaker */	int i, max_i, max = 0;	int classes[MAX_N_CLASSES];		for (i = 0; i < MAX_N_CLASSES; i++)		classes[i] = 0;			for (i = 0; i < k; i++)		classes[database[knn[i].index].symbol_id]++;		for (i = 0; i < MAX_N_CLASSES; i++)		if (classes[i] > max)		{			max = classes[i];			max_i = i;		}	return(max_i);}	NN *k_nn_new(int k){	int i;	NN *nn;	nn = (NN *)malloc(sizeof(NN) * k);	assert (nn != NULL);	for (i = 0; i < k; i++)	{		nn[i].index = -1;		nn[i].dist = FLT_MAX;	}	return(nn);}	float k_nn(NN *nn, int k, int candidate, float dist){	int i, insert;	for (i = 0; i < k; i++)	{		if (dist < nn[i].dist)			break;	}	if (i < k)	{		insert = i;				for (i = k - 1; i > insert; i--)			nn[i] = nn[i - 1];				nn[insert].index = candidate;		nn[insert].dist = dist;	}	return(nn[k - 1].dist);}float calc_dist(FEATURE *unknown, FEATURE *known, float *weights, float min_dist){	float dist = 0;	int i;		for (i = 0; i < N_FEATURES; i++)	{/* City-block */#if 1		dist += unknown->feature[i] * known->feature[i];   }   if (dist != 0.)		return(1. / dist); 	else 		return(FLT_MAX);#else/* Euclidean */			dist += weights[i] * (unknown->feature[i] - known->feature[i])												* (unknown->feature[i] -  known->feature[i]);	}	return((float)sqrt(dist)); #endif}
#include <stdio.h>
#include <stdlib.h>

//variables assigned in crystal_phase
int size;
double phase_tol = .99;
//dest is init to 0, src is the double array
int contour2D(double *src, double *dest, int x, int y){

	if(x < 0 || y < 0 || x >= size || y >= size)//out of bounds case
		return -1;//same case for when active if oob

	if(dest[y*size +x] > 0){//if this spot is solved
		return dest[y*size +x];//don't solve again, return it.
	}

	if(dest[y*size +x]==-1){//if active return active
		return -1;
	}

	if(src[y*size+x] < phase_tol){//must be here to close recursion
		return dest[y*size +x] = 0;
	}

	dest[y*size +x] = -1;//set active if all cases pass

	int min = size*size;
	int tmp;

	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){

			tmp = contour2D(src, dest, x+j, y+i);
			if( tmp != -1 && tmp < min){
				min = tmp;
			}
			if(tmp ==0)
				return dest[y*size +x] = min+1;

		}
	}
	src[y*size +x] = 1;
	return (dest[y*size +x] = min+1);
	
}


//assumes dest is initialized to zero
int contour3D(double *src, double *dest, int x, int y, int z, int size){
    int idx = z*size*size + y*size + x;

	if(x < 0 || y < 0 || z < 0 || x >= size || y >= size|| z >= size)//out of bounds case
		return -1;//same case for when active if oob

	if(dest[idx] != 0){//if this spot is solved, or active
		return dest[idx];//don't solve again, return it.
	}
    //zero in source implies border
    if(src[idx] == 0){
        return 0;
    }

	dest[idx] = -1;//set active if all cases pass

	int min = size*size*size;
	int tmp;

    for( int k = -1; k<2; k++){
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){

                tmp = contour3D(src, dest, x+j, y+i, z+k, size);
                if( tmp != -1 && tmp < min){
                    min = tmp;
                }
                if(tmp ==0)
                    return dest[idx] = 1;

            }
        }
    }
	src[idx] = 1;
	return (dest[idx] = min+1);
	
}





void printArray(double *a){

	for(int i = 0; i < size; i++){
		for(int j = 100; j< 200; j++){
			printf("%f ",a[i*size+j]);
		}
		printf("\n%d\n",i);
	}
}



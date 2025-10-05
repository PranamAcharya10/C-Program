/*Merge Sort using OpenMP*/

#include <stdio.h> 
#include <stdlib.h> 
#include<math.h>
#include <omp.h> 
#include <sys/time.h>
#define SIZE 99999       //SIZE-- number of elements to be sorted

// C program for generating a random number in a given range. 
void generate_list(int A[], int n) 
{ 
     int i,j,t; 
     for (i = 0; i <n; i++)  
    {  
        A[i]=i; 
     }  
		 for (i = 0; i <n; i++)  
    { 
			j= rand()%n;  	//Use rand() function to generate random numbers
			t=A[i];
			A[i]=A[j];
			A[j]=t;
		}
}

//Function merge that merges two sorted subarrays
void merge(int  a[], int  l, int  m, int  r)
{
	int  n1=m-l+1,n2=r-m;
	int  left[n1+1], right[n2+1],i,j,k;
	
	//copy first part to temporary array
	for(i=0; i<n1; i++)
		left[i]=a[l+i];
	left[i]=1000000;
	
	//copy second part to temporary array
	for(j=0; j<n2; j++)
		right[j]=a[m+1+j];
	right[j]=1000000;
	
	//Initialize the pointers
	i=0;
	j=0;
	
	//combine the arrays on the basis of order
	for(k=l;k<=r;k++)
	{
		if(left[i]<=right[j])
		{
			a[k]=left[i];
			i++;
		}
		else
		{
			a[k]=right[j];
			j++;
		}
	}
}

//Recursive Merge Sort Function 
//Function partitions the input arrray into two subarrays
void mergeSortSerial(int  a[], int  l, int  r)
{
		int  m;
		if (l < r)
		{
    m = floor((l + r)/2);				//Find the mid index
    mergeSortSerial(a,l,m); 		//call 1 -- to sort left subarray
    mergeSortSerial(a,m+1,r); 	//call 2 -- to sort right subarray
    merge(a,l,m,r);							//Merge the two sorted subarrays
    }
}

void mergeSort (int  a[], int  l, int  r)
{
		int  m;
    if (l < r){
        if ((r-l) > 1000)
				{
            m = floor((l + r)/2);	//Find the mid index
            #pragma omp parallel sections //num_threads(2)
        		{
	            #pragma omp section
	            {
	                mergeSort(a,l,m); //call 1 -- to sort left subarray
	            }
	            #pragma omp section
	            {
	                mergeSort(a,m+1,r); //call 2 -- to sort right subarray
	          	}
          	}
            merge(a,l,m,r);					//Merge the two sorted subarrays
        } 
				else
				{
					//If SIZE<=1000 go with serial processing
					mergeSortSerial(a, l, r);		
				}
    }
}

int main(){
		int  A[SIZE],B[SIZE],i;		//Declare A[] with SIZE elements
		double serialstart, serialfinish, serialexectime;
		double parallelstart, parallelfinish, parallelexectime;
		
    //Call generate_list() to create an array of random numbers
		generate_list(A, SIZE); 
		for(i=0;i<SIZE;i++)
			B[i]=A[i];							//Copy the elements in A[] to B[]
			
		//Display the array before sorting
    printf("\nThe original Array is:\n");
    for(i=0;i<SIZE;i++)
    	printf("%d ",A[i]); 		//Use ld instead of d as elements are long integers
    
    parallelstart=omp_get_wtime();	//start time
    omp_set_nested(1);							//Enable nested parallelism
		omp_set_num_threads(2);
    
		#pragma omp parallel
		{
			#pragma omp single
				mergeSort(A, 0, SIZE-1); 										//Call mergeSort()
  	}
		parallelfinish=omp_get_wtime();									//finish time
		parallelexectime=parallelfinish-parallelstart;	//Execution Time	
	
    //Display the Sorted Array
    printf("\nThe Sorted Array is:\n");
    for(i=0;i<SIZE;i++)
    	printf("%d ",A[i]);
    	
    printf("\nTime taken with Parallel Processing is %f",parallelexectime);
    
		serialstart=omp_get_wtime();
		mergeSortSerial(B, 0, SIZE-1); 
		serialfinish=omp_get_wtime();
		serialexectime=serialfinish-serialstart;
    
		printf("\nTime taken with Serial Processing is %f",serialexectime);
    return(0);
}

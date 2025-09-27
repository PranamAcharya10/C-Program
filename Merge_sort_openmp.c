#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<omp.h>
#include<conio.h>

#define SIZE 99999

void generate_list(int A[], int n)
{
	int i,j,t;
	for(i=0; i<n; i++)
	{
		A[i]=i;
	}
	for(i=0; i<n; i++)
	{
		j = rand()%n;
		t= A[i];
		A[i]=A[j];
		A[j]=t;
	}
}

void print(int a[], int n)
{
    int i;
    for(i=0; i<n; i++)
    	printf("%d", a[i]);
    printf("\n");
}


void merge(int a[], int l, int m, int r)
{
	int n1=m-l+1, n2=r-m;
	int left[n1+1], right[n2+1],i,j,k;
	
	for(i=0; i<n1; i++)
		left[i]= a[l+i];
	left[i]=100000;
	
	for(j=0; j<n2; j++)
		right[j]= a[m+1+j];
	right[j]=100000;
		
	i=0;   
	j=0;
	
	for(k=l;k<=r; k++)
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


void mergesortSerial(int a[], int l, int r)
{
	if(l<r)
	{
		int m=floor((l+r)/2);
			mergesortSerial(a,l,m);
			mergesortSerial(a,m+1,r);
			merge(a,l,m,r);
	}
}


void mergesort(int a[], int l, int r)
{
	int m;
	if(l<r)
	{
		if((r-l)>10000)
		{
			int mid = floor((l + r)/2);
			#pragma omp parallel
			{
				#pragma omp single
				{
					#pragma omp task firstprivate (a,l,m)
						mergesort(a,l,mid);
					#pragma omp task firstprivate (a,m,r)
						mergesort(a,mid,r);
							#pragma omp taskwait
								merge(a,l,mid,r);
				}
			}
		}
		else
		{
			mergesortSerial(a, l, r);
		}
	}
}


int main()
{
    int A[SIZE],B[SIZE],i;
    double serialstart, serialfinish,serialexectime;
    double parallelstart,parallelfinish,parallelexectime;
    
    generate_list(A, SIZE);
    for(i=0;i<SIZE;i++)
    	B[i]=A[i];
    
	printf("The original Array:");	
    print(A, SIZE);
    
    parallelstart = omp_get_wtime();
    omp_set_nested(1);
    omp_set_num_threads(2);
    
    #pragma omp parellel
    {
    	mergesort(A,0,SIZE-1);
	}
	parallelfinish = omp_get_wtime();
	parallelexectime=parallelfinish-parallelstart;
	
	printf("\n Sorted array");
	for(i=0;i<SIZE;i++)
		printf("%d",A[i]);
	
	printf("\nTime taken for parallel processing: %f",parallelexectime);

	serialstart = omp_get_wtime();
	mergesortSerial(B, 0, SIZE-1);
	serialfinish=omp_get_wtime();
	serialexectime=serialfinish-serialstart;
	
	printf("\nTime taken for serial processing: %f",serialexectime);
    
    /*printf("\nEnter n: ");
    scanf("\n%d",&n);
    int a[n];
    printf("Enter %d numbers to be sorted",n);
    for(i=0; i<n; i++)
        scanf("%d",&a[i]);
    printf("Unsorted array: ");
    print(a,n);

    merge_sort(a,0,n-1);
    printf("\nSorted array:");
    print(a,n);
    return 0;*/
}

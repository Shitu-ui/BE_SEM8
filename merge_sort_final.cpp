#include<iostream>
#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

void merge(vector<int>&arr,int low,int mid,int high)
{
	int n1=(mid-low+1);
	int n2=high-mid;
	
	int arr1[n1];
	int arr2[n2];
	
	for(int i=0;i<n1;i++)
	{
		arr1[i]=arr[low+i];
	}
	
	for(int j=0;j<n2;j++)
	{
		arr2[i]=arr[mid+1+j];
	}
	
	int i=0,j=0;
	int k=low;
	while(i<n1 && j<n2)
	{
		if(arr1[i]<=arr2[j])
		{
			arr[k++]=arr1[i];
			i++;
		}
		else
		{
			arr[k++]=arr[j];
			j++;
		}
	}
	while(i<n1)
	{
		arr[k++]=arr[i++];
	}
	while(j<n2)
	{
		arr[k++]=arr[j++];
	}
}

void mergeSort(vector<int>&arr,int low,int high)
{
	if(low<high)
	{
		int mid=(low+high)/2;
		mergeSort(arr,low,mid);
		mergeSort(arr,mid+1,high);
		merge(arr,low,mid,high);
	}
}

void parallel_mergeSort(vector<int>&arr,int low,int high)
{
	if(low<high)
	{
		int mid=(low+high)/2;
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				parallel_mergeSort(arr,low,mid);
			}
			#pragma omp section
			{
				parallel_mergeSort(arr,mid+1,high);
			}
		}
		merge(arr,low,mid,high);
	}
}

int main()
{
	int n;
	cout<<"Enter number of elements: ";
	cin>>n;
	vector<int>arr(n);
	vector<int>brr(n);
	for(int i=0;i<n;i++)
	{
		cin>>arr[i];
		brr[i]=arr[i];
	}
	
	double start,end;
	start=omp_get_wtime();
	mergeSort(arr,0,n-1);
	end=omp_get_wime();
	cout<<"Time taken by Sequential Time:"<<end-start<<endl;
	for(int i=0;i<n;i++)
	{
		cout<<arr[i]<<" ";
	}
	cout<<endl;
	
	arr=brr;
	start=omp_get_wtime();
	parallel_mergeSort(arr,0,n-1);
	end=omp_get_wtime();
	cout<<"Time taken by parallel Time: "<<end-start<<endl;
	for(int i=0;i<n;i++)
	{
		cout<<arr[i]<<" ";
	}
	cout<<endl;
	return 0;
}
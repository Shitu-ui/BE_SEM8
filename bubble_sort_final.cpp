#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

void bubble_seq(vector<int>&arr)
{
	int n=arr.size();
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n-i-1;j++)
		{
			if(arr[j]>arr[j+1])
			{
				swap(arr[j],arr[j+1]);
			}
		}
	}
}

void bubble_par(vector<int>&brr)
{
	int n=brr.size();
	for(int i=0;i<n;i++)
	{
		#pragma omp for
		for(int j=1;j<n;j+=2)
		{
			if(brr[j]<brr[j-1])
			{
				swap(brr[j],brr[j-1]);
			}
		}
		
		#pragma omp barrier
		#pragma omp for
		for(int j=2;j<n;j+=2)
		{
			if(brr[j]<brr[j-1])
			{
				swap(brr[j],brr[j-1]);
			}
		}
	}
}	

void print(vector<int> temp)
{
	for(int i=0;i<temp.size();i++)
	{
		cout<<temp[i]<<" ";
	}
	cout<<endl;
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
	
	double stime,etime;
	stime=omp_get_wtime();
	bubble_seq(arr);
	etime=omp_get_wtime();
	cout<<"Sequential time of Bubble: "<<etime-stime<<endl;
	print(arr);
	
	stime=omp_get_wtime();
	bubble_par(brr);
	etime=omp_get_wtime();
	cout<<"Parallel time of Bubble: "<<etime-stime<<endl;
	print(brr);
	
}
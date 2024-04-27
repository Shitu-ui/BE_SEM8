#include <iostream>
#include <cuda_runtime.h>
#include <conio.h>
using namespace std;

__global__ void addVectors(int* A, int* B, int* C, int n) 
{
    int i = blockIdx.x * blockDim.x + threadIdx.x; 
    if (i < n) 
    {
        C[i] = A[i] + B[i];
    }
}

int main() 
{
    int n;
    cout << "Enter the size of vectors: ";
    cin >> n;

    int* A, * B, * C;
    int size = n * sizeof(int);

    // Allocate memory on the host  
    cudaMallocHost(&A, size);  
    cudaMallocHost(&B, size);  
    cudaMallocHost(&C, size);

    // Initialize vector A
    cout << "Enter values for vector A:" << endl;
    for (int i = 0; i < n; i++) 
    {
        cout << "A[" << i << "]: ";
        cin >> A[i];
    }

    // Initialize vector B
    cout << "Enter values for vector B:" << endl;
    for (int i = 0; i < n; i++) 
    {
        cout << "B[" << i << "]: ";
        cin >> B[i];
    }

    // Get the number of threads per block from the user
    int threadsPerBlock;
    cout << "Enter the number of threads per block: ";
    cin >> threadsPerBlock;

    // Calculate the number of blocks
    int numBlocks = (n + threadsPerBlock - 1) / threadsPerBlock;

    // Allocate memory on the device  
    int* dev_A, * dev_B, * dev_C;  
    cudaMalloc(&dev_A, size);  
    cudaMalloc(&dev_B, size);  
    cudaMalloc(&dev_C, size);

    // Copy data from host to device
    cudaMemcpy(dev_A, A, size, cudaMemcpyHostToDevice);  
    cudaMemcpy(dev_B, B, size, cudaMemcpyHostToDevice);

    // Create CUDA events for timing
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Record the start event
    cudaEventRecord(start);

    // Launch the kernel  
    addVectors<<<numBlocks, threadsPerBlock>>>(dev_A, dev_B, dev_C, n);

    // Record the stop event
    cudaEventRecord(stop);

    // Synchronize to ensure kernel execution is complete
    cudaDeviceSynchronize();

    // Calculate the elapsed time
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    cout << "Execution Time: " << milliseconds << " ms" << endl;

    // Copy data from device to host
    cudaMemcpy(C, dev_C, size, cudaMemcpyDeviceToHost);

    // Print the results
    cout << "Resultant vector C:" << endl;
    for (int i = 0; i < n; i++) 
    {
        cout << C[i] << " ";
    }
    cout << endl;

    // Free memory  
    cudaFree(dev_A);  
    cudaFree(dev_B);  
    cudaFree(dev_C);  
    cudaFreeHost(A);  
    cudaFreeHost(B);  
    cudaFreeHost(C);

    // Destroy the events
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
	getch();
    return 0;
}
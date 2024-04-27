#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <conio.h>
using namespace std;

#define BLOCK_SIZE 5

__global__ void gpuMM(int *A, int *B, int *C, int N)
{
    // Matrix multiplication for NxN matrices C=A*B
    // Each thread computes a single element of C
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    int sum = 0.f;
    for (int i = 0; i < N; ++i)
        sum += A[row * N + i] * B[i * N + col];

    C[row * N + col] = sum;
}

int main(int argc, char *argv[])
{
    int N;
    cout << "Enter the size of square matrices (N x N): ";
    cin >> N;

    // Allocate memory on the host
    int *A, *B, *C;
    A = new int[N * N];
    B = new int[N * N];
    C = new int[N * N];

    // Initialize matrices on the host
    cout << "Enter values for Matrix A:" << endl;
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < N; i++)
        {
            cout << "A[" << j << "][" << i << "]: ";
            cin >> A[j * N + i];
        }
    }

    cout << "Enter values for Matrix B:" << endl;
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < N; i++)
        {
            cout << "B[" << j << "][" << i << "]: ";
            cin >> B[j * N + i];
        }
    }

    // Allocate memory on the device
    int size = N * N * sizeof(int);
    int *dA, *dB, *dC;
    cudaMalloc(&dA, size);
    cudaMalloc(&dB, size);
    cudaMalloc(&dC, size);

    // Copy matrices from host to device
    cudaMemcpy(dA, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, B, size, cudaMemcpyHostToDevice);

    dim3 threadBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid((N + BLOCK_SIZE - 1) / BLOCK_SIZE, (N + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // Measure GPU kernel execution time
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    // Execute the matrix multiplication kernel
    gpuMM<<<grid, threadBlock>>>(dA, dB, dC, N);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    cout << "GPU Execution Time: " << milliseconds << " ms" << endl;

    // Copy the GPU result back to CPU
    cudaMemcpy(C, dC, size, cudaMemcpyDeviceToHost);

    // Print the result matrix
    cout << "Resultant Matrix C:" << endl;
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < N; i++)
        {
            cout << C[j * N + i] << " ";
        }
        cout << endl;
    }

    // Free memory
    cudaFree(dA);
    cudaFree(dB);
    cudaFree(dC);
    delete[] A;
    delete[] B;
    delete[] C;
	getch();
    return 0;
}

#include <stdio.h>
#include <omp.h>

int main()
{
    int resultado = 0;

    int a[] = {1, 2, 3, 4, 5, 6};
    int b[] = {1, 2, 3, 4, 5, 6};
#pragma omp parallel shared(a, b)
    {
        int parcial = 0;
// Nun_thread = 3;
#pragma omp for schedule(static, 2)
        for (int i = 0; i < 6; i++)
        {
            parcial += a[i] * b[i];
        }
        printf("Thread %d: Processou, resultado parcial %d\n", omp_get_thread_num(), parcial);
#pragma omp critical // precisa do critical para evitar que mais de uma thread acesse a variável resultado ao mesmo tempo
        {
            resultado += parcial;
        }
    }

    printf("O produto escalar eh: %d\n", resultado);
}
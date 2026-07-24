// Identificar os números primos em um intervalo grande de inteiros.
#include <stdio.h>
#include <math.h>
#include <time.h>

int verificar_primo(int num);

int main()
{
    int limite;
    printf("Digite um numero para ser o fim do intervalo: ");
    scanf("%d", &limite);

    clock_t inicio, fim;
    double tempo_cpu;

    inicio = clock();

    for (int i = 2; i <= limite; i++)
    {
        if (verificar_primo(i))
        {
            printf("%d ", i);
        }
    }

    fim = clock();

    tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\n\nTempo de execucao: %f segundos\n", tempo_cpu);
    return 0;
}

int verificar_primo(int num)
{
    if (num == 2)
    {
        return 1;
    }
    if (num % 2 == 0)
    {
        return 0;
    }
    for (int i = 3; i <= sqrt(num); i += 2)
    {
        if (num % i == 0)
            return 0; // O número é divisível por outro número além de 1 e ele mesmo
    }
    return 1; // O número é primo
}
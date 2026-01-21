#include <stdio.h>
#include <openacc.h>
#define N 1000000

int main() {   /* acc_async.c */
        float a[N], b[N], c[N];
        for (int i = 0; i < N; ++i) { // Inicialização
            a[i] = i * 1.0f;
            b[i] = (N - i) * 1.0f;
        }
        // Cópia e execução assíncrona no device com a fila async 1
        #pragma acc data copyin(a[0:N], b[0:N]) copyout(c[0:N])
        {
            #pragma acc parallel loop async(1)
            for (int i = 0; i < N; ++i) {
                c[i] = a[i] + b[i];
            }
            // Verifica se a operação assíncrona já terminou
            while (!acc_async_test(1)) {
                printf("Aguardando término da fila async 1...\n");
            }
            // Aguarda todas as operações assíncronas (em qualquer fila)
            acc_async_wait_all();
            printf("Todas as atividades assíncronas concluídas.\n");
        }
        // Verificação simples do resultado
        if (c[0] == (a[0] + b[0])) {
            printf("Resultado correto: c[0] = %.2f\n", c[0]);
        } else {
            printf("Erro no cálculo!\n");
        }
        return 0;
    }

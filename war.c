#include <stdio.h>
#include <string.h>
#include <locale.h>

/* * Definição da struct Territorio
 * Estrutura composta para agrupar as informações de cada território.
 * Campos:
 * - nome: armazena o nome do território (até 30 caracteres)
 * - cor: armazena a cor do exército (até 10 caracteres)
 * - tropas: armazena a quantidade numérica de tropas
 */
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    // Declaração do vetor de structs para armazenar 5 territórios
    struct Territorio territorios[5];
    int i;

    printf("=== Sistema de Construção de Territórios ===\n");
    printf("Por favor, cadastre os 5 territórios abaixo:\n");

    // --- Entrada de Dados ---
    // Laço 'for' para iterar 5 vezes e preencher cada posição do vetor
    for (i = 0; i < 5; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);

        // 1. Leitura do Nome
        // O espaço antes do % ( " %[^\n]" ) serve para limpar o buffer do teclado 
        // e permitir a leitura de nomes compostos (ex: "Territorio Norte").
        printf("Nome do território: ");
        scanf(" %[^\n]", territorios[i].nome);

        // 2. Leitura da Cor
        printf("Cor do exército: ");
        scanf(" %[^\n]", territorios[i].cor);

        // 3. Leitura das Tropas
        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // --- Exibição dos Dados ---
    // Percorre o vetor novamente para mostrar o relatório consolidado
    printf("\n\n==========================================\n");
    printf("      BASE DE DADOS DOS TERRITÓRIOS       \n");
    printf("==========================================\n");

    for (i = 0; i < 5; i++) {
        printf("ID: %d | Nome: %-20s | Cor: %-10s | Tropas: %d\n", 
               i + 1, 
               territorios[i].nome, 
               territorios[i].cor, 
               territorios[i].tropas);
    }
    printf("==========================================\n");

    return 0;
}
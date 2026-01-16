#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// Definição da struct Territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Protótipos das Funções ---
void cadastrarTerritorios(struct Territorio* mapa, int qtd);
void exibirTerritorios(struct Territorio* mapa, int qtd);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);

int main() {
    setlocale(LC_ALL, "Portuguese"); // Permite acentuação
    srand(time(NULL)); // Inicializa a semente para números aleatórios

    struct Territorio* mapa;
    int qtdTerritorios;
    int idAtacante, idDefensor;
    int continuar = 1;

    printf("=== Simulação de Batalha de Territórios ===\n");
    
    // 1. Alocação Dinâmica
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &qtdTerritorios);

    // Usa calloc para alocar e já limpar a memória (inicializa com 0)
    mapa = (struct Territorio*) calloc(qtdTerritorios, sizeof(struct Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // 2. Cadastro
    cadastrarTerritorios(mapa, qtdTerritorios);

    // 3. Exibição Inicial
    printf("\n--- Estado Inicial do Mapa ---\n");
    exibirTerritorios(mapa, qtdTerritorios);

    // 4. Loop de Simulação de Ataques
    while (continuar) {
        printf("\n--- Rodada de Ataque ---\n");
        printf("Digite o ID do território Atacante (1 a %d): ", qtdTerritorios);
        scanf("%d", &idAtacante);
        
        printf("Digite o ID do território Defensor (1 a %d): ", qtdTerritorios);
        scanf("%d", &idDefensor);

        // Ajuste de índice (usuário digita 1, sistema usa 0)
        int idxAtk = idAtacante - 1;
        int idxDef = idDefensor - 1;

        // Validação básica dos índices
        if (idxAtk >= 0 && idxAtk < qtdTerritorios && idxDef >= 0 && idxDef < qtdTerritorios) {
            // Chama a função de ataque passando os endereços de memória dos territórios específicos
            atacar(&mapa[idxAtk], &mapa[idxDef]);
        } else {
            printf("IDs inválidos! Tente novamente.\n");
        }

        // Exibição pós-ataque
        printf("\n--- Situação Atualizada ---\n");
        exibirTerritorios(mapa, qtdTerritorios);

        printf("\nDeseja realizar outro ataque? (1-Sim / 0-Não): ");
        scanf("%d", &continuar);
    }

    // 5. Liberação de Memória
    liberarMemoria(mapa);
    printf("Simulação encerrada.\n");

    return 0;
}

// --- Implementação das Funções ---

// Função para preencher os dados do vetor dinâmico
void cadastrarTerritorios(struct Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\nCadastro do Território %d:\n", i + 1);
        
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome); // Lê string com espaços
        
        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Função para listar todos os territórios
void exibirTerritorios(struct Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("ID: %d | Nome: %-15s | Cor: %-10s | Tropas: %d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Função void atacar: simula a batalha e atualiza os ponteiros
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Validação: não pode atacar a própria cor
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: Não é possível atacar um território aliado (mesma cor)!\n");
        return;
    }

    // Simulação de dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n>>> Batalha: %s (Atq) vs %s (Def) <<<\n", atacante->nome, defensor->nome);
    printf("Resultado dos dados -> Atacante: %d | Defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Vitória do Atacante! O território foi conquistado.\n");
        
        // Atualiza a cor do defensor (acesso via ponteiro ->)
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere metade das tropas
        int tropasMovidas = atacante->tropas / 2;
        defensor->tropas = tropasMovidas;   // Defensor recebe as tropas
        atacante->tropas -= tropasMovidas;  // Atacante perde as tropas enviadas
        
        printf("%d tropas movidas para o novo território.\n", tropasMovidas);
    } else {
        printf("Defesa bem sucedida! O atacante recuou.\n");
        // Penalidade para o atacante
        if (atacante->tropas > 0) {
            atacante->tropas -= 1;
            printf("O atacante perdeu 1 unidade de tropa na batalha.\n");
        }
    }
}

// Função para limpar a memória Heap
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
    printf("Memória liberada com sucesso.\n");
}
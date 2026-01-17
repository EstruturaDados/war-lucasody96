#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Definição das Estruturas ---
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Lista de Missões Pré-definidas ---
const char* LISTA_MISSOES[] = {
    "Dominar o Mundo: Conquistar todos os territorios.",
    "General Supremo: Ter um total de 20 tropas ou mais no mapa.",
    "Estrategista: Conquistar ao menos 3 territorios (independente de quais)."
};
// Calcula o tamanho do vetor de missões
const int TOTAL_MISSOES = sizeof(LISTA_MISSOES) / sizeof(LISTA_MISSOES[0]);

// --- Protótipos das Funções ---
void configurarJogo(struct Territorio** mapa, int* qtd, char** missaoJogador, char* corJogador);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void exibirMapa(struct Territorio* mapa, int qtd);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
int verificarMissao(char* missao, struct Territorio* mapa, int qtd, char* corJogador);
void liberarMemoria(struct Territorio* mapa, char* missao);

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // Semente para números aleatórios

    struct Territorio* mapa = NULL;
    char* missaoJogador = NULL; // Ponteiro para a string da missão
    char corJogador[10];        // Armazena a cor escolhida pelo jogador
    int qtdTerritorios;
    int jogoRodando = 1;
    int idAtk, idDef;

    printf("=== WAR: SISTEMA DE MISSÕES ESTRATÉGICAS ===\n\n");

    // 1. Configuração Inicial (Alocação e Sorteio)
    configurarJogo(&mapa, &qtdTerritorios, &missaoJogador, corJogador);

    printf("\n>>> SUA MISSÃO: %s <<<\n", missaoJogador);
    printf("Você comanda o exército: %s\n", corJogador);
    printf("Pressione ENTER para começar...");
    getchar(); getchar(); // Pausa para leitura

    // 2. Loop do Jogo
    while (jogoRodando) {
        // Limpa a tela (opcional, funciona em Windows/Linux de forma simples)
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n--- TURNO ATUAL ---\n");
        printf("Missão: %s\n", missaoJogador);
        exibirMapa(mapa, qtdTerritorios);

        // Verificação de Vitória antes do ataque
        if (verificarMissao(missaoJogador, mapa, qtdTerritorios, corJogador)) {
            printf("\n************************************************\n");
            printf("PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO: %s\n", missaoJogador);
            printf("************************************************\n");
            break;
        }

        // Entrada de ataque
        printf("\nEscolha o ID do atacante (seu exército %s): ", corJogador);
        scanf("%d", &idAtk);
        printf("Escolha o ID do defensor (inimigo): ");
        scanf("%d", &idDef);

        // Ajuste de índice (usuário digita 1..N, vetor é 0..N-1)
        int idxAtk = idAtk - 1;
        int idxDef = idDef - 1;

        // Validação de índices
        if (idxAtk >= 0 && idxAtk < qtdTerritorios && idxDef >= 0 && idxDef < qtdTerritorios) {
            // Validação de regra: Só pode atacar com sua própria cor
            if (strcmp(mapa[idxAtk].cor, corJogador) != 0) {
                printf("\nErro: Você só pode atacar usando territórios da sua cor (%s)!\n", corJogador);
            } else {
                // Passagem por referência (endereço de memória) para modificar a struct original
                atacar(&mapa[idxAtk], &mapa[idxDef]);
            }
        } else {
            printf("\nErro: IDs inválidos.\n");
        }

        printf("\nPressione ENTER para continuar o próximo turno...");
        getchar(); getchar();
    }

    // 3. Limpeza de Memória
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// --- Implementação das Funções ---

void configurarJogo(struct Territorio** mapa, int* qtd, char** missaoJogador, char* corJogador) {
    // Definindo a cor do jogador
    printf("Escolha a cor do seu exército (ex: Azul, Vermelho): ");
    scanf(" %[^\n]", corJogador);

    // Definindo tamanho do mapa
    printf("Quantos territórios no mapa? (Recomendado: 5): ");
    scanf("%d", qtd);

    // Alocação dinâmica do vetor de structs
    *mapa = (struct Territorio*) calloc(*qtd, sizeof(struct Territorio));
    
    // Alocação dinâmica da string da missão 
    *missaoJogador = (char*) malloc(100 * sizeof(char));

    // Cadastro simplificado para teste
    for(int i = 0; i < *qtd; i++) {
        printf("Cadastrando Território %d:\n", i+1);
        printf("Nome: ");
        scanf(" %[^\n]", (*mapa)[i].nome);
        printf("Cor (dica: coloque '%s' em um deles para começar): ", corJogador);
        scanf(" %[^\n]", (*mapa)[i].cor);
        printf("Tropas: ");
        scanf("%d", &(*mapa)[i].tropas);
    }

    // Sorteio da missão
    atribuirMissao(*missaoJogador, LISTA_MISSOES, TOTAL_MISSOES);
}

// Sorteia uma missão e copia para a memória alocada do jogador
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    // strcpy: copia o conteúdo da string constante para a memória dinâmica do jogador
    strcpy(destino, missoes[indiceSorteado]);
}

void exibirMapa(struct Territorio* mapa, int qtd) {
    printf("\nSTATUS DO MAPA:\n");
    for(int i = 0; i < qtd; i++) {
        printf("[%d] %-15s | Cor: %-10s | Tropas: %d\n", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula o ataque usando ponteiros para alterar os valores originais
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    if (atacante->tropas <= 1) {
        printf("\nImpossível atacar: %s precisa de mais de 1 tropa.\n", atacante->nome);
        return;
    }
    
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nAção cancelada: Não pode atacar território aliado.\n");
        return;
    }

    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\nBatalha! %s (%d) vs %s (%d)\n", atacante->nome, dadoAtk, defensor->nome, dadoDef);

    if (dadoAtk > dadoDef) {
        printf("Vitória do Atacante!\n");
        
        // Mecânica de Conquista
        strcpy(defensor->cor, atacante->cor); // Troca a cor
        
        int tropasMovidas = atacante->tropas / 2;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;
        
        printf("O território %s agora pertence ao exército %s!\n", defensor->nome, atacante->cor);
    } else {
        printf("Vitória da Defesa!\n");
        atacante->tropas -= 1; // Penalidade
        printf("O atacante perdeu 1 tropa.\n");
    }
}

// Verifica se a condição da string da missão foi satisfeita no mapa
int verificarMissao(char* missao, struct Territorio* mapa, int qtd, char* corJogador) {
    int contadorTerritorios = 0;
    int contadorTropas = 0;

    // Calcula estatísticas atuais do jogador
    for(int i = 0; i < qtd; i++) {
        if(strcmp(mapa[i].cor, corJogador) == 0) {
            contadorTerritorios++;
            contadorTropas += mapa[i].tropas;
        }
    }

    // Lógica baseada na string da missão (strstr busca subtexto)
    
    // Missão 1: Dominar o Mundo (ter todos os territórios)
    if (strstr(missao, "Dominar o Mundo") != NULL) {
        if (contadorTerritorios == qtd) return 1;
    }
    // Missão 2: General Supremo (ter > 20 tropas)
    else if (strstr(missao, "General Supremo") != NULL) {
        if (contadorTropas >= 20) return 1;
    }
    // Missão 3: Estrategista (ter >= 3 territórios)
    else if (strstr(missao, "Estrategista") != NULL) {
        if (contadorTerritorios >= 3) return 1;
    }

    return 0; // Missão ainda não cumprida
}

void liberarMemoria(struct Territorio* mapa, char* missao) {
    // Importante: Liberar na ordem inversa ou independente, mas nunca esquecer
    if (mapa != NULL) free(mapa);
    if (missao != NULL) free(missao);
    printf("\nMemória liberada. Fim do programa.\n");
}
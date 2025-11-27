#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Struct ---

/**
 * @brief Estrutura que representa um cômodo (Nó) no mapa da mansão (Árvore Binária).
 * * Contém o nome da sala e ponteiros para os caminhos à esquerda e à direita (filhos).
 */
typedef struct Sala {
    char nome[50];       // Nome do cômodo
    struct Sala *esquerda; // Caminho/Sala à esquerda (Filho esquerdo)
    struct Sala *direita;  // Caminho/Sala à direita (Filho direito)
} Sala;

// --- Protótipos das Funções ---

Sala* criarSala(const char *nome);
void montarMapa(Sala **raiz);
void explorarSalas(Sala *atual);
void liberarMapa(Sala *raiz);


// --- Main (Função Principal) ---

int main() {
    Sala *hallDeEntrada = NULL;

    printf("🕵️ Detective Quest: Mapa da Mansão (Árvore Binária) 🕵️\n");
    printf("========================================================\n");
    
    // Monta a estrutura da árvore (mapa)
    montarMapa(&hallDeEntrada);

    if (hallDeEntrada != NULL) {
        printf("\n🚪 O mapa foi carregado! Iniciando a exploração...\n");
        // Inicia a navegação interativa a partir da raiz (Hall de Entrada)
        explorarSalas(hallDeEntrada);
    } else {
        printf("🚫 Erro: Não foi possível criar o mapa da mansão.\n");
    }
    
    // Libera a memória alocada dinamicamente
    liberarMapa(hallDeEntrada);
    printf("\n✅ Memória liberada. Fim do programa.\n");

    return 0;
}

// -------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES
// -------------------------------------------------------------------

/**
 * @brief Cria dinamicamente um novo nó (Sala) para a árvore.
 * @param nome O nome do cômodo.
 * @return Um ponteiro para a Sala recém-criada.
 */
Sala* criarSala(const char *nome) {
    // Alocação dinâmica de memória para a nova sala
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        perror("Erro de alocação de memória ao criar Sala");
        return NULL;
    }

    // Inicializa os campos
    strncpy(novaSala->nome, nome, 49);
    novaSala->nome[49] = '\0'; // Garantir terminação da string
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Monta a estrutura estática da árvore binária (o mapa da mansão).
 * * Esta função simula a criação automática da estrutura.
 * @param raiz Ponteiro para a raiz da árvore (Hall de Entrada).
 */
void montarMapa(Sala **raiz) {
    // Nível 0: Raiz
    *raiz = criarSala("Hall de Entrada");

    if (*raiz == NULL) return;

    // Nível 1
    (*raiz)->esquerda = criarSala("Sala de Estar"); // Esquerda do Hall
    (*raiz)->direita = criarSala("Cozinha");     // Direita do Hall

    // Verificação de sucesso na alocação
    if ((*raiz)->esquerda == NULL || (*raiz)->direita == NULL) return;

    // Nível 2
    // Sub-árvore da Sala de Estar
    (*raiz)->esquerda->esquerda = criarSala("Biblioteca");
    (*raiz)->esquerda->direita = criarSala("Jardim de Inverno"); // Folha
    
    // Sub-árvore da Cozinha
    (*raiz)->direita->esquerda = criarSala("Despensa"); // Folha
    (*raiz)->direita->direita = criarSala("Sala de Jantar");

    // Nível 3 (Folhas)
    if ((*raiz)->esquerda->esquerda != NULL) {
        // Filhos da Biblioteca (Folhas)
        (*raiz)->esquerda->esquerda->esquerda = criarSala("Quarto Principal"); // Folha
        (*raiz)->esquerda->esquerda->direita = criarSala("Banheiro");          // Folha
    }
    
    if ((*raiz)->direita->direita != NULL) {
        // Filhos da Sala de Jantar (Folhas)
        (*raiz)->direita->direita->esquerda = criarSala("Corredor de Servico"); // Folha
        // Direita da Sala de Jantar é NULL (outro tipo de folha/fim de caminho)
    }
}

/**
 * @brief Permite a exploração interativa da mansão.
 * @param atual O ponteiro para o nó (Sala) atual onde o jogador se encontra.
 */
void explorarSalas(Sala *atual) {
    char escolha;
    
    while (atual != NULL) {
        printf("\nVocê está em: ➡️ **%s**\n", atual->nome);
        
        // Verifica se é um nó-folha (fim da exploração)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("--------------------------------------------------------------------\n");
            printf("🛑 **CAMINHO FINALIZADO!** Este cômodo não possui mais saídas. Missão concluída.\n");
            printf("--------------------------------------------------------------------\n");
            break; 
        }

        printf("Escolha o próximo caminho (ou 's' para sair):\n");
        
        if (atual->esquerda != NULL) {
            printf(" [e] Esquerda: %s\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf(" [d] Direita: %s\n", atual->direita->nome);
        }
        
        printf("Sua escolha: ");
        // Leitura de um único caractere para a decisão
        if (scanf(" %c", &escolha) != 1) {
             while (getchar() != '\n'); // Limpa buffer em caso de erro
             escolha = 'i'; // Opção inválida
        }
        while (getchar() != '\n'); // Limpa o buffer de entrada

        // Lógica de navegação
        if (escolha == 's' || escolha == 'S') {
            printf("\n👋 Exploração interrompida. Saindo da mansão.\n");
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            // Move para o filho esquerdo
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            // Move para o filho direito
            atual = atual->direita;
        } else {
            printf("⚠️ Escolha inválida ou caminho bloqueado. Tente novamente.\n");
        }
    }
}

/**
 * @brief Libera recursivamente toda a memória alocada para os nós da árvore.
 * @param raiz O nó a partir do qual a liberação deve começar.
 */
void liberarMapa(Sala *raiz) {
    // Se o nó atual não é nulo, continua a liberação
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda); // Libera o caminho da esquerda
        liberarMapa(raiz->direita);  // Libera o caminho da direita
        // Libera o nó atual após a liberação dos filhos (Pós-ordem)
        free(raiz);                  
    }
}
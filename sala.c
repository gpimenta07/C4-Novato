#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definição das Structs ---

/**
 * @brief Estrutura que representa um cômodo (Nó do Mapa - Árvore Binária).
 */
typedef struct Sala {
    char nome[50];       // Nome do cômodo
    char pista[100];     // Pista associada ao cômodo (vazia se não houver)
    struct Sala *esquerda; // Caminho/Sala à esquerda
    struct Sala *direita;  // Caminho/Sala à direita
} Sala;

/**
 * @brief Estrutura que representa um nó da Pista (Nó da BST).
 */
typedef struct PistaNode {
    char conteudo[100];      // Conteúdo da pista
    struct PistaNode *esquerda; // Pistas menores (alfabeticamente)
    struct PistaNode *direita;  // Pistas maiores (alfabeticamente)
} PistaNode;

// --- Protótipos das Funções ---

// Funções do Mapa (Árvore Binária)
Sala* criarSala(const char *nome, const char *pista);
void montarMapa(Sala **raiz);

// Funções da BST de Pistas
PistaNode* criarPistaNode(const char *conteudo);
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo);
void exibirPistas(PistaNode *raiz); // Exibe em ordem alfabética (In-Ordem)
void liberarPistas(PistaNode *raiz);

// Funções de Exploração e Controle
void explorarSalasComPistas(Sala *mapa, PistaNode **pistasColetadas);
void liberarMapa(Sala *raiz);


// --- Main (Função Principal) ---

int main() {
    Sala *hallDeEntrada = NULL; // Raiz da Árvore do Mapa
    PistaNode *pistasColetadas = NULL; // Raiz da BST de Pistas

    printf("🕵️ Detective Quest: Coleta de Pistas com BST (Nível Aventureiro) 🕵️\n");
    printf("==================================================================\n");
    
    // Monta a estrutura da árvore do mapa
    montarMapa(&hallDeEntrada);

    if (hallDeEntrada != NULL) {
        printf("\n🚪 O mapa foi carregado! Iniciando a exploração no Hall de Entrada...\n");
        // Inicia a navegação interativa e a coleta
        explorarSalasComPistas(hallDeEntrada, &pistasColetadas);
    } else {
        printf("🚫 Erro: Não foi possível criar o mapa da mansão.\n");
    }
    
    // Exibe as pistas coletadas em ordem alfabética (Requisito)
    printf("\n\n------------------------------------------------------------------\n");
    printf("📜 JORNADA FINALIZADA. Pistas Coletadas em Ordem Alfabética (BST):\n");
    printf("------------------------------------------------------------------\n");
    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasColetadas);
    }
    
    // Libera toda a memória alocada
    liberarMapa(hallDeEntrada);
    liberarPistas(pistasColetadas);
    printf("\n✅ Memória do mapa e das pistas liberada. Fim do programa.\n");

    return 0;
}

// -------------------------------------------------------------------
// FUNÇÕES DO MAPA (ÁRVORE BINÁRIA)
// -------------------------------------------------------------------

/**
 * @brief Cria dinamicamente um novo cômodo (Sala) com uma pista.
 * @param nome O nome do cômodo.
 * @param pista O conteúdo da pista (use "" se não houver).
 * @return Um ponteiro para a Sala recém-criada.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        perror("Erro de alocação de memória (Sala)");
        return NULL;
    }

    strncpy(novaSala->nome, nome, 49);
    strncpy(novaSala->pista, pista, 99);
    novaSala->nome[49] = '\0';
    novaSala->pista[99] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Monta a estrutura estática do mapa com pistas fixas.
 */
void montarMapa(Sala **raiz) {
    // Nível 0: Raiz (Hall de Entrada)
    *raiz = criarSala("Hall de Entrada", "Pegadas de lama frescas foram deixadas aqui.");

    if (*raiz == NULL) return;

    // Nível 1
    (*raiz)->esquerda = criarSala("Sala de Estar", "Há uma xícara de café quente na mesa.");
    (*raiz)->direita = criarSala("Cozinha", ""); // Sem pista

    // Nível 2
    // Sub-árvore da Sala de Estar
    (*raiz)->esquerda->esquerda = criarSala("Biblioteca", "Um livro de Sherlock Holmes está aberto.");
    (*raiz)->esquerda->direita = criarSala("Jardim de Inverno", "As janelas estão trancadas por dentro."); // Folha
    
    // Sub-árvore da Cozinha
    (*raiz)->direita->esquerda = criarSala("Despensa", "Uma faca de prata está faltando no conjunto."); // Folha
    (*raiz)->direita->direita = criarSala("Sala de Jantar", "O lustre está ligeiramente torto.");

    // Nível 3 (Folhas)
    if ((*raiz)->esquerda->esquerda != NULL) {
        (*raiz)->esquerda->esquerda->esquerda = criarSala("Quarto Principal", "Um relógio de bolso parou às 03:00."); // Folha
        (*raiz)->esquerda->esquerda->direita = criarSala("Banheiro", ""); // Folha (Sem pista)
    }
    
    if ((*raiz)->direita->direita != NULL) {
        (*raiz)->direita->direita->esquerda = criarSala("Corredor de Servico", "O alarme está desligado."); // Folha
        // Direita da Sala de Jantar é NULL
    }
}

/**
 * @brief Libera recursivamente a memória do mapa.
 */
void liberarMapa(Sala *raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda); 
        liberarMapa(raiz->direita);  
        free(raiz);                  
    }
}


// -------------------------------------------------------------------
// FUNÇÕES DA BST DE PISTAS
// -------------------------------------------------------------------

/**
 * @brief Cria dinamicamente um novo nó de pista.
 */
PistaNode* criarPistaNode(const char *conteudo) {
    PistaNode *novoNo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        perror("Erro de alocação de memória (PistaNode)");
        return NULL;
    }
    strncpy(novoNo->conteudo, conteudo, 99);
    novoNo->conteudo[99] = '\0';
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

/**
 * @brief Insere uma nova pista na BST, mantendo a ordem alfabética (recursiva).
 * @param raiz A raiz da sub-árvore atual.
 * @param conteudo O texto da pista a ser inserida.
 * @return O ponteiro para a raiz da sub-árvore (atualizada).
 */
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo) {
    // 1. Caso Base: Árvore vazia ou sub-árvore vazia. Cria e retorna o novo nó.
    if (raiz == NULL) {
        printf("   (Pista inserida: '%s')\n", conteudo);
        return criarPistaNode(conteudo);
    }
    
    // Compara alfabeticamente
    int cmp = strcmp(conteudo, raiz->conteudo);
    
    if (cmp < 0) {
        // Pista é alfabeticamente menor, vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (cmp > 0) {
        // Pista é alfabeticamente maior, vai para a direita
        raiz->direita = inserirPista(raiz->direita, conteudo);
    } else {
        // cmp == 0: Pista já existe (evita duplicação)
        printf("   (Pista JÁ EXISTE: '%s' ignorada.)\n", conteudo);
    }
    
    // Retorna a raiz inalterada (se não for o caso base)
    return raiz;
}

/**
 * @brief Exibe todas as pistas da BST em ordem alfabética (Travessia In-Ordem).
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);  // Visita Esquerda (Pistas menores)
        printf("   -> %s\n", raiz->conteudo); // Visita Raiz (Pista atual)
        exibirPistas(raiz->direita);   // Visita Direita (Pistas maiores)
    }
}

/**
 * @brief Libera recursivamente a memória da BST.
 */
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// -------------------------------------------------------------------
// FUNÇÃO DE EXPLORAÇÃO COM COLETA
// -------------------------------------------------------------------

/**
 * @brief Controla a navegação interativa e a coleta de pistas.
 * @param mapa O ponteiro para o nó (Sala) atual do mapa.
 * @param pistasColetadas O ponteiro para a raiz da BST de pistas (passado por referência).
 */
void explorarSalasComPistas(Sala *mapa, PistaNode **pistasColetadas) {
    char escolha;
    Sala *atual = mapa;
    
    while (atual != NULL) {
        printf("\nVocê está em: ➡️ **%s**\n", atual->nome);

        // --- Lógica de Coleta de Pista (Requisito) ---
        if (strlen(atual->pista) > 0) {
            printf("🔍 **PISTA ENCONTRADA:** %s\n", atual->pista);
            // Insere a pista na BST
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
            // Limpa a pista da sala para não coletar novamente
            atual->pista[0] = '\0'; 
        } else {
            printf("🚫 Nenhuma pista nova encontrada neste cômodo.\n");
        }
        
        // Verifica caminhos disponíveis
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🛑 **Nó-Folha:** Este cômodo não possui mais saídas. Retorne ou saia.\n");
        }

        // --- Menu de Navegação ---
        printf("\nEscolha o próximo caminho:\n");
        if (atual->esquerda != NULL) {
            printf(" [e] Esquerda: %s\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf(" [d] Direita: %s\n", atual->direita->nome);
        }
        printf(" [s] Sair da mansão e analisar as pistas.\n");
        
        printf("Sua escolha: ");
        if (scanf(" %c", &escolha) != 1) { escolha = 'i'; }
        while (getchar() != '\n');

        // Lógica de navegação
        if (escolha == 's' || escolha == 'S') {
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("⚠️ Escolha inválida ou caminho bloqueado. Tente novamente.\n");
        }
    }
}
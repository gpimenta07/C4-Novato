#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes para a Tabela Hash ---
#define TAMANHO_HASH 7 // Tamanho primo para a tabela hash

// --- Structs ---

/**
 * @brief Estrutura para um cômodo da mansão (Nó do Mapa).
 */
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista estática associada à sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * @brief Estrutura para uma pista coletada (Nó da BST).
 */
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/**
 * @brief Estrutura para um item da Tabela Hash (Lista Encadeada na posição).
 */
typedef struct HashNode {
    char pista[100];      // Chave: Pista (para BST)
    char suspeito[50];    // Valor: Suspeito associado
    struct HashNode *proximo;
} HashNode;

// --- Variável Global para a Tabela Hash ---
HashNode* tabelaHash[TAMANHO_HASH];

// --- Protótipos das Funções ---

// Hash
int hashFunction(const char *chave);
void inicializarHash();
void inserirNaHash(const char *pista, const char *suspeito);
const char* encontrarSuspeito(const char *pista);
void preencherHashSuspeitos();

// Mapa e Exploração
Sala* criarSala(const char *nome, const char *pista);
void montarMapa(Sala **raiz);
void explorarSalas(Sala *mapa, PistaNode **pistasColetadas);
void liberarMapa(Sala *raiz);

// BST de Pistas
PistaNode* criarPistaNode(const char *conteudo);
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo);
void exibirPistas(PistaNode *raiz); // Travessia In-Ordem
void liberarPistas(PistaNode *raiz);

// Finalização do Jogo
void verificarSuspeitoFinal(PistaNode *pistasColetadas);


// --- Main (Função Principal) ---

int main() {
    Sala *hallDeEntrada = NULL; 
    PistaNode *pistasColetadas = NULL; 

    printf("👑 DETECTIVE QUEST: INVESTIGAÇÃO FINAL (Nível Mestre) 👑\n");
    printf("========================================================\n");
    
    // Inicialização e Preparação
    inicializarHash();
    preencherHashSuspeitos(); // Associa as pistas aos suspeitos
    montarMapa(&hallDeEntrada);

    if (hallDeEntrada != NULL) {
        printf("\n🚪 O mapa foi carregado! Iniciando a exploração no Hall de Entrada...\n");
        // Inicia a navegação interativa e coleta de pistas
        explorarSalas(hallDeEntrada, &pistasColetadas);
    } else {
        printf("🚫 Erro ao iniciar a simulação.\n");
    }
    
    // Fase de Acusação Final (Requisito)
    verificarSuspeitoFinal(pistasColetadas);
    
    // Libera a memória
    liberarMapa(hallDeEntrada);
    liberarPistas(pistasColetadas);
    // Liberação da Hash (Opcional, mas boa prática)
    // (A liberação da Hash é complexa e omitida aqui por simplificação, mas deveria ser implementada)
    
    printf("\n✅ Memória liberada. Fim do programa.\n");

    return 0;
}

// -------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES HASH (Suspeitos)
// -------------------------------------------------------------------

/**
 * @brief Função de hash simples (soma dos valores ASCII mod TAMANHO_HASH).
 * @param chave A string da pista.
 * @return O índice da tabela hash.
 */
int hashFunction(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma = soma + chave[i];
    }
    return soma % TAMANHO_HASH;
}

/**
 * @brief Inicializa a Tabela Hash com NULL.
 */
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

/**
 * @brief Insere a associação Pista (Chave) -> Suspeito (Valor) na Tabela Hash.
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = hashFunction(pista);
    
    // Cria novo nó da lista encadeada (Tratamento de Colisão)
    HashNode* novoNo = (HashNode*) malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        perror("Erro ao alocar HashNode");
        return;
    }
    
    strncpy(novoNo->pista, pista, 99);
    strncpy(novoNo->suspeito, suspeito, 49);
    novoNo->pista[99] = '\0';
    novoNo->suspeito[49] = '\0';
    
    // Insere no início da lista encadeada na posição [indice]
    novoNo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNo;
}

/**
 * @brief Consulta a Tabela Hash para encontrar o suspeito associado a uma pista.
 * @return O nome do suspeito ou "Desconhecido" se não for encontrado.
 */
const char* encontrarSuspeito(const char *pista) {
    int indice = hashFunction(pista);
    HashNode* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Suspeito encontrado
        }
        atual = atual->proximo;
    }
    return "Nenhum Suspeito Associado"; // Pista sem suspeito conhecido na Hash
}

/**
 * @brief Define as associações Pista -> Suspeito para a investigação.
 * * Lógica do Jogo: 2 pistas para cada suspeito, e uma pista neutra.
 */
void preencherHashSuspeitos() {
    // Pistas para Sustentar Acusação
    inserirNaHash("Pegadas de lama frescas foram deixadas aqui.", "Mordomo");
    inserirNaHash("Um livro de Sherlock Holmes está aberto.", "Mordomo");
    
    inserirNaHash("Há uma xícara de café quente na mesa.", "Herdeiro");
    inserirNaHash("O alarme está desligado.", "Herdeiro");
    
    inserirNaHash("Uma faca de prata está faltando no conjunto.", "A Sogra");
    inserirNaHash("Um relógio de bolso parou às 03:00.", "A Sogra");
    
    // Pista Neutra
    inserirNaHash("As janelas estão trancadas por dentro.", "Neutro"); 
    inserirNaHash("O lustre está ligeiramente torto.", "Neutro");
    
    printf("📖 Mapa de Suspeitos (Tabela Hash) Carregado. 3 Suspeitos Chave.\n");
}


// -------------------------------------------------------------------
// FUNÇÕES DO MAPA (ÁRVORE BINÁRIA) E EXPLORAÇÃO
// -------------------------------------------------------------------

/**
 * @brief Cria dinamicamente um novo cômodo (Sala) com uma pista.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) return NULL;
    
    strncpy(novaSala->nome, nome, 49);
    strncpy(novaSala->pista, pista, 99);
    novaSala->nome[49] = '\0';
    novaSala->pista[99] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Monta a estrutura estática do mapa com pistas.
 */
void montarMapa(Sala **raiz) {
    *raiz = criarSala("Hall de Entrada", "Pegadas de lama frescas foram deixadas aqui.");
    if (*raiz == NULL) return;

    (*raiz)->esquerda = criarSala("Sala de Estar", "Há uma xícara de café quente na mesa.");
    (*raiz)->direita = criarSala("Cozinha", "O lustre está ligeiramente torto.");

    if ((*raiz)->esquerda != NULL) {
        (*raiz)->esquerda->esquerda = criarSala("Biblioteca", "Um livro de Sherlock Holmes está aberto.");
        (*raiz)->esquerda->direita = criarSala("Jardim de Inverno", "As janelas estão trancadas por dentro.");
    }
    
    if ((*raiz)->direita != NULL) {
        (*raiz)->direita->esquerda = criarSala("Despensa", "Uma faca de prata está faltando no conjunto.");
        (*raiz)->direita->direita = criarSala("Sala de Jantar", "O alarme está desligado.");
    }

    if ((*raiz)->esquerda != NULL && (*raiz)->esquerda->esquerda != NULL) {
        (*raiz)->esquerda->esquerda->esquerda = criarSala("Quarto Principal", "Um relógio de bolso parou às 03:00.");
        (*raiz)->esquerda->esquerda->direita = criarSala("Banheiro", "");
    }
}

/**
 * @brief Controla a navegação interativa e a coleta de pistas.
 */
void explorarSalas(Sala *mapa, PistaNode **pistasColetadas) {
    char escolha;
    Sala *atual = mapa;
    
    while (atual != NULL) {
        printf("\nVocê está em: ➡️ **%s**\n", atual->nome);

        // Lógica de Coleta de Pista
        if (strlen(atual->pista) > 0) {
            printf("🔍 **PISTA ENCONTRADA:** %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
            atual->pista[0] = '\0'; // Limpa a pista da sala
        } else {
            printf("🚫 Nenhuma pista nova encontrada neste cômodo.\n");
        }
        
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🛑 **Nó-Folha:** Este cômodo não possui mais saídas.\n");
        }

        printf("\nEscolha o próximo caminho:\n");
        if (atual->esquerda != NULL) { printf(" [e] Esquerda: %s\n", atual->esquerda->nome); }
        if (atual->direita != NULL) { printf(" [d] Direita: %s\n", atual->direita->nome); }
        printf(" [s] Sair da mansão e analisar as pistas.\n");
        
        printf("Sua escolha: ");
        if (scanf(" %c", &escolha) != 1) { escolha = 'i'; }
        while (getchar() != '\n');

        if (escolha == 's' || escolha == 'S') { break; } 
        else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) { atual = atual->esquerda; } 
        else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) { atual = atual->direita; } 
        else { printf("⚠️ Escolha inválida ou caminho bloqueado. Tente novamente.\n"); }
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
    if (novoNo == NULL) return NULL;
    
    strncpy(novoNo->conteudo, conteudo, 99);
    novoNo->conteudo[99] = '\0';
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

/**
 * @brief Insere uma nova pista na BST (recursiva).
 */
PistaNode* inserirPista(PistaNode *raiz, const char *conteudo) {
    if (raiz == NULL) {
        printf("   (Pista inserida: '%s')\n", conteudo);
        return criarPistaNode(conteudo);
    }
    
    int cmp = strcmp(conteudo, raiz->conteudo);
    
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, conteudo);
    }
    return raiz;
}

/**
 * @brief Exibe todas as pistas da BST em ordem alfabética (Travessia In-Ordem).
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("   -> %s\n", raiz->conteudo);
        exibirPistas(raiz->direita);
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
// FUNÇÃO FINAL DE JULGAMENTO (Verificação e Acusação)
// -------------------------------------------------------------------

/**
 * @brief Função auxiliar para percorrer a BST de pistas e contar evidências.
 */
void contarPistasPorSuspeito(PistaNode *raiz, const char *suspeitoAlvo, int *contador) {
    if (raiz != NULL) {
        contarPistasPorSuspeito(raiz->esquerda, suspeitoAlvo, contador);
        
        // Verifica a associação da pista atual
        const char *suspeitoAssociado = encontrarSuspeito(raiz->conteudo);
        if (strcmp(suspeitoAssociado, suspeitoAlvo) == 0) {
            (*contador)++;
            printf("   [EVIDÊNCIA] Pista '%s' aponta para %s.\n", raiz->conteudo, suspeitoAlvo);
        }
        
        contarPistasPorSuspeito(raiz->direita, suspeitoAlvo, contador);
    }
}

/**
 * @brief Conduz a fase de julgamento, solicitando a acusação e verificando as evidências.
 */
void verificarSuspeitoFinal(PistaNode *pistasColetadas) {
    char suspeitoAcusado[50];
    int evidencias = 0;
    
    printf("\n\n------------------------------------------------------------------\n");
    printf("📜 FASE FINAL: ANÁLISE DAS EVIDÊNCIAS\n");
    printf("------------------------------------------------------------------\n");
    
    if (pistasColetadas == NULL) {
        printf("🚫 Nenhuma pista foi coletada. Impossível fazer uma acusação fundamentada.\n");
        printf("-> RESULTADO: O culpado escapou por falta de provas.\n");
        return;
    }
    
    printf("Pistas coletadas (em ordem alfabética):\n");
    exibirPistas(pistasColetadas);
    
    printf("\nSuspeitos Chave: Mordomo, Herdeiro, A Sogra.\n");
    printf("Digite o nome do **CULPADO** que você acusa: ");
    scanf("%49s", suspeitoAcusado);
    while (getchar() != '\n');
    
    printf("\n⏳ Verificando as evidências contra %s...\n", suspeitoAcusado);
    
    // Conta quantas pistas coletadas apontam para o suspeito acusado (recursivamente na BST)
    contarPistasPorSuspeito(pistasColetadas, suspeitoAcusado, &evidencias);
    
    printf("\n--- VEREDICTO ---\n");
    printf("Total de evidências contra %s: %d\n", suspeitoAcusado, evidencias);
    
    // Requisito: Verificar se pelo menos duas pistas sustentam a acusação
    if (evidencias >= 2) {
        printf("🎉 **SUCESSO!** Evidências suficientes (%d pistas) sustentam sua acusação contra %s.\n", evidencias, suspeitoAcusado);
        printf("-> RESULTADO: O culpado foi capturado. A investigação está encerrada!\n");
    } else {
        printf("😭 **FRACASSO!** Apenas %d evidência(s) encontrada(s). Você precisa de no mínimo 2.\n", evidencias);
        printf("-> RESULTADO: A acusação é insustentável. O verdadeiro culpado escapou!\n");
    }
}
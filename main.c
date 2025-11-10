#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct {
    char cor[10];
    char* missao;
} Jogador;

Territorio* alocarTerritorios(int n) {
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (!mapa) { printf("Erro de alocação!\n"); exit(1); }
    return mapa;
}

Jogador* alocarJogadores(int num) {
    Jogador* j = (Jogador*) malloc(num * sizeof(Jogador));
    if (!j) { printf("Erro de alocação!\n"); exit(1); }
    return j;
}

void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("Território %d - Nome: ", i+1);
        scanf("%29s", mapa[i].nome);
        printf("Cor: ");
        scanf("%9s", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirTerritorios(Territorio* mapa, int n) {
    printf("\nÍndice | Nome         | Cor     | Tropas\n");
    printf("-------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%6d | %-12s | %-7s | %6d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("\n");
}

void atribuirMissao(char* destino, char* missoes[], int total) {
    strcpy(destino, missoes[rand() % total]);
}

int verificarMissao(char* missao, char* cor, Territorio* mapa, int n) {
    if (strcmp(missao, "Controlar pelo menos 3 territórios") == 0) {
        int c = 0; for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, cor) == 0) c++;
        return c >= 3;
    }
    if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {
        for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropas > 0) return 0;
        return 1;
    }
    if (strcmp(missao, "Eliminar todas as tropas da cor azul") == 0) {
        for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, "azul") == 0 && mapa[i].tropas > 0) return 0;
        return 1;
    }
    if (strcmp(missao, "Ter mais de 20 tropas no total") == 0) {
        int t = 0; for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, cor) == 0) t += mapa[i].tropas;
        return t > 20;
    }
    if (strcmp(missao, "Controlar pelo menos 5 territórios") == 0) {
        int c = 0; for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, cor) == 0) c++;
        return c >= 5;
    }
    return 0;
}

void atacar(Territorio* a, Territorio* d) {
    int da = rand() % 6 + 1, dd = rand() % 6 + 1;
    printf("Dado atacante: %d | Defensor: %d\n", da, dd);
    if (da > dd) {
        printf("Conquista!\n");
        strcpy(d->cor, a->cor);
        int metade = a->tropas / 2;
        d->tropas = metade;
        a->tropas -= metade;
    } else {
        printf("Defesa bem-sucedida!\n");
        if (a->tropas > 0) a->tropas--;
    }
}

void liberarMemoria(Territorio* mapa, Jogador* jogadores, int nj) {
    free(mapa);
    for (int i = 0; i < nj; i++) free(jogadores[i].missao);
    free(jogadores);
}

int main() {
    srand(time(NULL));

    char* missoes[] = {
        "Controlar pelo menos 3 territórios",
        "Eliminar todas as tropas da cor vermelha",
        "Eliminar todas as tropas da cor azul",
        "Ter mais de 20 tropas no total",
        "Controlar pelo menos 5 territórios"
    };
    int totalM = 5;

    int n, nj;
    printf("Número de territórios: "); scanf("%d", &n);
    Territorio* mapa = alocarTerritorios(n);
    cadastrarTerritorios(mapa, n);

    printf("Número de jogadores: "); scanf("%d", &nj);
    Jogador* jogadores = alocarJogadores(nj);

    for (int i = 0; i < nj; i++) {
        printf("Cor do jogador %d: ", i+1); scanf("%9s", jogadores[i].cor);
        jogadores[i].missao = malloc(100);
        atribuirMissao(jogadores[i].missao, missoes, totalM);
        printf("Missão de %s: %s\n", jogadores[i].cor, jogadores[i].missao);
    }

    exibirTerritorios(mapa, n);

    int turno = 0, vencedor = 0;
    while (!vencedor) {
        int j = turno % nj;
        printf("\n--- Turno do jogador %s ---\n", jogadores[j].cor);

        int a, d;
        printf("Atacante (índice): "); scanf("%d", &a);
        printf("Defensor (índice): "); scanf("%d", &d);

        if (a < 0 || a >= n || d < 0 || d >= n || a == d || 
            strcmp(mapa[a].cor, jogadores[j].cor) != 0 || 
            strcmp(mapa[a].cor, mapa[d].cor) == 0 || 
            mapa[a].tropas < 2) {
            printf("Ataque inválido! Turno perdido.\n");
            turno++; continue;
        }

        printf("Ataque: %s -> %s\n", mapa[a].nome, mapa[d].nome);
        atacar(&mapa[a], &mapa[d]);
        exibirTerritorios(mapa, n);

        for (int k = 0; k < nj; k++) {
            if (verificarMissao(jogadores[k].missao, jogadores[k].cor, mapa, n)) {
                printf(">>> Jogador %s VENCEU! Missão: %s <<<\n", jogadores[k].cor, jogadores[k].missao);
                vencedor = 1; break;
            }
        }
        turno++;
    }

    liberarMemoria(mapa, jogadores, nj);
    return 0;
}

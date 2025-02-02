/*  BIBLIOTECAS UTILIZADAS */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "BibliotecaRegistro.h"

/* CONSTANTES PRÉ-DEFINIDAS */

/* PROTÓTIPOS DE FUNÇÕES */
int main();
void sortearPalavras(void);
void sortearPremio(void);
void layoutPalavras (void);
void mostrarPontuacao(int vencedor);
void exibirPalavras(void);
int verif_attLetra(char letra);
int contLetrasRest(void);
void verif_PalCompleta(int *jogadorAtual, int totalJogadores);
int exibirVencedor(void);
void turnoJogador(int *jogadorAtual, int totalJogadores);

/* VARIÁVEIS GLOBAIS */
FILE *Arq;
char palavraSegredo[3][16+1];
int i, tamanho, contador;
char letraDigitada;
int fim_doJogo = 0;

/* ===== CONSTRUÇÃO DAS FUNÇÕES ===== */
void sortearPalavras(void)
{
    /* DECLARAÇÃO DE VARIÁVEIS */
    int totalRegistros, indsorteado;

    /* ABRIR O ARQUIVO */
    Arq = fopen("PALAVRAS.DAT", "r+b");
    if (Arq==NULL)
    {
        printf("\nOcorreu um erro ao abrir o arquivo PALAVRAS.DAT! O programa sera encerrado.");
        exit(0);
    }

    /* CALCULAR O NUMERO DE REGISTROS */
    fseek(Arq, 0, SEEK_END); /* move o ponteiro do arquivo para o final */
    totalRegistros = ftell(Arq) / sizeof(registro); /* retorna a posição atual do ponteiro, que é igual ao tamanho do arquivo em bytes.*/

    /* VERIFICAR SE O ARQUIVO ESTA VAZIO */
    if (totalRegistros == 0)
    {
        printf("\nNão ha palavras disponiveis para sorteio");
        fclose(Arq);
        return;
    }

    /* SORTEAR UM INDICE */
    srand(time(NULL)); /* inicializa o gerador de num aleatorios com o tempo atual para garantir sorteios diferentes */
    indsorteado = rand() % totalRegistros; /* gera um num aleatorio entre 0 totalRegistros - 1. */

    /* LER O REGISTRO SORTEADO */
    fseek(Arq, indsorteado * sizeof(registro), SEEK_SET); /* move o ponteiro do arquivo para a posição correspondente ao registro sorteado */
    fread(&reg, sizeof(registro), 1, Arq); /* le o registro sorteado e o armazena na variavel reg */

    fclose(Arq);
}

void sortearPremio(void)
{
    int totalRegistros, indsorteado;

    Arq = fopen("PREMIOS.DAT", "r+b");
    if (Arq==NULL)
    {
        printf("\nOcorreu um erro ao abrir o arquivo PREMIOS.DAT! O programa sera encerrado.");
        exit(0);
    }

    fseek(Arq, 0, SEEK_END);
    totalRegistros = ftell(Arq) / sizeof(registropremio);

    if (totalRegistros == 0)
    {
        printf("\nNenhum premio disponivel para sorteio.");
        fclose(Arq);
        return;
    }

    srand(time(NULL));
    indsorteado = rand() % totalRegistros;

    fseek(Arq, indsorteado * sizeof(registropremio), SEEK_SET);
    fread(&regpr, sizeof(registropremio), 1, Arq);

    fclose(Arq);
}

void layoutPalavras(void)
{
    printf("\n                    A palavra esta associada com: %s                      \n\n", reg.pista);
    for(int j=0;j<reg.qtd; j++)
    {
        tamanho = strlen(reg.palavras[j]);

        for(int k=0;k<tamanho; k++)
            palavraSegredo[j][k] = '-';

        palavraSegredo[j][tamanho] = '\0';

        printf("\t\t\t\t ");
        for(int k=0;k<tamanho;k++)
            printf("- ");
        printf("\n\n");
    }
}

void exibirPalavras(void)
{
    printf("\n                    A(s) palavra esta associada com: %s                      \n\n", reg.pista);

    for(int j=0; j<reg.qtd;j++)
    {
        tamanho = strlen(reg.palavras[j]);

        printf("\t\t\t\t ");
        for(int k=0; k<tamanho; k++)
        {
            printf("%c ", palavraSegredo[j][k]);
        }
        printf("\n\n");
    }
}

void mostrarPontuacao(int vencedor)
{

    printf("    Jogador 1\t\t\t\tJogador 2\t\t\t\tJogador 3\n");
    printf("    ============\t\t\t============\t\t\t\t============\n");
    printf("    R$ %5.2f\t\t\t\tR$ %5.2f\t\t\t\tR$ %5.2f\n", jogador[0].pontuacao_jogador, jogador[1].pontuacao_jogador, jogador[2].pontuacao_jogador);

    if (vencedor == 0)
    {
        printf("\n     VENCEDOR!");
    }

    if (vencedor == 1)
    {
        printf("                                        VENCEDOR!");
    }

    if (vencedor == 2)
    {
        printf("                                                                                VENCEDOR!");
    }

}

int verif_attLetra(char letra)
{
    int cont=0;

    /* VERIFICA TODAS AS PALAVRAS */
    for (i=0;i<reg.qtd;i++)
    {
        for(int j=0;j<strlen(reg.palavras[i]); j++)
        {
            if(tolower(reg.palavras[i][j]) == tolower(letra) && palavraSegredo[i][j] == '-')
            {
                palavraSegredo[i][j] = reg.palavras[i][j];
                cont++;
            }
        }
    }

    return cont;
}

int contLetrasRest(void)
{
    int restante = 0;

    for (int i = 0; i < reg.qtd; i++) {
        for (int j = 0; j < strlen(reg.palavras[i]); j++) {
            if (palavraSegredo[i][j] == '-') {
                restante++;
            }
        }
    }

    return restante;
}

void verif_PalCompleta(int *jogadorAtual, int totalJogadores)
{
    char tentativa[100];
    char palavras_corretas[100] = "";
    int tentativas_restantes = 3; // 3 jogadores terão chance

    // Preparar string com todas as palavras corretas
    for(int i = 0; i < reg.qtd; i++) {
        strcat(palavras_corretas, reg.palavras[i]);
        if(i != reg.qtd - 1) strcat(palavras_corretas, " ");
    }

    // Loop para dar chance a todos os jogadores
    for(int t = 0; t < totalJogadores; t++) {
        system("cls");
        exibirPalavras();
        mostrarPontuacao(-1);

        printf("\nFaltam %d letras!\n", contLetrasRest());
        printf("\nJogador %d, voce tem 5 segundos para pensar...\n", *jogadorAtual + 1);

        // Contagem regressiva
        for(int i = 5; i > 0; i--) {
            printf("%d...", i);
            fflush(stdout);
            sleep(1);
        }

        // Capturar tentativa
        printf("\nDigite TODAS as palavras (separadas por espaco): ");
        fflush(stdin);
        fgets(tentativa, sizeof(tentativa), stdin);
        tentativa[strcspn(tentativa, "\n")] = '\0';

        // Comparar (case insensitive)
        if(strcasecmp(tentativa, palavras_corretas) == 0) {
            jogador[*jogadorAtual].pontuacao_jogador += regpr.premio;
           // printf("\nACERTOU! Pontuacao total: R$ %.2f\n", jogador[*jogadorAtual].pontuacao_jogador);
            fim_doJogo = 1;
            system("cls");
            int vencedor = exibirVencedor();
            exibirPalavras();
            mostrarPontuacao(vencedor);
            return;
        } else {
            if(t < totalJogadores - 1) {
                printf("Proximo jogador...\n");
                *jogadorAtual = (*jogadorAtual + 1) % totalJogadores;
                getche();
            }
        }
    }

    // Se nenhum acertou
    printf("\nNenhum jogador acertou! Fim do jogo.\n");
    fim_doJogo = 1;
    int vencedor = exibirVencedor();
    system("cls");
    exibirPalavras();
    mostrarPontuacao(vencedor);

}

int exibirVencedor(void)
{
    int vencedor = 0;
    float maiorPont = jogador[0].pontuacao_jogador;

    for(i=1; i<3; i++)
    {
        if(jogador[i].pontuacao_jogador > maiorPont)
        {
            maiorPont = jogador[i].pontuacao_jogador;
            vencedor = i;
        }
    }

    if (maiorPont == 0)
    {
        return -1;
    } else {
        return vencedor;
    }

}

void turnoJogador(int *jogadorAtual, int totalJogadores)
{

    system ("cls"); /*system ("clear");*/
    exibirPalavras();
    mostrarPontuacao(-1);
    printf("\n\nEsta e a vez do jogador %d", *jogadorAtual+1);
    printf("\n\nRODA A RODA!");
    getch();
    sortearPremio();

    if(contLetrasRest() <= 3)
    {
        verif_PalCompleta(jogadorAtual, totalJogadores);
        return;
    }

    if (fabs(regpr.premio - 0.00) < 0.001)
    {
        printf("\n\nPASSA A VEZ!");
        printf("\n\nA vez sera passada para o proximo jogador.");
        *jogadorAtual = (*jogadorAtual+1) % totalJogadores;
        getche();
        return;
    } else if (fabs(regpr.premio - 0.01) < 0.001)
        {
            printf("\n\nPERDE TUDO!");
            printf("\n\nA vez sera passada para o proximo Jogador.");
            jogador[*jogadorAtual].pontuacao_jogador = 0; /* zerar pontos */
            *jogadorAtual = (*jogadorAtual +1) % totalJogadores;
            getche();
            return;
        }
     else {
        do
        {
            printf("\n\nUma letra por R$ %.2f: ", regpr.premio);
            fflush(stdin);
            letraDigitada = getche();
            printf("\n");

            if (letraDigitada > 'z' || letraDigitada < 'a' && letraDigitada > 'Z' || letraDigitada < 'A')
            {
                printf("\n\nCarctere Invalido! Digite apenas letras.\n");
            } else {
                break;
            }
        } while(1);

        int acertos = verif_attLetra(letraDigitada);

        if(acertos > 0)
        {
            jogador[*jogadorAtual].pontuacao_jogador += regpr.premio * acertos;
        } else
        {
           printf("\nNao existe letra %c na palavra.", letraDigitada);
           printf("\nA vez sera passada para o proximo jogador.");
           getche();
           system ("cls"); /*system ("clear");*/
           *jogadorAtual = (*jogadorAtual + 1) % totalJogadores;
        }

        system ("cls"); /*system ("clear");*/
        exibirPalavras();

     }
}




/* ===== CORPO DO PROGRAMA ===== */
int main()
{
    int jogadorAtual = 0;
    int totalJogadores = 3;

    system ("cls"); /*system ("clear");*/
    sortearPalavras();
    printf("\n                    A palavra esta associada com: %s                      \n\n", reg.pista);

    printf("    Jogador 1\t\t\t\tJogador 2\t\t\tJogador 3\n");
    printf("    ============\t\t\t============\t\t\t============\n");
    printf("    R$ 0,00\t\t\t\tR$ 0,00\t\t\t\tR$ 0,00\n");

    printf("\n\nDigite qualquer tecla para continuar!");
    getche();

    system ("cls"); /*system ("clear");*/

    for (i=0; i<3; i++) /* Inicializando jogadores */
    {
        jogador[i].pontuacao_jogador = 0;
    }

    layoutPalavras();
    mostrarPontuacao(-1);

    while(!fim_doJogo)
    {
        turnoJogador(&jogadorAtual, totalJogadores);
    }

    /*for(i=0;i<3;i++)
    {
        turnoJogador(i);
        mostrarPontuacao();
    }*/





    getch();
    return 0;

}

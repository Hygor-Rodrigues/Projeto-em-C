#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

typedef struct {
    char nome[30];
    char placa[10];
    int vaga;
} reg;

void limpa_Buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int tamanho(FILE *arq) {
    long pos = ftell(arq);
    fseek(arq, 0, SEEK_END);
    long fim = ftell(arq);
    fseek(arq, pos, SEEK_SET);
    return (int)(fim / sizeof(reg));
}

void cadastrar(FILE *arq) {
    reg r;

    printf("\n=== Cadastrar ===\n");
    printf("Nome do proprietário: ");
    fgets(r.nome, sizeof(r.nome), stdin);
    r.nome[strcspn(r.nome, "\n")] = '\0';

    printf("Placa do carro: ");
    fgets(r.placa, sizeof(r.placa), stdin);
    r.placa[strcspn(r.placa, "\n")] = '\0';

    printf("Vaga: ");
    scanf("%d", &r.vaga);
    limpa_Buffer();

    fseek(arq, 0, SEEK_END);
    fwrite(&r, sizeof(reg), 1, arq);

    printf("\nRegistro salvo com sucesso!\n");
}

void consultar(FILE *arq) {
    int vaga;
    reg r;
    int ref = 0;
    int total = tamanho(arq);
    if (total == 0) {
        printf("\nNenhum registro cadastrado.\n");
        return;
    }

    printf("\nDigite o número da vaga que deseja consultar: ");
    scanf("%d", &vaga);
    limpa_Buffer();

    fseek(arq, 0, SEEK_SET);

    for (int i = 0; i < total; i++) {
        fread(&r, sizeof(reg), 1, arq);
        if (r.vaga == vaga) {
            ref = 1;

            printf("\n=== Dados da vaga ===\n");
            printf("Proprietário: %s\n", r.nome);
            printf("Placa: %s\n", r.placa);
            printf("Vaga: %d\n", r.vaga);
            break;
        }
    }

    if (!ref) {
        printf("\nNenhum registro encontrado para essa vaga.\n");
    }
}

int main() {
    setlocale(LC_ALL, "");

    FILE *arq = fopen("estacionamento.dat", "r+b");

    if (arq == NULL) {
        arq = fopen("estacionamento.dat", "w+b");
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            return 1;
        }
    }

    int opcao;

    do {
        printf("\n========== Controle de Estacionamento =========\n");
        printf("1. Cadastrar carro\n");
        printf("2. Consultar registro\n");
        printf("3. Consultar tamanho\n");
        printf("0. Sair do programa\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limpa_Buffer();
        switch (opcao) {
            case 1:
                cadastrar(arq);
                break;

            case 2:
                consultar(arq);
                system("PAUSE");
                break;

            case 3: {
                int total = tamanho(arq);
                printf("\nTotal de registros: %d\n", total);
                break;
            }

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    fclose(arq);
    return 0;
}

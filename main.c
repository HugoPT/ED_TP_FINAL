
#include <stdio.h>
#include "BDadosCoupe.h"

#define MIN_MENU 0
#define MAX_MENU 12



int menu_principal() {
    int x;
    printf("\n\n# MENU PRINCIPAL ------------------------------------------#");
    printf("\n| (1) Mostrar Base de Dados                                |");
    printf("\n| (2) Criar Tabela                                         |");
    printf("\n| (3) Mostrar Tabela                                       |");
    printf("\n| (4) Adicionar Campo tabela                               |");
    printf("\n| (5) Adicionar Registos tabela                            |");
    printf("\n| (6) Select Registos                                      |");
    printf("\n| (7) Update de Registos                                   |");
    printf("\n| (8) Apagar Registos                                      |");
    printf("\n| (9) Memoria total/desperdicada                           |");
    printf("\n| (10) Exportar Base de Dados                              |");
    printf("\n| (11) Importar Base de Dados                              |");
    printf("\n| (12) Exportar Tabela                                     |");
    printf("\n| -------------------------------------------------------- |");
    printf("\n|       (0) SAIR                                           |");
    printf("\n#----------------------------------------------------------#\n");
    fflush(stdin);
    do {

        printf("\n  Qual a sua opcao ? ");
        fflush(stdin);
        scanf("%d", &x);
    } while (x < MIN_MENU || x > MAX_MENU);
    return x;
}

//Delegate functions for print list fields
void showTableFields(void *list) {
    if (!list)return;
    ListaGenerica *temp = (ListaGenerica *) list;
    NOG *n = temp->Inicio;
    if (!temp->NEL) return;
    printf("+----------------+--------------+\n");
    printf("| Field          | Type         |\n");
    printf("+----------------+--------------+\n");
    while (n) {
        CAMPO *c = (CAMPO *) n->Info;
        printf("| %s\t\t| %s\t\t|\n", c->NOME_CAMPO, c->TIPO);
        n = n->Prox;
    }
    printf("+----------------+--------------+\n");

}

void showTableHeader(void *list) {
    if (!list)return;
    ListaGenerica *temp = (ListaGenerica *) list;
    NOG *n = temp->Inicio;
    if (!temp->NEL) return;
    printf("+----------------+--------------+\n");
    printf("|      Tabelas na Base de Dados  |\n");
    printf("+----------------+--------------+\n");
    while (n) {
        TABELA *t = n->Info;
        printf("| %s\t\t\t|\n", (char *) t->NOME_TABELA);
        n = n->Prox;
    }
    printf("+----------------+--------------+\n");

}

//Query functions
int equalMatch(char *value1, char *value2) {
    if (value1 == NULL || value2 == NULL) return 0;
    return !strcmp(value1, value2);
}

int greaterThen(char *value1, char *value2) {
    if (value1 == NULL || value2 == NULL) return 0;
    int num_1 = atoi(value1);
    int num_2 = atoi(value2);
    return num_1 > num_2;
}

int lessThen(char *value1, char *value2) {
    if (value1 == NULL || value2 == NULL) return 0;
    int num_1 = atoi(value1);
    int num_2 = atoi(value2);
    return num_1 < num_2;
}

//Delegate functions for print table content
void showTableData(void *list) {
    if (!list)return;
    ListaGenerica *temp = (ListaGenerica *) list;
    if (!temp->NEL) return;
    NOG *n = temp->Inicio;
    printf("+---------Conteudo Tabela--------+\n");
    printf("| ID             | Conteudo         |\n");
    while (n) {
        NOG *p = ((ListaGenerica *) (n->Info))->Inicio;
        while (p) {
            printf("%s\t\t", (char *) p->Info);
            p = p->Prox;
        }
        printf("\n");
        n = n->Prox;
    }
    printf("+----------------+--------------+\n");

}

int main() {

    //Instanciate BD
    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao-1.0");

    //Buffers for user input
    char user_input_buffer1[50];
    char user_input_buffer2[50];
    char user_input_buffer3[50];
    char user_input_buffer4[50];
    int condicao = -1;

    TABELA *t = NULL;


    for (;;) {
        //system("cls");
        switch (menu_principal()) {
            case 1:
                printf("%s\n", BD->NOME_BDADOS);
                printf("%s\n", BD->VERSAO_BDADOS);
                Mostrar_BDados(BD);
                break;
            case 2:
                printf("Qual o nome da tabela que deseja criar?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                Criar_Tabela(BD, strupr(user_input_buffer1));
                system("cls");
                Mostrar_BDados(BD);
                break;
            case 3:
                showTableHeader(BD->LTabelas);
                printf("Qual tabela deseja pesquisar?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                t = Pesquisar_Tabela(BD, strupr(user_input_buffer1));
                Mostrar_Tabela(t);
                if (!t) {
                    printf("Tabela nao encontrada!\n");
                    break;
                }
                break;
            case 4:
                printf("Qual o nome da tabela onde deseja adicionar campos?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                t = Pesquisar_Tabela(BD, strupr(user_input_buffer1));
                if (!t) {
                    printf("Tabela nao encontrada!\n");
                    break;
                }
                printf("Qual o nome do campo a adicionar?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                printf("Qual o tipo de dados? criar? (INT) (STRING) (FLOAT)\n");
                fflush(stdin);
                scanf("%s", user_input_buffer2);
                Add_Campo_Tabela(t, strupr(user_input_buffer1), strupr(user_input_buffer2));

                break;
            case 5:
                printf("Qual a tabela que pretende adicionar registos?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                t = Pesquisar_Tabela(BD, strupr(user_input_buffer1));
                if (!t) {
                    printf("Tabela nao encontrada!");
                    break;
                }
                Mostrar_Tabela(t);
                printf("Indique os dados separados por ; ?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                Add_Valores_Tabela(t, user_input_buffer1);
                Mostrar_Tabela(t);

                break;
            case 6:
                showTableHeader(BD->LTabelas);
                printf("Qual a tabela que pretende?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);

                t = Pesquisar_Tabela(BD, strupr(user_input_buffer1));
                if (!t) {
                    printf("Tabela nao encontrada!");
                    break;
                }
                Mostrar_Tabela(t);

                printf("Qual o Campo a filtrar?\n");
                fflush(stdin);
                scanf("%s", strupr(user_input_buffer1));
                printf("Qual o Valor a comparar?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer2);
                printf("Qual a condicao a aplicar?\n");
                printf("1- Menor que ||  2- Igual a || 3- Maior que\n");
                fflush(stdin);
                scanf("%d", &condicao);
                // Nested switch
                switch (condicao) {
                    case 1:
                        //SELECT(BD, "CLIENTES",equalMatch,"NOME","Hugo");
                        SELECT(BD, t->NOME_TABELA, lessThen, strupr(user_input_buffer1), user_input_buffer2);
                        break;
                    case 2:
                        SELECT(BD, t->NOME_TABELA, equalMatch, strupr(user_input_buffer1), user_input_buffer2);
                        break;
                    case 3:
                        SELECT(BD, t->NOME_TABELA, greaterThen, strupr(user_input_buffer1), user_input_buffer2);
                        break;
                    default:
                        printf("Erro!\n");
                        break;
                }
                break;
            case 7:
                showTableHeader(BD->LTabelas);
                printf("Qual a tabela que pretende?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                t = Pesquisar_Tabela(BD, strupr(user_input_buffer1));
                if (!t) {
                    printf("Tabela nao encontrada!");
                    break;
                }
                Mostrar_Tabela(t);
                //int condicao;
                printf("Indique um campo do registo que pretende alterar?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                printf("Qual o valor do registo nesse campo?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer3);

                printf("Qual a condicao a aplicar?\n");
                printf("1- Menor que ||  2- Igual a || 3- Maior que\n");
                fflush(stdin);
                scanf("%d", &condicao);

                printf("Indique que campo pretende alterar?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer2);

                printf("Indique o novo valor do registo?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer4);
                // Nested switch
                switch (condicao) {
                    case 1:
                        UPDATE(BD, t->NOME_TABELA, lessThen, strupr(user_input_buffer1), user_input_buffer3,
                               strupr(user_input_buffer2), user_input_buffer4);
                        break;
                    case 2:
                        UPDATE(BD, t->NOME_TABELA, equalMatch, strupr(user_input_buffer1), user_input_buffer3,
                               strupr(user_input_buffer2), user_input_buffer4);
                        break;
                    case 3:
                        UPDATE(BD, t->NOME_TABELA, greaterThen, strupr(user_input_buffer1), user_input_buffer3,
                               strupr(user_input_buffer2), user_input_buffer4);
                        break;
                    default:
                        printf("Erro!\n");
                        break;
                }
                break;
            case 8:
                showTableHeader(BD->LTabelas);
                printf("Qual a tabela que pretende?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                t = Pesquisar_Tabela(BD, strupr(user_input_buffer1));
                if (!t) {
                    printf("Tabela nao encontrada!");
                    break;
                }
                Mostrar_Tabela(t);
                printf("Indique um Campo para filtrar registos?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                printf("Qual o Valor a filtrar?\n");
                fflush(stdin);
                scanf("%s", user_input_buffer2);
                printf("Qual a condicao a aplicar?\n");
                printf("1- Menor que ||  2- Igual a || 3- Maior que\n");
                fflush(stdin);
                scanf("%d", &condicao);
                // Nested switch
                switch (condicao) {
                    case 1:
                        DELETE(BD, t->NOME_TABELA, lessThen, strupr(user_input_buffer1), user_input_buffer2);
                        break;
                    case 2:
                        DELETE(BD, t->NOME_TABELA, equalMatch, strupr(user_input_buffer1), user_input_buffer2);
                        break;
                    case 3:
                        DELETE(BD, t->NOME_TABELA, greaterThen, strupr(user_input_buffer1), user_input_buffer2);
                        break;
                    default:
                        printf("Erro!\n");
                        break;
                }
                break;
            case 9:
                Memoria_BDados(BD);
                Memoria_Desperdicada_BDados(BD);
                break;
            case 10:
                printf("Escolha um tipo de ficheiro\n");
                printf("1- Exportar para fExcel || 2- Exportar para fBinario\n");
                fflush(stdin);
                // int option;
                scanf("%d", &condicao);
                // Nested switch
                switch (condicao) {
                    case 1:
                        printf("Insira o nome do ficheiro a criar/atualizar\n");
                        fflush(stdin);
                        scanf("%s", user_input_buffer1);
                        if (Exportar_BDados_Excel(BD, user_input_buffer1)) {
                            printf("Exportado com Sucesso\n");
                        } else {
                            printf("Erro ao exportar !!!\n");
                        }

                        break;
                    case 2:
                        printf("Insira o nome do ficheiro a criar/atualizar\n");
                        fflush(stdin);
                        scanf("%s", user_input_buffer1);
                        if (Exportar_BDados_Ficheiro_Binario(BD, user_input_buffer1)) {
                            printf("Exportado com Sucesso\n");
                        } else {
                            printf("Erro ao exportar !!!\n");
                        }

                        break;
                    default:
                        printf("Erro!\n");
                        break;
                }
                break;
            case 11:
                printf("Escolha um tipo de ficheiro\n");
                printf("1- Importar de fExcel || 2- Importar de fBinario\n");
                fflush(stdin);
                scanf("%d", &condicao);
                // Nested switch
                switch (condicao) {
                    case 1:
                        printf("Insira o nome do ficheiro a importar\n");
                        fflush(stdin);
                        scanf("%s", user_input_buffer1);
                        if (Importar_BDados_Excel(BD, user_input_buffer1)) {
                            printf("Importado com Sucesso, Mostrando BD\n");
                            Mostrar_BDados(BD);
                        }else{
                            printf("Erro ao importar o ficheiro\n");
                        }
                        break;
                    case 2:
                        printf("Insira o nome do ficheiro a importar\n");
                        fflush(stdin);
                        scanf("%s", user_input_buffer1);
                        if (Importar_BDados_Ficheiro_Binario(BD, user_input_buffer1)) {
                            printf("Importado com Sucesso, Mostrando BD\n");
                            Mostrar_BDados(BD);
                        }

                        break;
                    default:
                        printf("Erro\n");
                        break;
                }
                break;
            case 12:
                showTableHeader(BD->LTabelas);
                printf("Escolha a tabela que pretende exportar\n");
                fflush(stdin);
                scanf("%s", user_input_buffer1);
                fflush(stdin);
                printf("Qual o nome do ficheiro a exportar?\n");
                scanf("%s", user_input_buffer2);
                if (Exportar_Tabela_BDados_Excel(BD, strupr(user_input_buffer1), user_input_buffer2)) {
                    printf("Exportado com Sucesso, Mostrando BD\n");
                }
                break;
            case 0:
                Destruir_BDados(BD);
                exit(0);    // Fim do Programa
                break;
        } // END switch (menu_principal())
        //system("pause");
        fflush(stdin);
    }
}




#include <stdio.h>
#include "BDadosCoupe.h"

#define MIN_MENU 1
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
        system("cls");
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
                Add_Campo_Tabela(t, user_input_buffer1, user_input_buffer2);

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
        system("pause");
        fflush(stdin);
    }
}






//#include <stdio.h>
//#include <ctype.h>
//#include "BDadosCoupe.h"
//
//
//int menu_principal() {
//    int x;
//    printf("\n\n# MENU PRINCIPAL ------------------------------------------#");
//    printf("\n| (1) Mostrar Base de Dados                                |");
//    printf("\n| (2) Criar Tabela                                         |");
//    printf("\n| (3) Mostrar Tabela                                       |");
//    printf("\n| (4) Adicionar Campo tabela                               |");
//    printf("\n| (5) Adicionar Registos tabela                            |");
//    printf("\n| (6) Select Registos                                      |");
//    printf("\n| (7) Update de Registos                                   |");
//    printf("\n| (8) Apagar Registos                                      |");
//    printf("\n| (9) Memoria total/desperdicada                           |");
//    printf("\n| (10) Exportar Base de Dados                              |");
//    printf("\n| (11) Importar Base de Dados                              |");
//    printf("\n| -------------------------------------------------------- |");
//    printf("\n|       (0) SAIR                                           |");
//    printf("\n#----------------------------------------------------------#\n");
//    fflush(stdin);
//    do {
//        printf("\n  Qual a sua opcao ? ");
//        fflush(stdin);
//        scanf("%d", &x);
//    } while (x < 0 || x > 11);
//    return x;
//}
//
////Delegate functions for print list fields
//void showTableFields(void *list) {
//    if (!list)return;
//    ListaGenerica *temp = (ListaGenerica *) list;
//    NOG *n = temp->Inicio;
//    if (!temp->NEL) return;
//    printf("+----------------+--------------+\n");
//    printf("| Field          | Type         |\n");
//    printf("+----------------+--------------+\n");
//    while (n) {
//        CAMPO *c = (CAMPO *) n->Info;
//        printf("| %s\t\t| %s\t\t|\n", c->NOME_CAMPO, c->TIPO);
//        n = n->Prox;
//    }
//    printf("+----------------+--------------+\n");
//
//}
//
////Query functions
//int equalMatch(char *value1, char *value2) {
//    if (value1 == NULL || value2 == NULL) return 0;
//    return !strcmp(value1, value2);
//}
//
//int greaterThen(char *value1, char *value2) {
//    if (value1 == NULL || value2 == NULL) return 0;
//    int num_1 = atoi(value1);
//    int num_2 = atoi(value2);
//    return num_1 > num_2;
//}
//
//int lessThen(char *value1, char *value2) {
//    if (value1 == NULL || value2 == NULL) return 0;
//    int num_1 = atoi(value1);
//    int num_2 = atoi(value2);
//    int val = num_1 < num_2;
//    return num_1 < num_2;
//}
//
////Delegate functions for print table content
//void showTableData(void *list) {
//    if (!list)return;
//    ListaGenerica *temp = (ListaGenerica *) list;
//    if (!temp->NEL) return;
//    NOG *n = temp->Inicio;
//    printf("+---------Conteudo Tabela--------+\n");
//    printf("| ID             | Conteudo         |\n");
//    while (n) {
//        NOG *p = ((ListaGenerica *) (n->Info))->Inicio;
//        while (p) {
//            printf("%s\t\t", (char *) p->Info);
//            p = p->Prox;
//        }
//        printf("\n");
//        n = n->Prox;
//    }
//    printf("+----------------+--------------+\n");
//
//}
//
//char *toUpper(char *temp) {
//    return strupr(temp);
//}
//
//
////int main() {
////    //Instanciate BD
////    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao-1.0");
////
////    char buffer[50];
////    for (;;) {
////        system("cls");
////        menu_principal();
////        switch (menu_principal()) {
////            case 1:
////                printf("%s\n", BD->NOME_BDADOS);
////                printf("%s\n", BD->VERSAO_BDADOS);
////                Mostrar_BDados(BD);
////                break;
////            case 2:
////                printf("Qual o nome da tabela que deseja criar?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////                Criar_Tabela(BD, strupr(buffer));
////                system("cls");
////                Mostrar_BDados(BD);
////                break;
////            case 3:
////                Mostrar_BDados(BD);
////                printf("Qual tabela deseja pesquisar?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////
////                TABELA *t = Pesquisar_Tabela(BD, strupr(buffer));
////                Mostrar_Tabela(t);
////                if (!t) {
////                    printf("Tabela nao encontrada!\n");
////                    break;
////                }
////                break;
////            case 4:
////                printf("Qual o nome da tabela onde deseja adicionar campos?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////                TABELA *t2 = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t2) {
////                    printf("Tabela nao encontrada!\n");
////                    break;
////                }
////                char data1[50];
////                char data2[50];
////                printf("Qual o nome do campo a adicionar?\n");
////                fflush(stdin);
////                scanf("%s", data1);
////                printf("Qual o tipo de dados? criar? (INT) (STRING) (FLOAT)\n");
////                fflush(stdin);
////                scanf("%s", data2);
////                Add_Campo_Tabela(t2, strupr(data1), strupr(data2));
////                break;
////            case 5:
////                printf("Qual a tabela que pretende adicionar registos?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////                TABELA *t3 = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t3) {
////                    printf("Tabela nao encontrada!");
////                    break;
////                }
////                Mostrar_Tabela(t3);
////                printf("Indique os dados separados por ; ?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////                Add_Valores_Tabela(t3, strupr(buffer));
////                Mostrar_Tabela(t3);
////                break;
////            case 6:
////                printf("Qual a tabela que pretende?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////                TABELA *t = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t) {
////                    printf("Tabela nao encontrada!");
////                    break;
////                }
////                Mostrar_Tabela(t);
////
////                char ncampo[50];
////                char valorcomparacao[50];
////                int condicao;
////                printf("Qual o Campo a filtrar?\n");
////                fflush(stdin);
////                scanf("%s", ncampo);
////                printf("Qual o Valor a comparar?\n");
////                fflush(stdin);
////                scanf("%s", valorcomparacao);
////                printf("Qual a condicao a aplicar?\n");
////                printf("1- Menor que ||  2- Igual a || 3- Maior que\n");
////                fflush(stdin);
////                scanf("%d", &condicao);
////                // Nested switch
////                switch (condicao) {
////                    case 1:
////                        SELECT(BD, t->NOME_TABELA, lessThen, ncampo, valorcomparacao);
////                        break;
////                    case 2:
////                        SELECT(BD, t->NOME_TABELA, equalMatch, ncampo, valorcomparacao);
////                        break;
////                    case 3:
////                        SELECT(BD, t->NOME_TABELA, greaterThen, ncampo, valorcomparacao);
////                        break;
////                    default:
////                        printf("Erro!\n");
////                        break;
////                }
////                break;
////            case 7:
////                printf("Qual a tabela que pretende?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////                TABELA *t = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t) {
////                    printf("Tabela nao encontrada!");
////                    break;
////                }
////                Mostrar_Tabela(t);
////
////                char ncampoup[50];
////                char ncampoalt[50];
////                char valorcomparacaoup[50];
////                char novovalor[50];
////                int condicaoo;
////                printf("Indique um campo do registo que pretende alterar?\n");
////                fflush(stdin);
////                scanf("%s", ncampoup);
////                printf("Qual o valor do registo nesse campo?\n");
////                fflush(stdin);
////                scanf("%s", valorcomparacaoup);
////                printf("Qual a condicao a aplicar?\n");
////                printf("1- Menor que ||  2- Igual a || 3- Maior que\n");
////                fflush(stdin);
////                scanf("%d", &condicaoo);
////                printf("Indique que campo pretende alterar?\n");
////                fflush(stdin);
////                scanf("%s", ncampoalt);
////                printf("Indique o novo valor do registo?\n");
////                fflush(stdin);
////                scanf("%s", novovalor);
////                // Nested switch
////                switch (condicaoo) {
////                    case 1:
////                        UPDATE(BD, t->NOME_TABELA, lessThen, ncampoup, valorcomparacaoup, ncampoalt, novovalor);
////                        break;
////                    case 2:
////                        UPDATE(BD, t->NOME_TABELA, equalMatch, ncampoup, valorcomparacaoup, ncampoalt, novovalor);
////                        break;
////                    case 3:
////                        UPDATE(BD, t->NOME_TABELA, greaterThen, ncampoup, valorcomparacaoup, ncampoalt, novovalor);
////                        break;
////                    default:
////                        printf("Erro!\n");
////                        break;
////                }
////                break;
////            case 8:
////                printf("Qual a tabela que pretende?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
////                TABELA *t = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t) {
////                    printf("Tabela nao encontrada!");
////                    break;
////                }
////                Mostrar_Tabela(t);
////                char ncampodel[50];
////                char valorcomparacaodel[50];
////                int condicaooo;
////                printf("Indique um Campo para filtrar registos?\n");
////                fflush(stdin);
////                scanf("%s", ncampodel);
////                printf("Qual o Valor a filtrar?\n");
////                fflush(stdin);
////                scanf("%s", valorcomparacaodel);
////                printf("Qual a condicao a aplicar?\n");
////                printf("1- Menor que ||  2- Igual a || 3- Maior que\n");
////                fflush(stdin);
////                scanf("%d", &condicaooo);
////                // Nested switch
////                switch (condicaooo) {
////                    case 1:
////                        DELETE(BD, t->NOME_TABELA, lessThen, ncampodel, valorcomparacaodel);
////                        break;
////                    case 2:
////                        DELETE(BD, t->NOME_TABELA, equalMatch, ncampodel, valorcomparacaodel);
////                        break;
////                    case 3:
////                        DELETE(BD, t->NOME_TABELA, greaterThen, ncampodel, valorcomparacaodel);
////                        break;
////                    default:
////                        printf("Erro!\n");
////                        break;
////                }
////                break;
////            case 9:
////                Memoria_BDados(BD);
////                Memoria_Desperdicada_BDados(BD);
////                break;
////            case 10:
////                printf("Escolha um tipo de ficheiro\n");
////                printf("1- Exportar para fExcel || 2- Exportar para fBinario\n");
////                fflush(stdin);
////                int option;
////                scanf("%d", &option);
////                // Nested switch
////                switch (option) {
////                    case 1:
////                        printf("Insira o nome do ficheiro a criar/atualizar\n");
////                        fflush(stdin);
////                        char ficheir_csv[50];
////                        char extension[5] = ".csv";
////                        char *file_name = NULL;
////                        scanf("%s", ficheir_csv);
////                        char *have_extension = strstr(ficheir_csv, ".csv");
////                        //Handle file passed with extension or just file name
////                        if (!have_extension) {
////                            //Passed file has no extension.We add it for you :-)
////                            printf("Nao tem  extensao, adicionando por si\n");
////                            file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + strlen(extension) + 1);
////                            strcpy(file_name, ficheir_csv);
////                            strcat(file_name, extension);
////
////                        } else {
////                            //Passed file has extension. All good to go :)
////                            file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
////                            strcpy(file_name, ficheir_csv);
////
////                        }
////                        if (Exportar_BDados_Excel(BD, file_name)) {
////                            printf("Exportado com Sucesso\n");
////                        } else {
////                            printf("Falha na exportacao para excel\n");
////                        }
////                        free(file_name);
////                        break;
////                    case 2:
////                        printf("Insira o nome do ficheiro a criar/atualizar\n");
////                        fflush(stdin);
////                        char fich_dat[50];
////                        char extensiondat[5] = ".dat";
////                        char *file_namedat = NULL;
////                        scanf("%s", fich_dat);
////                        char *have_extensiondat = strstr(fich_dat, ".dat");
////                        //Handle file passed with extension or just file name
////                        if (!have_extensiondat) {
////                            //Passed file has no extension.We add it for you :-)
////                            printf("Nao tem  extensao, adicionando por si\n");
////                            file_namedat = (char *) malloc(sizeof(char) * strlen(fich_dat) + strlen(extensiondat) + 1);
////                            strcpy(file_namedat, fich_dat);
////                            strcat(file_namedat, extensiondat);
////                        } else {
////                            //Passed file has extension. All good to go :)
////                            file_namedat = (char *) malloc(sizeof(char) * strlen(fich_dat) + 1);
////                            strcpy(file_namedat, fich_dat);
////                        }
////                        if (Exportar_BDados_Ficheiro_Binario(BD, file_namedat)) {
////                            printf("Exportado com Sucesso\n");
////                        } else {
////                            printf("Falha na exportacao para excel\n");
////                        }
////                        free(file_namedat);
////                        break;
////                    default:
////                        printf("Erro!\n");
////                        break;
////                }
////                break;
////            case 11:
////                printf("Escolha um tipo de ficheiro\n");
////                printf("1- Importar de fExcel || 2- Importar de fBinario\n");
////                fflush(stdin);
////                int opcaoo;
////                scanf("%d", &opcaoo);
////                // Nested switch
////                switch (opcaoo) {
////                    case 1:
////                        printf("Insira o nome do ficheiro a importar\n");
////                        fflush(stdin);
////                        char ficheir_csv[50];
////                        char extension[5] = ".csv";
////                        char *file_name = NULL;
////                        scanf("%s", ficheir_csv);
////                        char *have_extension = strstr(ficheir_csv, ".csv");
////                        //Handle file passed with extension or just file name
////                        if (!have_extension) {
////                            //Passed file has no extension.We add it for you :-)
////                            printf("Nao tem  extensao, adicionando por si\n");
////                            file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + strlen(extension) + 1);
////                            strcpy(file_name, ficheir_csv);
////                            strcat(file_name, extension);
////                        } else {
////                            //Passed file has extension. All good to go :)
////                            file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
////                            strcpy(file_name, ficheir_csv);
////                        }
////                        if (Importar_BDados_Excel(BD, file_name)) {
////                            printf("Importado com Sucesso, Mostrando BD\n");
////                            Mostrar_BDados(BD);
////                        }
////                        free(file_name);
////                        break;
////                    case 2:
////                        printf("Insira o nome do ficheiro a importar\n");
////                        fflush(stdin);
////                        char ficheir_dat[50];
////                        char extensiondat[5] = ".dat";
////                        char *fich_dat = NULL;
////                        scanf("%s", ficheir_dat);
////                        char *have_extensiondat = strstr(ficheir_dat, ".dat");
////                        //Handle file passed with extension or just file name
////                        if (!have_extensiondat) {
////                            //Passed file has no extension.We add it for you :-)
////                            printf("Nao tem  extensao, adicionando por si\n");
////                            fich_dat = (char *) malloc(sizeof(char) * strlen(ficheir_dat) + strlen(extensiondat) + 1);
////                            strcpy(fich_dat, ficheir_dat);
////                            strcat(fich_dat, extensiondat);
////                        } else {
////                            //Passed file has extension. All good to go :)
////                            fich_dat = (char *) malloc(sizeof(char) * strlen(ficheir_dat) + 1);
////                            strcpy(fich_dat, ficheir_dat);
////                        }
////                        printf("fichdat: %s", fich_dat);
////                        if (Importar_BDados_Ficheiro_Binario(BD, fich_dat)) {
////                            printf("Importado com Sucesso, Mostrando BD\n");
////                            Mostrar_BDados(BD);
////                        }
////                        free(fich_dat);
////                        break;
////                    default:
////                        printf("Erro\n");
////                        break;
////                }
////                break;
////            case 0:
////                Destruir_BDados(BD);
////                exit(0);    // Fim do Programa
////                break;
////        } // END switch (menu_principal())
////        system("pause");
////    }
////}
//

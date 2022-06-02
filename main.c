#include <stdio.h>
#include <ctype.h>
#include "BDadosCoupe.h"


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
    printf("\n| -------------------------------------------------------- |");
    printf("\n|       (0) SAIR                                           |");
    printf("\n#----------------------------------------------------------#\n");
    fflush(stdin);
    do {
        printf("\n  Qual a sua opcao ? ");
        fflush(stdin);
        scanf("%d", &x);
    } while (x < 0 || x > 11);
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
    int val = num_1 < num_2;
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

char *toUpper(char *temp) {
    return strupr(temp);
}


//int main() {
//    //Instanciate BD
//    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao-1.0");
//
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
////                printf("Qual tabela deseja pesquisar?\n");
////                fflush(stdin);
////                scanf("%s", buffer);
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
////                TABELA *t4 = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t4) {
////                    printf("Tabela nao encontrada!");
////                    break;
////                }
////                Mostrar_Tabela(t4);
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
////                        SELECT(BD, t4->NOME_TABELA, lessThen, ncampo, valorcomparacao);
////                        break;
////                    case 2:
////                        SELECT(BD, t4->NOME_TABELA, equalMatch, ncampo, valorcomparacao);
////                        break;
////                    case 3:
////                        SELECT(BD, t4->NOME_TABELA, greaterThen, ncampo, valorcomparacao);
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
////                TABELA *t5 = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t5) {
////                    printf("Tabela nao encontrada!");
////                    break;
////                }
////                Mostrar_Tabela(t5);
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
////                        UPDATE(BD, t5->NOME_TABELA, lessThen, ncampoup, valorcomparacaoup, ncampoalt, novovalor);
////                        break;
////                    case 2:
////                        UPDATE(BD, t5->NOME_TABELA, equalMatch, ncampoup, valorcomparacaoup, ncampoalt, novovalor);
////                        break;
////                    case 3:
////                        UPDATE(BD, t5->NOME_TABELA, greaterThen, ncampoup, valorcomparacaoup, ncampoalt, novovalor);
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
////                TABELA *t6 = Pesquisar_Tabela(BD, strupr(buffer));
////                if (!t6) {
////                    printf("Tabela nao encontrada!");
////                    break;
////                }
////                Mostrar_Tabela(t6);
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
////                        DELETE(BD, t6->NOME_TABELA, lessThen, ncampodel, valorcomparacaodel);
////                        break;
////                    case 2:
////                        DELETE(BD, t6->NOME_TABELA, equalMatch, ncampodel, valorcomparacaodel);
////                        break;
////                    case 3:
////                        DELETE(BD, t6->NOME_TABELA, greaterThen, ncampodel, valorcomparacaodel);
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
////                            free(file_name);
////                        } else {
////                            //Passed file has extension. All good to go :)
////                            file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
////                            strcpy(file_name, ficheir_csv);
////                            free(file_name);
////                        }
////                        if (Exportar_BDados_Excel(BD, file_name)) {
////                            printf("Exportado com Sucesso\n");
////                        } else {
////                            printf("Falha na exportacao para excel\n");
////                        }
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
//}

int main() {

    //buffer que o user vai escrever no menu
//    char str[50 ];
//    printf("Indique a opt");
//    scanf("%s",str);
//
//    printf("Sai em upper case-%s", strupr(str));



    //Instanciate BD
   // string *bbb = splitString("OLA|Tudo");

    //Importar_BDados_Excel(BD, "new.csv");
   // Mostrar_BDados(BD);

    //Importar_BDados_Excel(BD, "EXPBD.csv");
    //Exportar_BDados_Excel(BD,"new.csv");

    //Destruir_BDados(BD);

   // Exportar_BDados_Ficheiro_Binario(BD,"ab.dat");
   // Importar_BDados_Ficheiro_Binario(BD,"ab.dat");
   // Mostrar_BDados(BD);
    //Exportar_Tabela_BDados_Excel(BD,"CLIENTES","asdasdas.csv");


    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao-1.0");
    Importar_BDados_Excel(BD, "a.csv");




//    //Create Table for Clients
  /* TABELA *clientes = Criar_Tabela(BD, "CLIENTES");
//    //Create Table for Moradas
    TABELA *moradas = Criar_Tabela(BD, "MORADAS");
//    //Create table for cities
    TABELA *cidades = Criar_Tabela(BD, "CIDADES");
    //Add fields to clients
    Add_Campo_Tabela(clientes, "ID", "INT");
    Add_Campo_Tabela(clientes, "NOME", "STRING");
    Add_Campo_Tabela(clientes, "IDADE", "INT");
    //Add fields to addresses
    Add_Campo_Tabela(moradas, "ID", "INT");
    Add_Campo_Tabela(moradas, "LOCALIDADE", "STRING");
//    //Add fields to cities
    Add_Campo_Tabela(cidades, "ID", "INT");
    Add_Campo_Tabela(cidades, "NOME", "STRING");

////
//    //Add values to table clients
    Add_Valores_Tabela(clientes, "1;Hugo;37");
    Add_Valores_Tabela(clientes, "2;Andre;22");
    Add_Valores_Tabela(clientes, "3;Paulo;22");
    Add_Valores_Tabela(clientes, "4;Filipe;45");
    //Add_Valores_Tabela_BDados(BD, "CLIENTES", "5;Ana;34");

    Add_Valores_Tabela(moradas, "1;RuaDoVoltaAtraz");
    Add_Valores_Tabela(moradas, "2;Ruadireita");
    Add_Valores_Tabela(moradas, "3;Ruatorta");
    Add_Valores_Tabela(moradas, "4;Ruacentral");
    Add_Valores_Tabela(moradas, "5;RuaViseu");
    Add_Valores_Tabela_BDados(BD, "MORADAS", "6;RuaDaTorre");

    Add_Valores_Tabela(cidades, "1;Viseu-NELAS");
    Add_Valores_Tabela(cidades, "2;Lisboa");
    Add_Valores_Tabela(cidades, "3;Evora");
    //Exportar_BDados_Excel(BD,"new.csv");


    //Add_Valores_Tabela_BDados(BD, "CIDADES", "4;Porto");
//
//    //Show List with user delegate function
    // MostrarLG(moradas->LCampos, showTableFields);
    //MostrarLG(moradas->LRegistos, showTableData);

    // MostrarLG(cidades->LCampos, showTableFields);
    //MostrarLG(cidades->LRegistos, showTableData);

*/
//
//    //Show List with user delegate function
//    // MostrarLG(moradas->LCampos, showTableFields);
//    //Show List with user delegate function
//    // MostrarLG(moradas->LRegistos, showTableData);
//

    // Mostrar_Tabela_NOME(BD, "CIDADES");


    //Search for a table in DB
//    TABELA *t = Pesquisar_Tabela(BD, "CLIENTES");
//    t != NULL ? printf("Tabela encontrada %s\n", t->NOME_TABELA) : printf("!!! Tabela nao encontrada !!!\n");
//    MostrarLG(clientes->LRegistos, showTableData);
//    DELETE_TABLE_DATA(t);
//
//    //MostrarLG(clientes->LRegistos, showTableData);
//    Add_Valores_Tabela(clientes, "1;hugo;37");
    //  Add_Valores_Tabela(clientes, "2;Andre;22");
//    Add_Valores_Tabela(clientes, "3;Paulo;22");
//    Add_Valores_Tabela_BDados(BD, "CLIENTES", "4;Andrubal;33");
//    MostrarLG(clientes->LRegistos, showTableData);

    //Show wasted memory
    //Memoria_Desperdicada_BDados(BD);
    //CRUD Functions
    //-------
    //Memoria_Desperdicada_BDados(BD);

//    DROP_TABLE(BD, "CIDADES");
//    printf("Existem %d tabelas\n",BD->LTabelas->NEL);

//    printf("------------------\n");
//    DROP_TABLE(BD, "CLIENTES");
//
//    DROP_TABLE(BD, "MORADAS");
    // DROP_TABLE(BD, "CLIENTES");


    //Funcao permite passar a extensao ou nao internamente adiciona e aloca memoria dinamica
    // Exportar_BDados_Excel(BD,"dados_exportados");
//    Exportar_BDados_Excel(BD,"dados_exportados2.csv");
//
//    Exportar_Tabela_BDados_Excel(BD,"CIDADES","cidades");
//    Exportar_Tabela_BDados_Excel(BD,"MORADAS","moradas.csv");
//
//    Mostrar_BDados(BD);
    //Memoria_Desperdicada_BDados(BD);
    // MostrarLG(BD->LTabelas, showTableData);
    //showTableData(moradas);
    //Show wasted memory
    //Memoria_Desperdicada_BDados(BD);

    //Dealoc DB
    //Show DB


    // Mostrar_BDados(BD);


//    TABELA *t = Pesquisar_Tabela(BD, "MORADAS");
//    Mostrar_Tabela(t);
//
// SELECT(BD, "MORADAS", greaterThen, "ID", "3");
    //Exportar_BDados_Excel(BD,"ExpBD.csv");

    Mostrar_BDados(BD);

    //Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));

   // SELECT(BD, "CLIENTES", greaterThen, "ID", "1");
    //Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));
    //UPDATE(BD,"CLIENTES",equalMatch,"ID","2","NOME","AndreFilipe");
    Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));
    DELETE(BD,"CLIENTES",greaterThen,"ID","3");
    Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));
   // Mostrar_BDados(BD);
//    Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));
//    Add_Valores_Tabela(Pesquisar_Tabela(BD,"CLIENTES"), "3;Paulo;22");
//    Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));
    //Mostrar_BDados(BD);
    //Destruir_BDados(BD);

    // Memoria_Desperdicada_BDados(BD);
    Destruir_BDados(BD);

    return 0;
}

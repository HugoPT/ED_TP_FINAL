#include <stdio.h>
#include <ctype.h>
#include "BDadosCoupe.h"


int menu_principal() {
    int x;
    printf("\n\n# MENU PRINCIPAL ------------------------------------------#");
    printf("\n| (1) Criar Tabela                    |");
    printf("\n| (2) Mostrar Tabela   |");
    printf("\n| (3) Adicionar campo tabela   |");
    printf("\n| (3) Adicionar registos na tabela                 |");
    printf("\n| (4) Exportar Tabela->CSV               |");

    printf("\n| -------------------------------------------------------- |");
    printf("\n|       (0) SAIR                                           |");
    printf("\n#----------------------------------------------------------#\n");
    fflush(stdin);
    do {
        printf("\n  Qual a sua opcao ? ");
        fflush(stdin);
        scanf("%d", &x);
    } while (x < 0 || x > 10);
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


char *toUpper(char * temp) {

    return strupr (temp);
}

int main() {

    //buffer que o user vai escrever no menu
//    char str[50 ];
//    printf("Indique a opt");
//    scanf("%s",str);
//
//    printf("Sai em upper case-%s", strupr(str));



    //Instanciate BD
   // string *bbb = splitString("OLA|Tudo");
    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao-1.0");
    //Importar_BDados_Excel(BD, "new.csv");
   // Mostrar_BDados(BD);

    //Importar_BDados_Excel(BD, "EXPBD.csv");
    //Exportar_BDados_Excel(BD,"new.csv");

    //Destruir_BDados(BD);

   // Exportar_BDados_Ficheiro_Binario(BD,"ab.dat");
    Importar_BDados_Ficheiro_Binario(BD,"ab.dat");
    Mostrar_BDados(BD);
    //Importar_BDados_Excel(BD, "ExpBD.csv");

//    char buffer[50];
//    for (;;) {
//        system("cls");
//        switch (menu_principal()) {
//            case 1:
//                printf("Qual o nome da tabela que deseja criar?\n");
//                fflush(stdin);
//                scanf("%s", buffer);
//
//                Criar_Tabela(BD, buffer);
//                Mostrar_BDados(BD);
//                break;
//            case 2:
//                Mostrar_BDados(BD);
//                break;
//            case 3:
//                printf("Qual o nome da tabela que adicionar campos? criar?\n");
//                fflush(stdin);
//                scanf("%s", buffer);
//                TABELA *t = Pesquisar_Tabela(BD, buffer);
//                if (!t) {
//                    printf("Tabela nao encontrada!!!\n");
//                    break;
//                }
//                char data1[50];
//                char data2[50];
//                printf("Qual o nome do campos? criar?\n");
//                fflush(stdin);
//                scanf("%s", data1);
//                printf("Qual o tipo de dados? criar? (INT) (STRING) (FLOAT)\n");
//                fflush(stdin);
//                scanf("%s", data2);
//                Add_Campo_Tabela(t, data1, data2);
//                break;
//            case 4:
//                printf("Qual a tabela que prentende adicionar registos?\n");
//                fflush(stdin);
//                scanf("%s", buffer);
//                TABELA *t2 = Pesquisar_Tabela(BD, buffer);
//                if (!t2) {
//                    printf("Tabela nao encontrada");
//                    break;
//                }
//                Mostrar_Tabela(t2);
//                printf("Indique os dados separados por ; ?\n");
//                fflush(stdin);
//                scanf("%s", buffer);
//                Add_Valores_Tabela(t2, buffer);
//                Mostrar_Tabela(t2);
//                break;
//                break;
//            case 5:
//                SELECT(BD, "MORADAS", equalMatch, "LOCALIDADE", "RuaViseu");
//                break;
//            case 6:
//                printf("opca6\n");
//                break;
//            case 7:
//                printf("opcao7\n");
//                break;
//            case 8:
//                printf("opcao8\n");
//                break;
//            case 9:
//                printf("opcao9\n");
//                break;
//            case 10:  // para facilitar os testes
//                printf("opcao10\n");
//                break;
//            case 0:
//                Destruir_BDados(BD);
//                exit(0);    // Fim do Programa
//                break;
//        } //END switch (menu_principal())
//        //system("pause");1
//    }


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

    */
    //Add_Valores_Tabela_BDados(BD, "CIDADES", "4;Porto");
//
//    //Show List with user delegate function
    // MostrarLG(moradas->LCampos, showTableFields);
    //MostrarLG(moradas->LRegistos, showTableData);

    // MostrarLG(cidades->LCampos, showTableFields);
    //MostrarLG(cidades->LRegistos, showTableData);


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

    //Mostrar_BDados(BD);

    Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));

    SELECT(BD, "CLIENTES", lessThen, "ID", "3");
    //UPDATE(BD,"CLIENTES",lessThen,"ID","2","NOME","AndreFilipe");
    //DELETE(BD,"CLIENTES",lessThen,"ID","3");

//    Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));
//    Add_Valores_Tabela(Pesquisar_Tabela(BD,"CLIENTES"), "3;Paulo;22");
//    Mostrar_Tabela(Pesquisar_Tabela(BD,"CLIENTES"));
    //Mostrar_BDados(BD);
    Destruir_BDados(BD);

    // Memoria_Desperdicada_BDados(BD);

    return 0;
}

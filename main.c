#include <stdio.h>
#include "BDadosCoupe.h"

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
int equalMatch(char * value1, char*value2){
    return !strcmp(value1,value2);
}
int greaterThen(char *value1, char *value2){
    int num_1 = atoi(value2);
    int num_2 = atoi(value2);
    return value1>value2;

}
int lessThen(int value1, int value2){
    return value1>value2;

}



//Delegate functions for print table content
void showTableData(void *list) {
    if (!list)return;
    ListaGenerica *temp = (ListaGenerica *) list;
    //printf("A lista contem %d registos\n", temp->NEL);
    if (temp->NEL / 2 == 0) return;

    // printf("A lista contem %d registos\n", temp->NEL / 2);
    NOG *n = temp->Inicio;
    printf("+---------Conteudo Tabela--------+\n");
    //printf("+----------------+--------------+\n");
    printf("| ID             | Nome         |\n");
    int formatter = 1;
    while (n) {
        if (formatter > 2) {
            printf("|\n");
            formatter = 1;
        }
        printf("%s\t\t", (char*)n->Info);
        formatter++;
        n = n->Prox;
    }
    printf("|\n");
    printf("+----------------+--------------+\n");

}

int main() {
    //Instanciate BD
ListaGenerica *l = CriarLG();
    printf("Tamanho ocupado pela lista %d",sizeof (l));

    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao-1.0");
//    //Create Table for Clients
    TABELA *clientes = Criar_Tabela(BD, "CLIENTES");
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
    Add_Valores_Tabela_BDados(BD, "CLIENTES", "5;Ana;34");

////todo problema no meio ????
//    Add_Valores_Tabela(moradas, "1;RuaDoVoltaAtraz");
//    Add_Valores_Tabela(moradas, "2;Ruadireita");
//    Add_Valores_Tabela(moradas, "3;Ruatorta");
////
    Add_Valores_Tabela(cidades, "1;Viseu NELAS");
    Add_Valores_Tabela(cidades, "2;Lisboa");
    Add_Valores_Tabela(cidades, "3;Evora");
    Add_Valores_Tabela_BDados(BD, "CIDADES", "4;Porto");
////
//
//    //Show List with user delegate function
//    MostrarLG(moradas->LCampos, showTableFields);
//    MostrarLG(moradas->LRegistos, showTableData);
////
//    MostrarLG(cidades->LCampos, showTableFields);
//    MostrarLG(cidades->LRegistos, showTableData);


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
//    MostrarLG(clientes->LRegistos, showTableData);
//    Add_Valores_Tabela(clientes, "1;hugo");
//    Add_Valores_Tabela(clientes, "2;Andre");
//    Add_Valores_Tabela(clientes, "3;Paulo");
//    Add_Valores_Tabela_BDados(BD, "CLIENTES", "4;Andrubal");
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


    Mostrar_BDados(BD);



    SELECT(BD,"MORADAS",equalMatch,"Nome","2");
    Destruir_BDados(BD);

    // Memoria_Desperdicada_BDados(BD);

    return 0 ;
}

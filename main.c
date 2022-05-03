#include <stdio.h>
#include "BDadosCoupe.h"

//Delegate functions for print list fields
void showTableFields(void *list) {
    if (!list)return;
    ListaGenerica *temp = (ListaGenerica *) list;
    printf("A lista contem %d campos\n", temp->NEL);
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

//Delegate functions for print table content
void showTableData(void *list) {
    if (!list)return;
    ListaGenerica *temp = (ListaGenerica *) list;
    printf("A listbbbbbbbba contem %d registos\n", temp->NEL);
    if (temp->NEL /2 == 0) return;

    printf("A lista contem %d registos\n", temp->NEL / 2);
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
        printf("%s\t\t", n->Info);
        formatter++;
        n = n->Prox;
    }
    printf("|\n");
    printf("+----------------+--------------+\n");

}

int main() {
    //Instanciate BD
    BDadosCoupe *BD = Criar_BDados("BD-Banco", "Versao 1.0");
    //Create Table for Clients
    TABELA *clientes = Criar_Tabela(BD, "CLIENTES");
    //Create Table for Moradas
    TABELA *moradas = Criar_Tabela(BD, "MORADAS");
    TABELA *cidades = Criar_Tabela(BD, "CIDADES");
    //Add fields to clients
    Add_Campo_Tabela(clientes, "ID", "INT");
    Add_Campo_Tabela(clientes, "NOME", "STRING");
    Add_Campo_Tabela(clientes, "IDADE", "INT");
    //Add fields to addresses
    Add_Campo_Tabela(moradas, "ID", "INT");
    Add_Campo_Tabela(moradas, "LOCALIDADE", "STRING");

    Add_Campo_Tabela(cidades, "ID", "INT");
    Add_Campo_Tabela(cidades, "NOME", "STRING");

    //Add values to table clients
    Add_Valores_Tabela(clientes, "1;Hugo");
    Add_Valores_Tabela(clientes, "2;Andre");
    Add_Valores_Tabela(clientes, "3;Paulo");
    // Add_Valores_Tabela_BDados(BD, "CLIENTES", "4;Andrubal");


    Add_Valores_Tabela(cidades, "1;Viseu");
    //Add values to table addresses
    Add_Valores_Tabela(moradas, "1;Viseu");
    Add_Valores_Tabela(moradas, "2;Lisboa");
    //Add_Valores_Tabela_BDados(BD, "MORADAS", "3;Porto");


    //Show List with user delegate function
   // MostrarLG(clientes->LCampos, showTableFields);
   // MostrarLG(clientes->LRegistos, showTableData);

    //MostrarLG(cidades->LRegistos, showTableData);
    //MostrarLG(cidades->LRegistos, showTableData);

    //Show List with user delegate function
    // MostrarLG(moradas->LCampos, showTableFields);
    //Show List with user delegate function
    // MostrarLG(moradas->LRegistos, showTableData);


   // Mostrar_Tabela_NOME(BD, "MORADAS");
    //Show DB
  //  Mostrar_BDados(BD);

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

    //DROP_TABLE(BD, "CIDADES");
    //DROP_TABLE(BD, "MORADAS");
    //DROP_TABLE(BD, "CLIENTES");
    printf("Tabelas %d\n", BD->LTabelas->NEL);

   // MostrarLG(BD->LTabelas, showTableData);
    //showTableData(moradas);
    //Show wasted memory
    //Memoria_Desperdicada_BDados(BD);

    //Dealoc DB
    Destruir_BDados(BD);

    return 0;
}
